#ifndef GREEDYCHOPPER_H
#define GREEDYCHOPPER_H

#include "HelloWorldScene.h"
#include "cocos2d.h"

#include "SimpleAudioEngine.h"

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
	int graph[30][30], chopper[2][100];
	int length, head, tail, direction, beanX, beanY, x, y, level, life, speed;
	int sumTime;
	cocos2d::CCSprite* sprite[54][54];
	bool flag, lifeFlag;
	float gameSpeed;
	cocos2d::CCLabelTTF* levelTag;
	cocos2d::CCLabelTTF* timeTag;
	cocos2d::CCLabelTTF* lifeTag;
	cocos2d::CCLabelTTF* speedTag;
};

#endif  // __HELLOWORLD_SCENE_H__