#pragma once
#include <cmath>
#include <iostream>
#include <time.h>
using namespace std;
class Person
{
private:
	int startFloor;									//起始楼层
	int endFloor;									//到达楼层
	int order;										//要求方向
public:
	static const int UP = 1;
	static const int DOWN = -1;
	Person(int start, int order) {					//上电梯的构造方式
		startFloor = start;
		this->order = order;
	}
	Person(int end) {								//下电梯的构造方式
		endFloor = end;
	}
	int getStart() { return startFloor; }
	int getEnd() { return endFloor; }
	int getOrder() { return order; }
};