#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ElevatorScheduler.h"

USING_NS_CC;


class MainScene : public cocos2d::Scene
{
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	DrawNode *floor[20];								//¥����
	Label *floorNumber[20];								//¥���
	ui::Button *floorButton[20][2];						//ÿ¥�����µ��ݺ��а�ť

	Label *elevatorNumber[5][20];						//ÿ��¥����¥�����ʾ��
	Sprite *elevatorUP[5][20];							//ÿ��¥������״̬��ʾ��
	Sprite *elevatorDOWN[5][20];						//ÿ��¥������״̬��ʾ��

	LayerColor *bg[5];									//���ݱ�����
	ui::Button *floors[5][20];							//������¥�㰴ť
	ui::Button *warning[5];								//���ݱ�����ť
	ui::Button *door[5][2];								//�����Ű�ť
	DrawNode *elevatorState[5][2];						//��������״̬��ʾ��
	Label *curentfFloor[5];								//������¥����ʾ��

	ElevatorScheduler *es = new ElevatorScheduler();	//���ݵ�����
public:
    static cocos2d::Scene* createScene();
	void drawBuilding();								//��ʼ����¥
	void drawElevatorState();							//��ʼ��¥�������ʾ��
	void drawElevator();								//��ʼ�������ڲ���ʾ
    virtual bool init();								//��������ʼ��
	virtual void update(float dt);						//ÿ֡ˢ�µ������ⲿ¥��״̬��ʾ
	CREATE_FUNC(MainScene);
};

#endif // __HELLOWORLD_SCENE_H__
