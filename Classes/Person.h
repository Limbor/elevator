#pragma once
#include <cmath>
#include <iostream>
#include <time.h>
using namespace std;
class Person
{
private:
	int number;
	int startFloor;
	int endFloor;
	int order;
public:
	static const int UP = 1;
	static const int DOWN = -1;
	Person(int start, int order) {
		startFloor = start;
		this->order = order;
	}
	void setEnd() {
		do {
			srand((unsigned)time(NULL));
			endFloor = rand() % 20 + 1;
		} while ((endFloor - startFloor)*order < 0);
		cout << "To:" << endFloor << endl;
	}
	int getNumber() { return number; }
	int getStart() { return startFloor; }
	int getEnd() { return endFloor; }
	int getOrder() { return order; }
};