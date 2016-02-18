#include "MainMenu.h"
#include "AboutScene.h"
#include "HelpScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size winSize = Director::getInstance()->getWinSize();
	percent = (visibleSize.height / winSize.height) < (visibleSize.width / winSize.width) ?
		      (visibleSize.height / winSize.height) : (visibleSize.width / winSize.width);
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

	auto playItem = MenuItemImage::create(
											"play.png",
		                                    "play.png",
											CC_CALLBACK_1(MainMenu::startGameCallback, this));
	auto aboutItem = MenuItemImage::create(
		                                   "about.png",
		                                   "about.png",
		                                   CC_CALLBACK_1(MainMenu::aboutCallback, this));
	auto helpItem = MenuItemImage::create(
		                                  "help.png",
		                                  "help.png",
		                                  CC_CALLBACK_1(MainMenu::helpCallback, this));

	playItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - playItem->getContentSize().width / 2,
		                       origin.y + visibleSize.height / 2 + aboutItem->getContentSize().height - playItem->getContentSize().height / 2 + 10));
	aboutItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - closeItem->getContentSize().width / 2,
		                        origin.y + visibleSize.height / 2 - aboutItem->getContentSize().height / 2));
	helpItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - helpItem->getContentSize().width / 2,
		                       origin.y + visibleSize.height / 2 -aboutItem->getContentSize().height - helpItem->getContentSize().height / 2 - 10));
    // create menu, it's an autorelease object
    auto menu = Menu::create(playItem, helpItem, closeItem, aboutItem, NULL);
	menu->setScale(percent);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    //label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                        origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    //this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("background.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setScale(percent);
	sprite->setOpacity(90);

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    return true;
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainMenu::startGameCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->pushScene(GameScene::createScene());
}

void MainMenu::helpCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->pushScene(HelpScene::createScene());
}

void MainMenu::aboutCallback(cocos2d::Ref * pSender)
{
	Director::getInstance()->pushScene(AboutScene::createScene());
}
