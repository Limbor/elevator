#pragma once
#include "Elevator.h"
#include <cmath>
#include <queue>

using namespace std;

class ElevatorScheduler;

//需求信息结构体
struct message {
	int floor;
	int order;
	message(int f, int o) :floor(f), order(o) {};
};

class ElevatorScheduler
{
private:
	mutex scheduleLock;									//调度器锁
	queue<message> messageQueue;						//消息队列
public:
	static const int UP = 1;
	static const int DOWN = -1;

	Elevator *elevators[5];								//五部电梯
	int orderElevator[21][2] = { 0 };					//记录每次呼叫由哪部电梯接受

	ElevatorScheduler() {								//创建电梯，开启调度线程
		for (int i = 0; i < 5; i++) elevators[i] = Elevator::createElevator(i);
		std::thread t(&ElevatorScheduler::tryToSchedule, this);
		t.detach();
	}

	void callElevator(Person* person)					//呼叫电梯
	{
		scheduleLock.lock();
		int startFloor = person->getStart();
		int order = person->getOrder();
		delete(person);
		messageQueue.push(message(startFloor, order));	//加入消息队列
		scheduleLock.unlock();
	}
	void tryToSchedule() {								//调度线程
		while (true) {									//消息队列为空时等待
			while (messageQueue.empty()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
			scheduleLock.lock();
			int startFloor = messageQueue.front().floor;
			int order = messageQueue.front().order;
			int elevatorNumber = -1;
			int minCost = INT_MAX;
			int finalState;
			for (int i = 0; i < 5; i++) elevators[i]->elevatorLock.lock();
			for (int i = 0; i < 5; i++) {				//调度主算法
				if (elevators[i]->getIsBroken()) continue;
				int dis = startFloor - elevators[i]->getFloor();
				int cost = abs(dis);
				int state = (dis == 0) ? order : dis / cost;
				if (elevators[i]->getWorkState() == Elevator::WAIT && dis != 0) cost += 10;
				else if (elevators[i]->getWorkState() == Elevator::WAIT && dis == 0) cost = 0;
				else {
					if (order != elevators[i]->getWorkState()) continue;
					if (!elevators[i]->arriveList.empty()) {
						if (state != order) {
							continue;
						}
					}
					else continue;
				}
				if (cost < minCost) {					//寻找最佳电梯
					minCost = cost;
					elevatorNumber = i;
					finalState = state;
				}
			}
			if (elevatorNumber == -1) {					//无电梯接受则将此需求插入消息队列最后
				messageQueue.push(message(messageQueue.front().floor, messageQueue.front().order));
			}
			else {										//将需求插入目标电梯的需求队列中
				elevators[elevatorNumber]->addRequest(new Person(startFloor, order));
				if (elevators[elevatorNumber]->getWorkState() == Elevator::WAIT) {
					elevators[elevatorNumber]->setWorkState(finalState);
				}
				if (order == UP) orderElevator[startFloor][0] = elevatorNumber;
				else orderElevator[startFloor][1] = elevatorNumber;
			}
			for (int i = 0; i < 5; i++) elevators[i]->elevatorLock.unlock();
			messageQueue.pop();
			scheduleLock.unlock();
		}
	}
};