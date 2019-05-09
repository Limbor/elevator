#include "ElevatorDrawer.h"

ElevatorDrawer* ElevatorDrawer::createDrawer(int number) {
	ElevatorDrawer* enemy = new ElevatorDrawer();
	if (enemy && enemy->init())
	{
		enemy->autorelease();
		enemy->enemyInit(number);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}
bool ElevatorDrawer::init() {
	if (!Layer::init())
	{
		return false;
	}
	return true;
}
void ElevatorDrawer::enemyInit(int number) {
	this->number = number;
	
	curentfFloor = Label::createWithSystemFont(String::createWithFormat("%i", 1)->getCString(), "Arial", 60);
	curentfFloor->setColor(Color3B::RED);
	curentfFloor->setPosition(35 , 250);
	addChild(curentfFloor);
	/*for (int i = 0; i < 20; i++) {

	}*/
	setPosition(755 + number * 210, 300);
}
