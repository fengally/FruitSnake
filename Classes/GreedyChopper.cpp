#include "HelloWorldScene.h"
#include "GreedyChopper.h"

using namespace cocos2d;

CCScene* GreedyChopper::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		GreedyChopper *layer = GreedyChopper::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GreedyChopper::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		memset(map, 0, sizeof(map));
		//墙壁,在边缘
		for (int i = 0; i < 24; i++)
		{
			map[0][i] = map[23][i] = 1;
			map[i][0] = map[i][23] = 1;
		}
		//记录贪吃蛇所在的坐标
		// 初始化节点数量
		snakeNum = 4;
		// 初始化节点数据

		for (int i = 0; i < snakeNum; i++) {
			snake[i][0] = 10-i;  
			snake[i][1] = 4;
			if(i==0){
				snake_sprite[i]=CCSprite::create("head.png");
			}else{
				snake_sprite[i]=CCSprite::create("tail.png");
			}
			snake_sprite[i]->setPosition(ccp(BLOCK_W * snake[i][0]+BLOCK_W/2, BLOCK_W * snake[i][1]+BLOCK_W/2 ));
			this->addChild(snake_sprite[i], 2);

		}
		srand(time(0));

		do
		{
			foodX = rand() % 22 + 1;
			foodY = rand() % 22 + 1;
		} while (map[foodX][foodY]);  
		map[foodX][foodY] = 2;         //随机产生食物

		//把map数组都画上 sprite，这个就得在init初始化的时候画好，不用随循环每次都画。
		for (int i = 0; i < 24; i++)
		{
			for (int j = 0; j < 24; j++)
			{
				if (map[i][j] == 1)
					sprite[i][j] = CCSprite::create("wall.png");
				/*else if (map[i][j] == 2)
				sprite[i][j] = CCSprite::create("tail.png");
				else if (map[i][j] == 3)
				sprite[i][j] = CCSprite::create("head.png");
				*/
				else
					sprite[i][j] = CCSprite::create("grid.png");
				sprite[i][j]->setPosition(ccp(BLOCK_W * i+BLOCK_W/2, BLOCK_W * j+BLOCK_W/2));
				this->addChild(sprite[i][j], 1);
			}
		}

		//关于地图 以及snake的初始化完成，以下是关于各种记录和按钮的初始化
		///////////////////////////////////////////////////////////////////////////////////////


		//方向默认向上，自动前进速度默认为800ms，等级为1
		life = 3, direction = DIRECTION_RIGHT, gameSpeed = 500, level = 1, speed = 20 / (gameSpeed / 1000);
		flag = true, lifeFlag = true;
		sumTime = 0;
		char life_str[24], level_str[24], time_str[24], speed_str[24];
		sprintf(life_str, "Life: %d", life);
		lifeTag = CCLabelTTF::create(life_str, "font09.fnt", 20);
		CC_BREAK_IF(!lifeTag);
		lifeTag->setPosition(ccp(630, 460));
		this->addChild(lifeTag, 1);
		sprintf(level_str, "Level: %d", level);
		levelTag = CCLabelTTF::create(level_str, "font09.fnt", 20);
		CC_BREAK_IF(!levelTag);
		levelTag->setPosition(ccp(630, 440));
		this->addChild(levelTag, 1);
		sprintf(time_str, "Time: %d s", sumTime / 1000);
		timeTag = CCLabelTTF::create(time_str, "font09.fnt", 20);
		CC_BREAK_IF(!levelTag);
		timeTag->setPosition(ccp(630, 420));
		this->addChild(timeTag, 1);
		sprintf(speed_str, "Speed: %d", speed);
		speedTag = CCLabelTTF::create(speed_str, "font09.fnt", 20);
		CC_BREAK_IF(!speedTag);
		speedTag->setPosition(ccp(630, 400));
		this->addChild(speedTag, 1);
		this->schedule(schedule_selector(GreedyChopper::step), gameSpeed / 1000);
		CCMenuItemImage *up = CCMenuItemImage::create(
			"up.png",
			"up1.png",
			this,
			menu_selector(GreedyChopper::clickUp));
		CC_BREAK_IF(! up);
		up->setPosition(ccp(630, 280));
		CCMenuItemImage *down = CCMenuItemImage::create(
			"down.png",
			"down1.png",
			this,
			menu_selector(GreedyChopper::clickDown));
		CC_BREAK_IF(! down);
		down->setPosition(ccp(630, 200));
		CCMenuItemImage *right = CCMenuItemImage::create(
			"right.png",
			"right1.png",
			this,
			menu_selector(GreedyChopper::clickRight));
		CC_BREAK_IF(! right);
		right->setPosition(ccp(685, 240));
		CCMenuItemImage *left = CCMenuItemImage::create(
			"left.png",
			"left1.png",
			this,
			menu_selector(GreedyChopper::clickLeft));
		CC_BREAK_IF(! left);
		left->setPosition(ccp(575, 240));
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(GreedyChopper::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);

		// Place the menu item bottom-right conner.
		pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

		CCMenuItemImage *pStartItem = CCMenuItemImage::create(
			"newGame.png", 
			"newGame1.png",
			this,
			menu_selector(GreedyChopper::menuOnNewGame));
		CC_BREAK_IF(! pStartItem);
		pStartItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 150, 20));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(up, down, right, left, pCloseItem, pStartItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		// 2. Add a label shows "Hello World".

		// Create a label and initialize with string "Hello World".

		// 3. Add add a splash screen, show the cocos2d splash image.
		CCSprite* pSprite = CCSprite::create("welcome.jpg");
		pSprite->setOpacity(90);
		CC_BREAK_IF(! pSprite);

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		// Place the sprite on the center of the screen
		pSprite->setPosition(ccp(size.width/2, size.height/2));

		// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(pSprite, 0);


		bRet = true;
	} while (0);

	return bRet;
}

