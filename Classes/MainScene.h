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

	DrawNode *floor[20];								//楼层线
	Label *floorNumber[20];								//楼层号
	ui::Button *floorButton[20][2];						//每楼的上下电梯呼叫按钮

	Label *elevatorNumber[5][20];						//每层楼电梯楼层的显示器
	Sprite *elevatorUP[5][20];							//每层楼电梯上状态显示器
	Sprite *elevatorDOWN[5][20];						//每层楼电梯下状态显示器

	LayerColor *bg[5];									//电梯背景板
	ui::Button *floors[5][20];							//电梯内楼层按钮
	ui::Button *warning[5];								//电梯报警按钮
	ui::Button *door[5][2];								//开关门按钮
	DrawNode *elevatorState[5][2];						//电梯运行状态显示器
	Label *curentfFloor[5];								//电梯内楼层显示器

	ElevatorScheduler *es = new ElevatorScheduler();	//电梯调度器
public:
    static cocos2d::Scene* createScene();
	void drawBuilding();								//初始化大楼
	void drawElevatorState();							//初始化楼层电梯显示器
	void drawElevator();								//初始化电梯内部显示
    virtual bool init();								//主场景初始化
	virtual void update(float dt);						//每帧刷新电梯内外部楼层状态显示
	CREATE_FUNC(MainScene);
};

#endif // __HELLOWORLD_SCENE_H__
