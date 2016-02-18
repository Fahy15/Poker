#include "HelpScene.h"
#include "MainMenu.h"

USING_NS_CC;

Scene * HelpScene::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}

bool HelpScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto size = Director::getInstance()->getWinSize();
	auto visible = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//ÉèÖÃ±³¾°
	auto spriteBK = Sprite::create("background.png");
	spriteBK->setPosition(size.width / 2, size.height / 2);
	spriteBK->setOpacity(75);
	this->addChild(spriteBK, 0);
	//¶ÁÈ¡xml×Ö·û
	CCDictionary* message = CCDictionary::createWithContentsOfFile("chinese.xml");
	auto helpTitleKey = message->valueForKey("helpTitle");
	const char* helpTitle = helpTitleKey->getCString();
	//°ïÖúÐÅÏ¢
	auto helpTitleLabel = Label::createWithTTF(helpTitle, "fonts/newSongTi.ttf", 25);
	helpTitleLabel->setPosition(Point(
		size.width - helpTitleLabel->getContentSize().width,
		size.height - helpTitleLabel->getContentSize().height));
	this->addChild(helpTitleLabel, 1);

	auto helpMessageKey = message->valueForKey("helpMessage");
	const char* helpMessage = helpMessageKey->getCString();
	auto helpMessageLabel = Label::createWithTTF(helpMessage, "fonts/newSongTi.ttf",20);
	helpMessageLabel->setDimensions(300,200);
	helpMessageLabel->enableOutline(Color4B(255, 0, 0, 0), 1);
	//helpMessageLabel->setAnchorPoint(Point(1,1));
	/*helpMessageLabel->setPosition(Point(
		size.width - helpTitleLabel->getContentSize().width,
		size.height - (helpTitleLabel->getContentSize().height*2)));*/
	helpMessageLabel->setPosition(size.width / 2, size.height / 2);
	this->addChild(helpMessageLabel, 1);

	//·µ»Ø°´Å¥
	auto menuItemBack = MenuItemFont::create("Back", CC_CALLBACK_1(HelpScene::gotoMenuCallback, this));
	auto menu = Menu::create(menuItemBack, NULL);
	menu->setPosition(Point::ZERO);
	menuItemBack->setPosition(Point(
		size.width - menuItemBack->getContentSize().width,
		menuItemBack->getContentSize().height));
	this->addChild(menu, 2);
	return true;
}

void HelpScene::gotoMenuCallback(Ref * object)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->popScene();
}
