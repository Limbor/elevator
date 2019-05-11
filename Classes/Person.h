#pragma once
#include <cmath>
#include <iostream>
#include <time.h>
using namespace std;
class Person
{
private:
	int startFloor;									//��ʼ¥��
	int endFloor;									//����¥��
	int order;										//Ҫ����
public:
	static const int UP = 1;
	static const int DOWN = -1;
	Person(int start, int order) {					//�ϵ��ݵĹ��췽ʽ
		startFloor = start;
		this->order = order;
	}
	Person(int end) {								//�µ��ݵĹ��췽ʽ
		endFloor = end;
	}
	int getStart() { return startFloor; }
	int getEnd() { return endFloor; }
	int getOrder() { return order; }
};