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
		DrawNode *floor = DrawNode::create();
		Label *floorNumber = Label::create(String::createWithFormat("%i", i + 1)->getCString(), "Arial", 18);
		floorNumber->setPosition(Vec2(10, i * 45 + 20));
		floorNumber->setColor(Color3B::BLACK);
		floor->drawSegment(Vec2(x, i * 45), Vec2(x + 800, i * 45), 1, Color4F::BLACK);
		floorButton[i][0] = ui::Button::create("up.png","up_sel.png","up_sel.png");
		floorButton[i][1] = ui::Button::create("down.png", "down_sel.png", "down_sel.png");
		floorButton[i][0]->setTag(i * 10);
		floorButton[i][1]->setTag(i * 10 + 1);
		floorButton[i][0]->setPosition(Vec2(floorButton[i][0]->getContentSize().width / 2 + 50,
			floorButton[i][0]->getContentSize().height / 2 + 25 + i * 45));
		floorButton[i][1]->setPosition(Vec2(floorButton[i][1]->getContentSize().width / 2 + 50,
			floorButton[i][1]->getContentSize().height / 2 + 5 + i * 45));
		floorButton[i][0]->addClickEventListener([&](Object* sender) {
			ui::Button *m = (ui::Button *)sender;
			int flag = m->getTag();
			int order = flag % 10 == 1 ? -1 : 1;
			int floor = (flag / 10) + 1;
			m->setEnabled(false);
			Person *person = new Person(floor, order);
			es->callElevator(person);
		});
		floorButton[i][1]->addClickEventListener([&](Object* sender) {
			ui::Button *m = (ui::Button *)sender;
			int flag = m->getTag();
			int order = flag % 10 == 1 ? -1 : 1;
			int floor = flag / 10 + 1;
			m->setEnabled(false);
			Person *person = new Person(floor, order);
			es->callElevator(person);
		});
		if(i != 19) this->addChild(floorButton[i][0]);
		if(i != 0) this->addChild(floorButton[i][1]);
		this->addChild(floorNumber);
		this->addChild(floor);
	}
}

void MainScene::drawElevatorState()
{
	for (int i = 0; i < 5; i++) {
		/*elevator[i] = DrawNode::create();
		elevator[i]->drawSolidRect(cocos2d::Vec2(180 + 120 * i, 0), cocos2d::Vec2(230 + 120 * i, 45), cocos2d::Color4F::BLACK);*/
		this->addChild(es->elevators[i]);
		for (int j = 0; j < 20; j++) {
			elevatorNumber[i][j] = Label::create(String::createWithFormat("%i", 1)->getCString(), "Arial", 16);
			elevatorNumber[i][j]->setPosition(Vec2(150 + i * 120, 20 + 45 * j));
			elevatorNumber[i][j]->setColor(Color3B::BLACK);

			elevatorUP[i][j] = Sprite::create();
			elevatorUP[i][j]->initWithFile("up_sel.png");
			elevatorUP[i][j]->setPosition(Vec2(170 + i * 120, 20 + 45 * j));
			elevatorUP[i][j]->setVisible(false);

			elevatorDOWN[i][j] = Sprite::create();
			elevatorDOWN[i][j]->initWithFile("down_sel.png");
			elevatorDOWN[i][j]->setPosition(Vec2(170 + i * 120, 20 + 45 * j));
			elevatorDOWN[i][j]->setVisible(false);

			this->addChild(elevatorUP[i][j]);
			this->addChild(elevatorDOWN[i][j]);
			this->addChild(elevatorNumber[i][j]);
		}
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
	drawElevatorState();
	scheduleUpdate();
    return true;
}

void MainScene::update(float dt)
{
	for (int i = 0; i < 5; i++) {
		int floor = es->elevators[i]->getFloor();
		int state = es->elevators[i]->getWorkState();
		/*if (state != 0) {
			auto moveTo = MoveTo::create(2, Vec2(0, 45 * state));
			es->elevators[i]->runAction(moveTo);
		}*/
		for (int j = 0; j < 20; j++) {
			elevatorNumber[i][j]->setString(String::createWithFormat("%i", floor)->getCString());
			if (es->elevators[i]->getWorkState() == Elevator::WAIT) {
				elevatorDOWN[i][j]->setVisible(false);
				elevatorUP[i][j]->setVisible(false);
			}
			else {
				if (state == Elevator::UP) elevatorUP[i][j]->setVisible(true);
				else elevatorDOWN[i][j]->setVisible(true);
			}
		}
	}
}


