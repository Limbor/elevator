#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class ElevatorDrawer :public Layer
{
private:
	int number;
	LayerColor *bg;
public:
	ui::Button *floors[20];
	Label *curentfFloor;
	static ElevatorDrawer* createDrawer(int number);
	virtual bool init();
	void enemyInit(int number);
};
