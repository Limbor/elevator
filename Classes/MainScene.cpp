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

void MainScene::drawBuilding()
{
	int x = 50;
	for (int i = 0; i < 20; i++) {
		floor[i] = DrawNode::create();
		floor[i]->retain();
		floorNumber[i] = Label::createWithSystemFont(String::createWithFormat("%i", i + 1)->getCString(), "Arial", 18);
		floorNumber[i]->retain();
		floorNumber[i]->setPosition(Vec2(10, i * 45 + 20));
		floorNumber[i]->setColor(Color3B::BLACK);
		floor[i]->drawSegment(Vec2(x, i * 45), Vec2(x + 700, i * 45), 1, Color4F::BLACK);
		floorButton[i][0] = ui::Button::create("up.png","up_sel.png","up_sel.png");
		floorButton[i][1] = ui::Button::create("down.png", "down_sel.png", "down_sel.png");
		floorButton[i][0]->setTag(i * 10);
		floorButton[i][1]->setTag(i * 10 + 1);
		floorButton[i][0]->setPosition(Vec2(floorButton[i][0]->getContentSize().width / 2 + 50,
			floorButton[i][0]->getContentSize().height / 2 + 25 + i * 45));
		floorButton[i][1]->setPosition(Vec2(floorButton[i][1]->getContentSize().width / 2 + 50,
			floorButton[i][1]->getContentSize().height / 2 + 5 + i * 45));
		floorButton[i][0]->addClickEventListener([&](Ref* sender) {
			ui::Button *m = (ui::Button *)sender;
			int flag = m->getTag();
			int order = flag % 10 == 1 ? -1 : 1;
			int floor = (flag / 10) + 1;
			m->setEnabled(false);
			Person *person = new Person(floor, order);
			es->callElevator(person);
		});
		floorButton[i][1]->addClickEventListener([&](Ref* sender) {
			ui::Button *m = (ui::Button *)sender;
			int flag = m->getTag();
			int order = flag % 10 == 1 ? -1 : 1;
			int floor = flag / 10 + 1;
			m->setEnabled(false);
			Person *person = new Person(floor, order);
			es->callElevator(person);
		});
		if(i != 19) this->addChild(floorButton[i][0]);
		else floorButton[i][0]->retain();
		if(i != 0) this->addChild(floorButton[i][1]);
		else floorButton[i][1]->retain();
		this->addChild(floorNumber[i]);
		this->addChild(floor[i]);
	}
}

void MainScene::drawElevatorState()
{
	for (int i = 0; i < 5; i++) {
		this->addChild(es->elevators[i]);
		es->elevators[i]->retain();
		for (int j = 0; j < 20; j++) {
			elevatorNumber[i][j] = Label::createWithSystemFont(String::createWithFormat("%i", 1)->getCString(), "Arial", 16);
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

void MainScene::drawElevator()
{
	for (int i = 0; i < 5; i++) {
		bg[i] = LayerColor::create(Color4B::BLACK, 200, 400);
		bg[i]->setPosition(Vec2(755 + i * 210, 250));
		addChild(bg[i]);
		curentfFloor[i] = Label::createWithSystemFont(String::createWithFormat("%i", 1)->getCString(), "Arial", 60);
		curentfFloor[i]->setColor(Color3B::RED);
		curentfFloor[i]->setPosition(Vec2(800 + i * 210, 530));
		addChild(curentfFloor[i]);
		for (int j = 0; j < 20 ; j++) {
			floors[i][j] = ui::Button::create(String::createWithFormat("elevator%i.png", j + 1)->getCString(),
				String::createWithFormat("light%i.png", j + 1)->getCString(),
				String::createWithFormat("light%i.png", j + 1)->getCString());
			int x = j / 10;
			int y = j % 10;
			floors[i][j]->setPosition(Vec2(875 + i * 210 + x * 35, 275 + y * 35));
			floors[i][j]->setTag((i + 1) * 100 + j + 1);
			floors[i][j]->addClickEventListener([&](Ref* sender) {
				ui::Button *b = (ui::Button*)sender;
				int tag = b->getTag();
				b->setEnabled(false);
				int floor = tag % 100;
				int number = tag / 100 - 1;
				es->elevators[number]->elevatorLock.lock();
				es->elevators[number]->addArriveList(new Person(floor));
				es->elevators[number]->elevatorLock.unlock();
			});
			addChild(floors[i][j]);
		}
	}
}

bool MainScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	auto backGround = LayerColor::create(Color4B::WHITE);
	this->addChild(backGround);
	auto elevatorBG = LayerColor::create(Color4B::GRAY, 1050, 900);
	elevatorBG->setPosition(Vec2(750, 0));
	this->addChild(elevatorBG);
	drawBuilding();
	drawElevatorState();
	drawElevator();
	scheduleUpdate();
    return true;
}

void MainScene::update(float dt)
{
	for (int i = 0; i < 5; i++) {
		int floor = es->elevators[i]->getFloor();
		int state = es->elevators[i]->getWorkState();
		int door = es->elevators[i]->getDoorState();

		if (es->orderElevator[floor][0] == i) {
			floorButton[floor - 1][0]->setEnabled(true);
			es->orderElevator[floor][0] = 0;
		}
		if (es->orderElevator[floor][1] == i) {
			floorButton[floor - 1][1]->setEnabled(true);
			es->orderElevator[floor][1] = 0;
		}
		curentfFloor[i]->setString(String::createWithFormat("%i", floor)->getCString());
		if (es->elevators[i]->arrive[floor] == 1) {
			floors[i][floor - 1]->setEnabled(true);
			es->elevators[i]->arrive[floor] = 0;
		}
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


