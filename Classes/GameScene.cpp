#include "GameScene.h"
#include "MainMenu.h"

#define kInAngleZ        -90 //里面卡牌的起始Z轴角度
#define kInDeltaZ        90  //里面卡牌旋转的Z轴角度差

#define kOutAngleZ       0   //封面卡牌的起始Z轴角度
#define kOutDeltaZ       -90  //封面卡牌旋转的Z轴角度差

USING_NS_CC;
using namespace std;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto menuItem = MenuItemFont::create(
		"Back",
		CC_CALLBACK_1(GameScene::gotoMenuCallback, this));
	menuItem->setColor(Color3B(255, 0, 0));
	menuItem->setPosition(Vec2(origin.x + visibleSize.width - menuItem->getContentSize().width / 2,
		origin.y + menuItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(menuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	this->setRow(6);
	this->setCol(9);
	int pokerNum = 0;
	char* jpg = ".jpg";
	char pokerName[7];
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++, pokerNum++)
		{
			//按顺序读取卡牌，并设置卡牌精灵的tag为名字的数字
			sprintf(pokerName, "%d", pokerNum);    //为了更好的可移植性，使用sprintf
			//itoa(pokerNum,pokerName,10);    //itoa为windows的库函数，削弱了可移植性
			strcat(pokerName, jpg);
			auto sprite = Sprite::create(pokerName);
			sprite->setTag(pokerNum);
			s_Pokers spoker;
			spoker.poker = sprite;
			spoker.flag = 0;
			spoker.pokerBack = Sprite::create("back.png");

			this->pokers.push_back(spoker);
		}
	}
	//随机序列化pokers
	std::srand(time(NULL));    //随机种子
	std::random_shuffle(this->pokers.begin(),this->pokers.end());

	Size pokerSize = this->pokers.at(0).poker->getContentSize();
	for (int i = 0, vectorNum = 0; i < row; i++)
	{
		for (int j = 0; j < col && vectorNum < POKERCOUNT; j++, vectorNum++)
		{
			//卡牌正面，隐藏
			s_Pokers s_poker = this->pokers.at(vectorNum);
			Sprite* poker = s_poker.poker;
			poker->setAnchorPoint(Vec2(0, 1));
			poker->setPosition(Vec2(10+j*(pokerSize.width+1) + origin.x, visibleSize.height-i*(pokerSize.height+1)-10 + origin.y));
			poker->setVisible(false);
			this->addChild(poker);

			//卡牌反面，显示
			Sprite* pokerBack = s_poker.pokerBack;
			pokerBack->setAnchorPoint(Vec2(0, 1));
			pokerBack->setPosition(Vec2(10 + j*(pokerSize.width + 1) + origin.x, visibleSize.height - i*(pokerSize.height + 1) - 10 + origin.y));
			this->addChild(pokerBack);

		}
	}
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameScene::gotoMenuCallback(Ref * object)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->popScene();
}

void GameScene::setRow(int row)
{
	this->row = row;
}

void GameScene::setCol(int col)
{
	this->col = col;
}

void GameScene::openCard(Node* pSend, void* pPoker)
{
	s_Pokers* poker = (s_Pokers*)pPoker;
	if (poker && 0 == poker->flag)
	{
		poker->flag = 1;
	}
}

void GameScene::closeCard(Node* pSend, void* pPoker)
{
	s_Pokers* poker = (s_Pokers*)pPoker;
	if (poker)
	{
		poker->flag = 0;
	}
}


void GameScene::resetAllCards(float dt)
{
	mutex.lock();
	//迭代器遍历pokers
	for (auto i = pokers.begin(); i != pokers.end(); i++)
	{
		//判断卡牌是否正面朝上
		if (0 != (*i).flag)
		{
			(*i).pokerBack->setVisible(true);
			(*i).poker->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
				Hide::create(),
				NULL));
			(*i).pokerBack->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
				CCCallFuncND::create(this, callfuncND_selector(GameScene::closeCard), &(*i)),
				NULL));
		}
	}
	mutex.unlock();
}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 location = touch->getLocation();
	std::vector<s_Pokers*> vPoker;
	s_Pokers* clickPoker = nullptr;
	mutex.lock();    // lock, 避免定时器重复操作
	//迭代器遍历pokers
	for (int i = 0; i < pokers.size(); i++)
	{
		s_Pokers* pPoker = &pokers[i];
		//判断卡牌是否正面朝上
		if (1 == pPoker->flag)
		{
			vPoker.push_back(pPoker);
		}

		//判断点击位置在哪张卡牌
		Rect pokerRect(pPoker->poker->getPositionX(), (pPoker->poker->getPositionY() - pPoker->poker->getContentSize().height),
			pPoker->poker->getContentSize().width, pPoker->poker->getContentSize().height);
		if (pokerRect.containsPoint(location))
		{
			//判断点击位置的卡牌是否为反面
			if (0 == pPoker->flag)
			{
				pPoker->poker->setVisible(true);
				pPoker->pokerBack->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
					Hide::create(),
					NULL));
				pPoker->poker->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
					CCCallFuncND::create(this, callfuncND_selector(GameScene::openCard), pPoker),
					NULL));
				clickPoker = pPoker;
			}
		}
	}
	// 虽然是大于等于2, 但只有2张牌就会处理
	if (vPoker.size() >= 2)
	{
		vPoker[0]->pokerBack->setVisible(true);
		//第二张牌正面翻反面
		vPoker[0]->poker->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
			Hide::create(),
			NULL));
		vPoker[0]->pokerBack->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
			CCCallFuncND::create(this, callfuncND_selector(GameScene::closeCard), vPoker[0]),
			NULL));

		//第一张牌正面翻反面
		vPoker[1]->pokerBack->setVisible(true);
		vPoker[1]->poker->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
			Hide::create(),
			NULL));
		vPoker[1]->pokerBack->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
			CCCallFuncND::create(this, callfuncND_selector(GameScene::closeCard), vPoker[1]),
			NULL));
	}
	else if (vPoker.capacity() >= 1)
	{
		if (clickPoker)
		{
			s_Pokers* poker = vPoker[0];
			//判断翻开的两张牌是否为相同大小
			if ((clickPoker->poker->getTag() / 4) == (poker->poker->getTag() / 4))
			{
				poker->flag = 2;
				clickPoker->flag = 2;
				scheduleOnce(schedule_selector(GameScene::resetAllCards), 3.0f);
			}
		}
	}
	mutex.unlock();

	return true;
}