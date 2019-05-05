#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class MainScene : public cocos2d::Scene
{
private:
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	ui::Button *floorButton[20][2];
public:
    static cocos2d::Scene* createScene();
	void drawBuilding();
    virtual bool init();
	void call(Object* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // __HELLOWORLD_SCENE_H__
