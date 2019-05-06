#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ElevatorScheduler.h"

USING_NS_CC;


class MainScene : public cocos2d::Scene
{
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	ui::Button *floorButton[20][2];
	Label* elevatorNumber[5][20];
	Sprite* elevatorUP[5][20];
	Sprite* elevatorDOWN[5][20];
	//DrawNode *elevator[5];
	ElevatorScheduler *es = new ElevatorScheduler();
public:
    static cocos2d::Scene* createScene();
	void drawBuilding();
	void drawElevatorState();
    virtual bool init();
	virtual void update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // __HELLOWORLD_SCENE_H__
