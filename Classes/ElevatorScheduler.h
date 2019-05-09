#pragma once
#include "Elevator.h"
#include <cmath>
#include <queue>

using namespace std;

class ElevatorScheduler;

struct message {
	int floor;
	int order;
	message(int f, int o) :floor(f), order(o) {};
};

class ElevatorScheduler
{
private:
	const int maxFloor = 20;
	mutex scheduleLock;
	queue<message> messageQueue;
public:
	static const int UP = 1;
	static const int DOWN = -1;

	Elevator *elevators[5];
	int orderElevator[21][2] = { 0 };

	ElevatorScheduler() {
		for (int i = 0; i < 5; i++) elevators[i] = Elevator::createElevator(i);
		std::thread t(&ElevatorScheduler::tryToSchedule, this);
		t.detach();
	}

	void callElevator(Person* person)
	{
		scheduleLock.lock();
		int startFloor = person->getStart();
		int order = person->getOrder();
		delete(person);
		messageQueue.push(message(startFloor, order));
		scheduleLock.unlock();
	}
	void tryToSchedule() {
		while (true) {
			while (messageQueue.empty()) std::this_thread::sleep_for(std::chrono::milliseconds(100));
			scheduleLock.lock();
			int startFloor = messageQueue.front().floor;
			int order = messageQueue.front().order;
			int elevatorNumber = -1;
			int minCost = INT_MAX;
			int finalState;
			for (int i = 0; i < 5; i++) elevators[i]->elevatorLock.lock();
			for (int i = 0; i < 5; i++) {
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
				if (cost < minCost) {
					minCost = cost;
					elevatorNumber = i;
					finalState = state;
				}
			}
			if (elevatorNumber == -1) {
				messageQueue.push(message(messageQueue.front().floor, messageQueue.front().order));
			}
			else {
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