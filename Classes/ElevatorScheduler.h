#pragma once
#include "Elevator.h"
#include <cmath>
#include <queue>

using namespace std;

class ElevatorScheduler;
//void schedule(ElevatorScheduler* es, int elevatorNumber);

struct message {
	int floor;
	int order;
	message(int f, int o) :floor(f), order(o) {};
};

class ElevatorScheduler
{
private:
	const int maxFloor = 20;
	//int orderFloor[5][21] = { 0 };
	mutex scheduleLock;
	//Floor* floors = new Floor[21];
	queue<message> messageQueue;
	//priority_queue<Person>* takingPerson = new  priority_queue<Person>[5];
public:
	static const int UP = 1;
	static const int DOWN = -1;
	//friend void schedule(ElevatorScheduler* es, int elevatorNumber);
	Elevator *elevators[5];

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
		messageQueue.push(message(startFloor, order));
		scheduleLock.unlock();
	}
	/*int getDestination(int elevatorNumber) {
		int state = elevators[elevatorNumber].getWorkState();
		if (state == Elevator::UP) {
			for (int i = 20; i > elevators[elevatorNumber].getFloor(); i--) {
				if (orderFloor[elevatorNumber][i]) return i;
			}
		}
		else {
			for (int i = 1; i < elevators[elevatorNumber].getFloor(); i++) {
				if (orderFloor[elevatorNumber][i]) return i;
			}
		}
		return 0;
	}*/

	//在人进电梯的过程中讲UI中读取的目的楼层写入这个人之中
	void orderElevator(int elevatorNumber, Person person) {
	}
	/*bool hasTask(int elevatorNumber, int state) {
		if (state == Elevator::UP) {
			for (int i = elevators[elevatorNumber].getFloor(); i < 21; i++) {
				if (orderFloor[elevatorNumber][i]) return true;
			}
			return false;
		}
		else {
			for (int i = elevators[elevatorNumber].getFloor(); i > 0; i--) {
				if (orderFloor[elevatorNumber][i]) return true;
			}
			return false;
		}
	}*/
	void tryToSchedule() {
		while (true) {
			while (messageQueue.empty()) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			scheduleLock.lock();
			int startFloor = messageQueue.front().floor;
			int order = messageQueue.front().order;

			int elevatorNumber = -1;
			int minCost = INT_MAX;
			int finalState;
			for (int i = 0; i < 5; i++) elevators[i]->elevatorLock.lock();
			for (int i = 0; i < 5; i++) {
				//if (elevators[i].getLoadState() != Elevator::LOAD) continue;
				int dis = startFloor - elevators[i]->getFloor();
				//if (dis == 0 && elevators[i]->getWorkState() != Elevator::WAIT) continue;
				int cost = abs(dis);
				int state = dis == 0 ? order : dis / cost;
				if (elevators[i]->getWorkState() == Elevator::WAIT && dis != 0) cost += 10;
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
				message m = messageQueue.front();
				messageQueue.push(m);
			}
			else {
				elevators[elevatorNumber]->addRequest(new Person(startFloor, order));
				if (elevators[elevatorNumber]->getWorkState() == Elevator::WAIT) {
					elevators[elevatorNumber]->setWorkState(finalState);
				}
			}
			for (int i = 0; i < 5; i++) elevators[i]->elevatorLock.unlock();
			messageQueue.pop();
			scheduleLock.unlock();
		}
	}
};

/*void schedule(ElevatorScheduler* es, int elevatorNumber) {
	int state = es->elevators[elevatorNumber].getWorkState();
	while(true) {
		es->elevators[elevatorNumber].work();
		if (int temp = es->orderFloor[elevatorNumber][es->elevators[elevatorNumber].getFloor()]) {
			es->elevators[elevatorNumber].setDoorState(Elevator::OPEN);
			if (temp == ElevatorScheduler::CALL || temp == ElevatorScheduler::BOTH) {
				int count = es->from_to.count(es->elevators[elevatorNumber].getFloor());
				es->elevators[elevatorNumber].enter(1);
				es->iter = es->from_to.find(es->elevators[elevatorNumber].getFloor());
				for (int i = 0; i < count; i++, es->iter++)
					es->orderElevator(elevatorNumber, es->iter->second);
			}
			else if(temp == ElevatorScheduler::GO || temp == ElevatorScheduler::BOTH)
				es->elevators[elevatorNumber].exit(1);
			es->orderFloor[elevatorNumber][es->elevators[elevatorNumber].getFloor()] = 0;
			if (!es->hasTask(elevatorNumber, state)) {
				if (es->hasTask(elevatorNumber, -state))
					es->elevators[elevatorNumber].setWorkState(-state);
				else {
					es->elevators[elevatorNumber].setWorkState(Elevator::WAIT);
					return;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			es->elevators[elevatorNumber].setDoorState(Elevator::CLOSE);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	while (true) {
		int state = es->elevators[elevatorNumber].getWorkState();
		int floor = es->elevators[elevatorNumber].getFloor();
		int orderState = es->orderFloor[elevatorNumber][floor];
		if (abs(es->orderFloor[elevatorNumber][floor]) == state) {
			es->elevators[elevatorNumber].setDoorState(Elevator::OPEN);
			while (!es->floors[floor].isEmpty(floor, orderState)) {
				es->takingPerson[elevatorNumber].push(es->floors[floor].quitPerson(floor, orderState));
				es->elevators[elevatorNumber].enter(1);
				es->orderElevator(elevatorNumber, es->floors[floor].quitPerson(floor, orderState));
				es->floors[floor].deletePerson(floor, orderState);
				if (es->elevators[elevatorNumber].getLoadState() == Elevator::FULLLOAD) break;
			}
		}
		Person p = es->takingPerson[elevatorNumber].top();
		while (p.getEnd() == floor) {
			es->elevators[elevatorNumber].setDoorState(Elevator::OPEN);
			es->elevators[elevatorNumber].exit(1);
			es->takingPerson[elevatorNumber].pop();
			if (!es->takingPerson[elevatorNumber].empty()) {
				p = es->takingPerson[elevatorNumber].top();
			}
		}
		if (es->takingPerson[elevatorNumber].empty()) {
			if (es->hasTask(elevatorNumber, state));
			else if (es->hasTask(elevatorNumber, -state)) es->elevators[elevatorNumber].setWorkState(-state);
			else es->elevators[elevatorNumber].setWorkState(Elevator::WAIT);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		es->elevators[elevatorNumber].setDoorState(Elevator::CLOSE);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	es->elevators[elevatorNumber].work();
}*/