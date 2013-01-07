#ifndef GREEDYCHOPPER_H
#define GREEDYCHOPPER_H

#include "HelloWorldScene.h"
#include "cocos2d.h"

#include "SimpleAudioEngine.h"
//#define BLOCK_W 20

class GreedyChopper: public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	//a selector starting
	void menuOnNewGame(CCObject* pSender);

	// a selector callback
	void menuCloseCallback(CCObject* pSender);

	void clickUp(CCObject* pSender);
	void clickDown(CCObject* pSender);
	void clickRight(CCObject* pSender);
	void clickLeft(CCObject* pSender);
	void step(float dt);

	// implement the "static node()" method manually
	CREATE_FUNC(GreedyChopper);
private:
	static  const int BLOCK_W =20,DIRECTION_UP=1,DIRECTION_DOWN=2,DIRECTION_RIGHT=3,DIRECTION_LEFT=4;
	int map[30][30], snake[100][2];//[0][0] ,[0][1] is zhe x and y of head .
	int snakeNum, head, tail, direction, foodX, foodY, x, y, level, life, speed;
	int sumTime;
	//all zhe map  is sprite , sprite set position ,create with pic.
	cocos2d::CCSprite* sprite[30][30];
	cocos2d::CCSprite* snake_sprite[100];
	cocos2d::CCSprite* pfood;
	bool flag, lifeFlag;
	float gameSpeed;
	cocos2d::CCLabelTTF* levelTag;
	cocos2d::CCLabelTTF* timeTag;
	cocos2d::CCLabelTTF* lifeTag;
	cocos2d::CCLabelTTF* speedTag;
};

#endif  // __HELLOWORLD_SCENE_H__