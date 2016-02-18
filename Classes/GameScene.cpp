#include "GameScene.h"
#include "MainMenu.h"

#define kInAngleZ        270 //���濨�Ƶ���ʼZ��Ƕ�
#define kInDeltaZ        90  //���濨����ת��Z��ǶȲ�

#define kOutAngleZ       0   //���濨�Ƶ���ʼZ��Ƕ�
#define kOutDeltaZ       90  //���濨����ת��Z��ǶȲ�

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

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	Vec2 location = touch->getLocation();
	s_Pokers* poker;
	int count = 0;
	//����������pokers
	for (auto i = pokers.begin(); i != pokers.end(); i++)
	{
		//�жϵ��λ�������ſ���
		Rect pokerRect((*i).poker->getPositionX(), ((*i).poker->getPositionY() - (*i).poker->getContentSize().height), (*i).poker->getContentSize().width, (*i).poker->getContentSize().height);
		if (pokerRect.containsPoint(location))
		{
			//�жϵ��λ�õĿ����Ƿ�Ϊ����
			if (0 == (*i).flag)
			{
				(*i).poker->setVisible(true);
				(*i).pokerBack->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
					Hide::create(),
					NULL));
				(*i).poker->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
					NULL));
				(*i).flag = 1;
			}
		}
		else
		{
			//�жϿ����Ƿ����泯��
			if (1 == (*i).flag)
			{
				count += 1;
				if (2 <= count)
				{
					//�жϷ������������Ƿ�Ϊ��ͬ��С
					if (((*i).poker->getTag() / 4) == (poker->poker->getTag() / 4))
					{
						poker->flag = 2;
						(*i).flag = 2;
					}
					else
					{
						(*i).pokerBack->setVisible(true);
						//�ڶ��������淭����
						(*i).poker->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
							Hide::create(),
							NULL));
						(*i).pokerBack->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
							NULL));
						(*i).flag = 0;

						//��һ�������淭����
						poker->pokerBack->setVisible(true);
						poker->poker->runAction((ActionInterval *)Sequence::create(CCOrbitCamera::create(0.1, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
							Hide::create(),
							NULL));
						poker->pokerBack->runAction((ActionInterval *)Sequence::create(OrbitCamera::create(0.1, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
							NULL));
						poker->flag = 0;
					}
				}
				poker = &(*i);
			}
		}
	}
	return true;
}