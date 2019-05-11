#pragma once
#include <mutex>
#include <thread>
#include <vector>
#include "Person.h"
#include "cocos2d.h"

using namespace std;
class Elevator : public cocos2d::DrawNode
{
private:
	int floor;													//所处楼层
	int workState;												//工作状态
	int doorState;												//门开关状态
	bool isBroken;												//电梯是否损坏
	int number = 0;												//电梯号
public:
	static const int UP = 1;
	static const int DOWN = -1;	
	static const int WAIT = 0;
	static const int OPEN = 4;
	static const int CLOSE = 5;

	int arrive[21] = { 0 };										//楼层指令
	vector<Person*> requestList;								//需求列表
	vector<Person*> arriveList;									//到达列表
	mutex elevatorLock;											//电梯锁

	static Elevator* createElevator(int number) {				//新建电梯及初始化
		Elevator* enemy = new Elevator();
		if (enemy && enemy->init())
		{
			enemy->autorelease();
			enemy->enemyInit(number);
			return enemy;
		}
		CC_SAFE_DELETE(enemy);
		return NULL;
	}
	virtual bool init() {
		if (!cocos2d::DrawNode::init())
		{
			return false;
		}
		return true;
	}
	void enemyInit(int number) {
		floor = 1;
		workState = WAIT;
		doorState = CLOSE;
		isBroken = false;
		this->number = number;
		cocos2d::DrawNode::create();
		cocos2d::DrawNode::drawSolidRect(cocos2d::Vec2(190 + 120 * number, 0), cocos2d::Vec2(240 + 120 * number, 45), cocos2d::Color4F::BLACK);
		//电梯运转线程开启
		std::thread t(&Elevator::work, this);
		t.detach();
	}
	int getFloor() { return floor; }
	int getWorkState() { return workState; }
	bool getIsBroken() { return isBroken; }
	void setWorkState(int state) { workState = state; }
	void setDoorState(int state) { doorState = state; }
	void hasBroken() { isBroken = true; }
	int getDoorState() { return doorState; }
	void addRequest(Person* person) {							//添加需求
		if (requestList.size() == 0) {
			requestList.push_back(person);
		}
		else {
			for (vector<Person*>::iterator i = requestList.begin(); i != requestList.end(); i++)
			{
				if ((*i)->getStart() >= person->getStart()) {
					requestList.insert(i, person);
					return;
				}
			}
		}
	}
	void addArriveList(Person* person) {						//添加到达
		arrive[person->getEnd()] = 1;
		if (arriveList.size() == 0) {
			arriveList.push_back(person);
			if (workState == WAIT && person->getEnd() != floor) workState = (person->getEnd() - floor) > 0 ? UP : DOWN;
		}
		else {
			for (vector<Person*>::iterator i = arriveList.begin(); i != arriveList.end(); i++) {
				if ((*i)->getEnd() >= person->getEnd()) {
					arriveList.insert(i, person);
					return;
				}
			}
		}
	}
	void work() {												//运行线程
		while (!isBroken) {
			while (!this->requestList.empty() || !this->arriveList.empty()) {
				if (isBroken) break;
				elevatorLock.lock();
				move();
				elevatorLock.unlock();
				if (workState != WAIT) {
					stopAllActions();
					cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(1.5, cocos2d::Vec2(getPosition().x, floor * 45 + workState * 45 - 45));
					runAction(moveTo);
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));
					floor += workState;
				}
			}
			workState = WAIT;
		}
	}
	void move() {												//需求判断
		for (vector<Person*>::iterator i = requestList.begin(); i != requestList.end();)
		{
			if ((*i)->getStart() == floor) {
				if (doorState == CLOSE) {
					setDoorState(OPEN);
					clear();
					drawSolidRect(cocos2d::Vec2(190 + 120 * number, 0), cocos2d::Vec2(190 + 120 * number + 50, 45), cocos2d::Color4F::GREEN);
				}
				CC_SAFE_DELETE(*i);
				i = requestList.erase(i);
				if (requestList.size() == 0 && arriveList.size() == 0) workState = WAIT;
				else if (requestList.empty() && !arriveList.empty()) {
					if(workState * (arriveList.front()->getEnd() - floor) < 0) workState = -workState;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				continue;
			}
			i++;
		}
		isArrive();
	}
	void isArrive() {											//到达判断
		for (vector<Person*>::iterator i = arriveList.begin(); i != arriveList.end();)
		{
			if ((*i)->getEnd() == floor) {
				if (doorState == CLOSE) {
					setDoorState(OPEN);
					clear();
					drawSolidRect(cocos2d::Vec2(190 + 120 * number, 0), cocos2d::Vec2(190 + 120 * number + 50, 45), cocos2d::Color4F::GREEN);
				}
				CC_SAFE_DELETE(*i);
				i = arriveList.erase(i);
				if (arriveList.empty()) workState = WAIT;
				else if (workState * (arriveList.front()->getEnd() - floor) < 0) workState = -workState;
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				continue;
			}
			i++;
		}
		if (doorState == OPEN) {
			setDoorState(CLOSE);
			clear();
			drawSolidRect(cocos2d::Vec2(190 + 120 * number, 0), cocos2d::Vec2(190 + 120 * number + 50, 45), cocos2d::Color4F::BLACK);
		}
	}
};

