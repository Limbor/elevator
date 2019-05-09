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
	int floor;
	int workState;
	int doorState;
	int number = 0;
public:
	static const int UP = 1;
	static const int DOWN = -1;
	static const int WAIT = 0;
	static const int OPEN = 4;
	static const int CLOSE = 5;

	int arrive[21] = { 0 };
	vector<Person*> requestList;
	vector<Person*> arriveList;
	mutex elevatorLock;

	static Elevator* createElevator(int number) {
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
		doorState=CLOSE;
		this->number = number;
		cocos2d::DrawNode::create();
		cocos2d::DrawNode::drawSolidRect(cocos2d::Vec2(190 + 120 * number, 0), cocos2d::Vec2(240 + 120 * number, 45), cocos2d::Color4F::BLACK);
		std::thread t(&Elevator::work, this);
		t.detach();
	}
	int getFloor() { return floor; }
	int getWorkState() { return workState; }
	void setWorkState(int state) { workState = state; }
	void setDoorState(int state) { doorState = state; }
	int getDoorState() { return doorState; }
	void addRequest(Person* person) {
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
	void addArriveList(Person* person) {
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
	void work() {
		while (true) {
			while (!this->requestList.empty() || !this->arriveList.empty()) {
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
	void move() {
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
	void isArrive() {
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