void GreedyChopper::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void GreedyChopper::menuOnNewGame(CCObject* pSender)
{
	CCScene* scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		HelloWorld* pScene = HelloWorld::create();
		CC_BREAK_IF(!pScene);
		scene->addChild(pScene);
		CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.5, scene));
	} while (0);
}

void GreedyChopper::clickUp(CCObject* pSender)
{
	if (flag)
		direction = DIRECTION_UP;
}

void GreedyChopper::clickDown(CCObject* pSender)
{
	if (flag)
		direction = DIRECTION_DOWN;
}

void GreedyChopper::clickRight(CCObject* pSender)
{
	if (flag)
		direction = DIRECTION_RIGHT;
}

void GreedyChopper::clickLeft(CCObject* pSender)
{
	if (flag)
		direction = DIRECTION_LEFT;
}

void GreedyChopper::step(float dt)
{
	if (flag)
		sumTime += gameSpeed;
	this->removeChild(timeTag, true);
	char time_str[24];
	sprintf(time_str, "Time: %d s", sumTime / 1000);
	timeTag = CCLabelTTF::create(time_str, "font09.fnt", 20);
	timeTag->setPosition(ccp(630, 420));
	this->addChild(timeTag, 1);




	// 第一个单元格移动

	switch (direction) {

	case DIRECTION_UP:

		x = snake[0][0] ;
		y = snake[0][1] + 1;
		lifeFlag = true;
		break;

	case DIRECTION_DOWN:

		x = snake[0][0] ;
		y = snake[0][1] - 1;
		lifeFlag = true;
		break;

	case DIRECTION_LEFT:

		x = snake[0][0] - 1;
		y = snake[0][1];
		lifeFlag = true;
		break;

	case DIRECTION_RIGHT:

		x = snake[0][0] + 1;
		y = snake[0][1];
		lifeFlag = true;
		break;

	}





	//贪吃蛇触碰到墙壁，Game Over
	if ((x == 0 || x == 23 || y == 0 || y == 23) || (map[x][y] && !(x == foodX && y == foodY)))
	{
		if (life && lifeFlag)
			life--;
		this->removeChild(lifeTag, true);
		char life_str[24];
		sprintf(life_str, "Life: %d", life);
		lifeTag = CCLabelTTF::create(life_str, "font09.fnt", 20);
		lifeTag->setPosition(ccp(630, 460));
		this->addChild(lifeTag, 1);
		direction = 0;
		if (!life)
		{
			flag = false;
			CCLabelTTF* pLabel = CCLabelTTF::create("Game Over!", "font09.fnt", 24);

			// Get window size and place the label upper. 
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			pLabel->setPosition(ccp(630, 70));

			// Add the label to HelloWorld layer as a child layer.
			this->addChild(pLabel, 1);
		}
		return;
	}
	//吃到苹果时候
	else if (x == foodX && y == foodY)
	{
		//snake长度+1
		snakeNum++;
		snake_sprite[snakeNum-1]=CCSprite::create("tail.png");
		snake_sprite[snakeNum-1]->setPosition(ccp(BLOCK_W * foodX+BLOCK_W/2, BLOCK_W * foodY+BLOCK_W/2));
		this->addChild(snake_sprite[snakeNum-1], 1);


		//随机产生个food
		do
		{
			foodX = rand() % 22 + 1;
			foodY = rand() % 22 + 1;
		} while (0);
		int i=foodX,j=foodY;
		this->removeChild(sprite[i][j], true);
		sprite[i][j] = CCSprite::create("tail.png");
		sprite[i][j]->setPosition(ccp(BLOCK_W * i+BLOCK_W/2, BLOCK_W * j+BLOCK_W/2));
		this->addChild(sprite[i][j], 1);
		/*length++;
		if (length >= 8)
		{
		length -= 8;
		level++;
		if (gameSpeed > 50)
		gameSpeed -= 50;
		this->removeChild(levelTag, true);
		char level_str[24];
		sprintf(level_str, "Level: %d", level);
		levelTag = CCLabelTTF::create(level_str, "font09.fnt", 20);
		levelTag->setPosition(ccp(630, 430));
		this->addChild(levelTag, 1);
		this->removeChild(speedTag, true);
		char speed_str[24];
		speed = 20 / (gameSpeed / 1000);
		sprintf(speed_str, "Speed: %d", speed);
		speedTag = CCLabelTTF::create(speed_str, "font09.fnt", 20);
		speedTag->setPosition(ccp(630, 410));
		this->addChild(speedTag, 1);
		}
		//2为tail    3head    1wall   
		map[foodX][foodY] = 0;
		//原来的头部就变成了tail(tail是除了头部的其他部分)
		//map[snake[0][head]][snake[1][head]] = 2;
		int i=snake[0][head],j=snake[1][head];
		map[i][j] = 2;
		this->removeChild(sprite[i][j], true);
		sprite[i][j] = CCSprite::create("tail.png");
		sprite[i][j]->setPosition(ccp(20 * i + 5, 20 * j + 5));
		this->addChild(sprite[i][j], 1);
		head = (head + 1) % 100;

		//苹果点 设为 head
		//map[x][y] = 3;
		i=x,j=y;
		map[i][j] = 3;
		this->removeChild(sprite[i][j], true);
		sprite[i][j] = CCSprite::create("head.png");
		sprite[i][j]->setPosition(ccp(20 * i + 5, 20 * j + 5));
		this->addChild(sprite[i][j], 1);


		//设此点为head点 
		snake[0][head] = x;
		snake[1][head] = y;
		srand(time(0));
		do
		{
		foodX = rand() % 22 + 1;
		foodY = rand() % 22 + 1;
		} while (map[foodX][foodY]);
		//随机一个点 作为下一个苹果位置，这个点的贴图居然和tail是一样的。tail不是尾部的一个，是除了head之外所有的。
		//map[foodX][foodY] = 2;
		i=foodX,j=foodY;
		map[i][j] = 2;
		this->removeChild(sprite[i][j], true);
		sprite[i][j] = CCSprite::create("tail.png");
		sprite[i][j]->setPosition(ccp(20 * i + 5, 20 * j + 5));
		this->addChild(sprite[i][j], 1);
		*/
	}
	//也没吃到苹果，也没撞墙的时候。
	else
	{

		/*
		//tail的格子设为0，就是grid了。将最后一个tail设为grid
		int i=snake[0][tail],j=snake[1][tail];
		map[i][j] = 0;
		this->removeChild(sprite[i][j], true);
		sprite[i][j] = CCSprite::create("grid.png");
		sprite[i][j]->setPosition(ccp(20 * i + 5, 20 * j + 5));
		this->addChild(sprite[i][j], 1);
		tail = (tail + 1) % 100;

		//head设为tail
		i=snake[0][head],j=snake[1][head];
		map[i][j] = 2;
		this->removeChild(sprite[i][j], true);
		sprite[i][j] = CCSprite::create("tail.png");
		sprite[i][j]->setPosition(ccp(20 * i + 5, 20 * j + 5));
		this->addChild(sprite[i][j], 1);
		head = (head + 1) % 100;

		//这个点设为head
		i=x,j=y;
		map[i][j] = 3;
		this->removeChild(sprite[i][j], true);
		sprite[i][j] = CCSprite::create("head.png");
		sprite[i][j]->setPosition(ccp(20 * i + 5, 20 * j + 5));
		this->addChild(sprite[i][j], 1);

		//记录头的数组也设为这个点的坐标。
		snake[0][head] = x;
		snake[1][head] = y;
		*/
	}

	// 蛇身移动

	for (int i = snakeNum; i > 0; i--) {

		//最后一节坐标=倒数第二个坐标，而最后一节坐标被丢弃
		snake[i][0] = snake[i - 1][0];

		snake[i][1] = snake[i - 1][1];

	}

	snake[0][0] = x;
	snake[0][1] = y;





	//判断好了 移动好了坐标，以下开始画snake的每个sprite，和随机出来的food sprite
	//	snake_sprite数组 每个精灵，的坐标都设置为 snake里面存储的坐标
	for (int i = 0; i < snakeNum; i++)
	{
		int x1=snake[i][0];
		int y1=snake[i][1];
		snake_sprite[i]->setPosition(ccp(BLOCK_W * x1+BLOCK_W/2, BLOCK_W * y1+BLOCK_W/2));
		//this->addChild(snake_sprite[i], 1);
		//snake_sprite[i]

	}



	/*
	for (int i = 0; i < 24; i++)
	{
	for (int j = 0; j < 24; j++)
	this->removeChild(sprite[i][j], true);
	}
	for (int i = 0; i < 24; i++)
	{
	for (int j = 0; j < 24; j++)
	{
	if (map[i][j] == 1)
	sprite[i][j] = CCSprite::create("wall.png");
	else if (map[i][j] == 2)
	sprite[i][j] = CCSprite::create("tail.png");
	else if (map[i][j] == 3)
	sprite[i][j] = CCSprite::create("head.png");
	else
	sprite[i][j] = CCSprite::create("grid.png");
	sprite[i][j]->setPosition(ccp(20 * i + 5, 20 * j + 5));
	this->addChild(sprite[i][j], 1);
	}
	}
	*/
}