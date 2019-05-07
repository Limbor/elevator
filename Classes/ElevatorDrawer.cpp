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
	bg = LayerColor::create(Color4B::BLACK, 200, 300);
	addChild(bg);
	setPosition(755 + number * 210, 300);
}
