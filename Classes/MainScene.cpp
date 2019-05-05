#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance

void MainScene::drawBuilding()
{
	int x = 50;
	for (int i = 0; i < 20; i++) {
		DrawNode* floor = DrawNode::create();
		Label* floorNumber = Label::create(String::createWithFormat("%i", i + 1)->getCString(), "Arial", 18);
		floorNumber->setPosition(Vec2(10, i * 45 + 20));
		floorNumber->setColor(Color3B::BLACK);
		floor->drawSegment(Vec2(x, i * 45), Vec2(x + 800, i * 45), 1, Color4F::BLACK);
		floorButton[i][0] = ui::Button::create("up.png","up_sel.png","up_sel.png");
		floorButton[i][1] = ui::Button::create("down.png", "down_sel.png", "down_sel.png");
		floorButton[i][0]->setTag(i * 10);
		floorButton[i][1]->setTag(i * 10 + 1);
		floorButton[i][0]->setPosition(Vec2(floorButton[i][0]->getContentSize().width / 2 + 50, floorButton[i][0]->getContentSize().height / 2 + 25 + i * 45));
		floorButton[i][1]->setPosition(Vec2(floorButton[i][1]->getContentSize().width / 2 + 50, floorButton[i][1]->getContentSize().height / 2 + 5 + i * 45));
		floorButton[i][0]->addClickEventListener([&](Object* sender) {
			ui::Button *m = (ui::Button *)sender;
			int flag = m->getTag();
			int order = flag % 10 == 1 ? -1 : 1;
			int floor = flag / 10;
			m->setEnabled(false);
		});
		floorButton[i][1]->addClickEventListener([&](Object* sender) {
			ui::Button *m = (ui::Button *)sender;
			int flag = m->getTag();
			int order = flag % 10 == 1 ? -1 : 1;
			int floor = flag / 10;
			m->setEnabled(false);
		});
		this->addChild(floorButton[i][0]);
		this->addChild(floorButton[i][1]);
		this->addChild(floorNumber);
		this->addChild(floor);
	}
}

bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	auto backGround = LayerColor::create(ccc4(255, 255, 255, 255));
	backGround->setVisible(true);
	this->addChild(backGround);
	drawBuilding();
    return true;
}

void MainScene::call(Object * pSender)
{
	ui::Button *m = (ui::Button *)pSender;
	int flag = m->getTag();
	int order = flag % 10 == 1 ? -1 : 1;
	int floor = flag / 10;
}


