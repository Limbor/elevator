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
		//楼层线
		floor[i] = DrawNode::create();
		floor[i]->retain();
		floor[i]->drawSegment(Vec2(x, i * 45), Vec2(x + 700, i * 45), 1, Color4F::BLACK);
		//楼层号
		floorNumber[i] = Label::createWithSystemFont(String::createWithFormat("%i", i + 1)->getCString(), "Arial", 18);
		floorNumber[i]->retain();
		floorNumber[i]->setPosition(Vec2(10, i * 45 + 20));
		floorNumber[i]->setColor(Color3B::BLACK);
		//外部呼叫按钮
		floorButton[i][0] = ui::Button::create("up.png","up_sel.png","up_sel.png");
		floorButton[i][1] = ui::Button::create("down.png", "down_sel.png", "down_sel.png");
		floorButton[i][0]->setTag(i * 10);
		floorButton[i][1]->setTag(i * 10 + 1);
		floorButton[i][0]->setPosition(Vec2(floorButton[i][0]->getContentSize().width / 2 + 50,
			floorButton[i][0]->getContentSize().height / 2 + 25 + i * 45));
		floorButton[i][1]->setPosition(Vec2(floorButton[i][1]->getContentSize().width / 2 + 50,
			floorButton[i][1]->getContentSize().height / 2 + 5 + i * 45));
		//呼叫按钮回调函数
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
		//电梯显示
		this->addChild(es->elevators[i]);
		es->elevators[i]->retain();
		for (int j = 0; j < 20; j++) {
			//电梯所在楼层
			elevatorNumber[i][j] = Label::createWithSystemFont(String::createWithFormat("%i", 1)->getCString(), "Arial", 16);
			elevatorNumber[i][j]->setPosition(Vec2(150 + i * 120, 20 + 45 * j));
			elevatorNumber[i][j]->setColor(Color3B::BLACK);
			//电梯向上运行时显示
			elevatorUP[i][j] = Sprite::create();
			elevatorUP[i][j]->initWithFile("up_sel.png");
			elevatorUP[i][j]->setPosition(Vec2(170 + i * 120, 20 + 45 * j));
			elevatorUP[i][j]->setVisible(false);
			//电梯向下运行时显示
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
		//添加背景板
		bg[i] = LayerColor::create(Color4B::BLACK, 200, 400);
		bg[i]->setPosition(Vec2(755 + i * 210, 250));
		addChild(bg[i]);
		//楼层显示器
		curentfFloor[i] = Label::createWithSystemFont(String::createWithFormat("%i", 1)->getCString(), "Arial", 60);
		curentfFloor[i]->setColor(Color3B::RED);
		curentfFloor[i]->setPosition(Vec2(800 + i * 210, 530));
		addChild(curentfFloor[i]);
		//运行状态显示器
		for (int j = 0; j < 2; j++) {
			elevatorState[i][j] = DrawNode::create();
			if (j == 0) elevatorState[i][j]->drawTriangle(Vec2(780 + i * 210, 460), Vec2(820 + i * 210, 460),
				Vec2(800 + i * 210, 490), Color4F::RED);
			else  elevatorState[i][j]->drawTriangle(Vec2(780 + i * 210, 440), Vec2(820 + i * 210, 440),
				Vec2(800 + i * 210, 410), Color4F::RED);
			elevatorState[i][j]->setVisible(false);
			this->addChild(elevatorState[i][j]);
		}
		//添加报警按钮
		warning[i] = ui::Button::create("Warning.png");
		warning[i]->setPosition(Vec2(840 + i * 210, 275));
		warning[i]->setTag(50 + i);
		warning[i]->addClickEventListener([&](Ref* sender) {
			ui::Button *b = (ui::Button*)sender;
			int tag = b->getTag();
			es->elevators[tag - 50]->hasBroken();
			b->setEnabled(false);
		});
		addChild(warning[i]);
		//添加开关门按钮
		for (int j = 0; j < 2; j++) {
			door[i][j] = ui::Button::create(String::createWithFormat("door%i.png", j + 1)->getCString());
			door[i][j]->setPosition(Vec2(780 + i * 210 + j * 30, 275));
			this->addChild(door[i][j]);
		}
		//添加楼层按钮
		for (int j = 0; j < 20 ; j++) {
			floors[i][j] = ui::Button::create(String::createWithFormat("elevator%i.png", j + 1)->getCString(),
				String::createWithFormat("light%i.png", j + 1)->getCString(),
				String::createWithFormat("light%i.png", j + 1)->getCString());
			int x = j / 10;
			int y = j % 10;
			floors[i][j]->setPosition(Vec2(875 + i * 210 + x * 35, 275 + y * 35));
			floors[i][j]->setTag((i + 1) * 100 + j + 1);
			//楼层按钮回调函数
			floors[i][j]->addClickEventListener([&](Ref* sender) {
				ui::Button *b = (ui::Button*)sender;
				int tag = b->getTag();
				int floor = tag % 100;
				int number = tag / 100 - 1;
				if (!es->elevators[number]->getIsBroken()) {
					b->setEnabled(false);
					es->elevators[number]->elevatorLock.lock();
					es->elevators[number]->addArriveList(new Person(floor));
					es->elevators[number]->elevatorLock.unlock();
				}
			});
			addChild(floors[i][j]);
		}
	}
}

bool MainScene::init()
{
    if (!Scene::init() )
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
		//将已完成的楼层呼叫按钮关闭
		if (es->orderElevator[floor][0] == i + 1) {
			floorButton[floor - 1][0]->setEnabled(true);
			es->orderElevator[floor][0] = 0;
		}
		if (es->orderElevator[floor][1] == i + 1) {
			floorButton[floor - 1][1]->setEnabled(true);
			es->orderElevator[floor][1] = 0;
		}
		curentfFloor[i]->setString(String::createWithFormat("%i", floor)->getCString());
		//将已完成电梯内部按钮熄灭
		if (es->elevators[i]->arrive[floor] == 1) {
			floors[i][floor - 1]->setEnabled(true);
			es->elevators[i]->arrive[floor] = 0;
		}
		//电梯内部状态显示
		if (es->elevators[i]->getWorkState() == Elevator::WAIT) {
			elevatorState[i][0]->setVisible(false);
			elevatorState[i][1]->setVisible(false);
		}
		else {
			if (state == Elevator::UP) elevatorState[i][0]->setVisible(true);
			else elevatorState[i][1]->setVisible(true);
		}
		//电梯外部状态显示
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
		//将损坏的电梯的任务重新调度
		if (es->elevators[i]->getIsBroken()) {
			for (int j = 0; j < 20; j++) {
				if (es->orderElevator[j + 1][0] == i + 1) {
					es->callElevator(new Person(j + 1, 1));
					es->orderElevator[j + 1][0] = 0;
				}
				else if (es->orderElevator[j + 1][1] == i + 1) {
					es->callElevator(new Person(j + 1, -1));
					es->orderElevator[j + 1][1] = 0;
				}
			}
		}
	}
}


