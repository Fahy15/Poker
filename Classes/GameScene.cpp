#include "GameScene.h"
#include "MainMenu.h"

#define kInAngleZ        -90 //���濨�Ƶ���ʼZ��Ƕ�
#define kInDeltaZ        90  //���濨����ת��Z��ǶȲ�

#define kOutAngleZ       0   //���濨�Ƶ���ʼZ��Ƕ�
#define kOutDeltaZ       -90  //���濨����ת��Z��ǶȲ�

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
			//��˳���ȡ���ƣ������ÿ��ƾ����tagΪ���ֵ�����
			sprintf(pokerName, "%d", pokerNum);    //Ϊ�˸��õĿ���ֲ�ԣ�ʹ��sprintf
			//itoa(pokerNum,pokerName,10);    //itoaΪwindows�Ŀ⺯���������˿���ֲ��
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
	//������л�pokers
	std::srand(time(NULL));    //�������
	std::random_shuffle(this->pokers.begin(),this->pokers.end());

	Size pokerSize = this->pokers.at(0).poker->getContentSize();
	for (int i = 0, vectorNum = 0; i < row; i++)
	{
		for (int j = 0; j < col && vectorNum < POKERCOUNT; j++, vectorNum++)
		{
			//�������棬����
			s_Pokers s_poker = this->pokers.at(vectorNum);
			Sprite* poker = s_poker.poker;
			poker->setAnchorPoint(Vec2(0, 1));
			poker->setPosition(Vec2(10+j*(pokerSize.width+1) + origin.x, visibleSize.height-i*(pokerSize.height+1)-10 + origin.y));
			poker->setVisible(false);
			this->addChild(poker);

			//���Ʒ��棬��ʾ
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
	//����������pokers
	for (auto i = pokers.begin(); i != pokers.end(); i++)
	{
		//�жϿ����Ƿ����泯��
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
	mutex.lock();    // lock, ���ⶨʱ���ظ�����
	//����������pokers
	for (int i = 0; i < pokers.size(); i++)
	{
		s_Pokers* pPoker = &pokers[i];
		//�жϿ����Ƿ����泯��
		if (1 == pPoker->flag)
		{
			vPoker.push_back(pPoker);
		}

		//�жϵ��λ�������ſ���
		Rect pokerRect(pPoker->poker->getPositionX(), (pPoker->poker->getPositionY() - pPoker->poker->getContentSize().height),
			pPoker->poker->getContentSize().width, pPoker->poker->getContentSize().height);
		if (pokerRect.containsPoint(location))
		{
			//�жϵ��λ�õĿ����Ƿ�Ϊ����
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
	// ��Ȼ�Ǵ��ڵ���2, ��ֻ��2���ƾͻᴦ��
	if (vPoker.size() >= 2)
	{
		vPoker[0]->pokerBack->setVisible(true);
		//�ڶ��������淭����
		vPoker[0]->poker->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
			Hide::create(),
			NULL));
		vPoker[0]->pokerBack->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
			CCCallFuncND::create(this, callfuncND_selector(GameScene::closeCard), vPoker[0]),
			NULL));

		//��һ�������淭����
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
			//�жϷ������������Ƿ�Ϊ��ͬ��С
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