#include "AboutScene.h"
#include "MainMenu.h"

USING_NS_CC;

cocos2d::Scene * AboutScene::createScene()
{
	auto scene = Scene::create();
	auto layer = AboutScene::create();
	scene->addChild(layer);
	return scene;
}

bool AboutScene::init()
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
	auto aboutTitleKey = message->valueForKey("aboutTitle");
	const char* aboutTitle = aboutTitleKey->getCString();
	//°ïÖúÐÅÏ¢
	auto aboutTitleLabel = Label::createWithTTF(aboutTitle, "fonts/newSongTi.ttf", 25);
	aboutTitleLabel->setPosition(Point(
		size.width - aboutTitleLabel->getContentSize().width,
		size.height - aboutTitleLabel->getContentSize().height));
	this->addChild(aboutTitleLabel, 1);

	auto aboutMessageKey = message->valueForKey("aboutMessage");
	const char* aboutMessage = aboutMessageKey->getCString();
	auto aboutMessageLabel = Label::createWithTTF(aboutMessage, "fonts/newSongTi.ttf", 20);
	aboutMessageLabel->setDimensions(300, 200);
	aboutMessageLabel->enableOutline(Color4B(255, 0, 0, 0), 1);
	//aboutMessageLabel->setAnchorPoint(Point(1,1));
	/*aboutMessageLabel->setPosition(Point(
	size.width - aboutTitleLabel->getContentSize().width,
	size.height - (aboutTitleLabel->getContentSize().height*2)));*/
	aboutMessageLabel->setPosition(size.width / 2, size.height / 2);
	this->addChild(aboutMessageLabel, 1);

	//·µ»Ø°´Å¥
	auto menuItemBack = MenuItemFont::create("Back", CC_CALLBACK_1(AboutScene::gotoMenuCallback, this));
	auto menu = Menu::create(menuItemBack, NULL);
	menu->setPosition(Point::ZERO);
	menuItemBack->setPosition(Point(
		size.width - menuItemBack->getContentSize().width,
		menuItemBack->getContentSize().height));
	this->addChild(menu, 2);
	return true;
}

void AboutScene::gotoMenuCallback(cocos2d::Ref * pSender)
{
	auto scene = MainMenu::createScene();
	Director::getInstance()->popScene();
}
