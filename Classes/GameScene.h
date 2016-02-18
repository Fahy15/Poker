#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

struct s_Pokers
{
	Sprite* poker;    //��������
	Sprite* pokerBack;    //���Ʊ���
	int flag;    //����״̬��־��0 ���� 1 ���������Ա� 2 �Ա���ȷ
};

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void gotoMenuCallback(cocos2d::Ref* pSender);
	bool onTouchBegan(Touch* touch, Event* event);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void setRow(int row);
	void setCol(int col);
public:
	std::vector<s_Pokers> pokers;
	const int POKERCOUNT = 54;
private:
	int row, col;
};

#endif // __HELLOWORLD_SCENE_H__