
#include "WorldLevels.h"
#include "Applic.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::experimental;

WorldLevels::WorldLevels()
{
	_deltaX = 155;
	_deltaY = 215;
	CW = App::get()->getCurrentWorld();
}

WorldLevels::~WorldLevels()
{
	AudioEngine::pause(App::get()->getMenuBgrAudioID());
	if (_listener) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	}
}

Scene* WorldLevels::createScene(bool inGame)
{
	auto scene = Scene::create();
	auto layer = WorldLevels::create();
	scene->addChild(layer);


	return scene;
}

bool WorldLevels::init()
{
	//App::get()->setCurrentWorld(CW);
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

#ifdef _WIN32
	//install controler buttons
	App::get()->_player->clear_controller();
	App::get()->_player->setButtomFunction([]{ Director::getInstance()->end(); }, CXBOXController::BUTTON_BACK);
	App::get()->_player->setButtomFunction([this]
	{ if (App::get()->getCurrentMap() <= 0)
	App::get()->setCurrentMap(COUNT_LVLS_ON_1_WORLD - 1);
	else
		App::get()->decrementCurrnetMap();
	int neck = 0;
	if (App::get()->getCurrentMap() >= 10)
		neck = _deltaX;
	_arrow->setPosition(Vec2(215 + _deltaX * (App::get()->getCurrentMap() % 5) + neck, 680 - _deltaY * (App::get()->getCurrentMap() / 5))); },
		CXBOXController::BUTTON_LEFT);
	App::get()->_player->setButtomFunction([this]
	{if (App::get()->getCurrentMap() >= COUNT_LVLS_ON_1_WORLD - 1)
	App::get()->setCurrentMap(0);
	else
		App::get()->incrementCurrnetMap();
	int neck = 0;
	if (App::get()->getCurrentMap() >= 10)
		neck = _deltaX;
	_arrow->setPosition(Vec2(215 + _deltaX * (App::get()->getCurrentMap() % 5) + neck, 680 - _deltaY * (App::get()->getCurrentMap() / 5)));
	},
		CXBOXController::BUTTON_RIGHT);
	App::get()->_player->setButtomFunction([this]{ App::get()->startLevel(NULL); },
		CXBOXController::BUTTON_START);
	App::get()->_player->setButtomFunction([this]{ App::get()->startLevel(NULL); },
		CXBOXController::BUTTON_A);
	App::get()->_player->setButtomFunction([this]{ App::get()->gotoWorldMap(NULL); },
		CXBOXController::BUTTON_B);
#endif

	FileUtils::getInstance()->addSearchPath("menu_layer"); // work in all project 
	auto backBtn = MenuItemImage::create("back_btn.png", "back_psd.png", [](Ref*) {
		App::get()->gotoWorldMap(NULL);
	});
	auto wiew = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	backBtn->setScale(0.5);
	backBtn->setAnchorPoint(Vec2(0, 1));
	backBtn->setPosition(Vec2(150, 200 ));

	auto menu = Menu::create(backBtn, NULL);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 2);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("maps_list/fon_main_3.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);

	vector<string> pics;

	for (size_t i = 2; i <= 14; i++)
	{
		pics.push_back(std::to_string(i) + ".png");
	}


	const size_t width = 153;
	const size_t offsetRight = 100;
	const size_t dX = 15;


	for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
	{
		Sprite* temp;
		if (App::get()->getConfig()._complitedLvls[CW][i] == true)
		{
			temp = Sprite::create("maps_list/" + pics.at(i));
			Sprite* color = Sprite::create("maps_list/symb_" + std::to_string(CW) + ".png");

			color->setPosition(Vec2(temp->getContentSize().width / 2, 181));
			assert(color != NULL);
			temp->addChild(color, 1);
		}

		else if (App::get()->getConfig()._availableLvls[CW][i] == true)
			temp = Sprite::create("maps_list/card_available.png");
		else
			temp = Sprite::create("maps_list/card_non_available.png");

		_levels.push_back(temp);
	}



	auto deltaX = 0;
	auto deltaY = 0;

	for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
	{


		_levels[i]->setPosition(Vec2(220 + deltaX, 600 + deltaY));
		//_levels[i]->setVisible(false);
		addChild(_levels[i], 1);

		if ((i + 1) % 5 == 0)
		{
			deltaY -= _deltaY;
			deltaX = 0;
			if (i == 9)
				deltaX += _deltaX;
		}
		else
		{
			deltaX += _deltaX;

		}

	}


	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	_listener = listener;


	listener->setSwallowTouches(true);
	listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();

		for (auto i = 0; i < COUNT_LVLS_ON_1_WORLD; i++)
		{
			if (_levels[i]->getBoundingBox().containsPoint(p) && App::get()->canTouch())
			{
				if (App::get()->getConfig()._availableLvls[CW][i] == true)
				{
					App::get()->setCurrentMap(i);

					App::get()->startLevel(NULL);
					return true;
				}

			}
		}


		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);

#ifdef _WIN32
	_arrow = Sprite::create("down_arrow.png");
	_arrow->setPosition(Vec2(215 + _deltaX * (App::get()->getCurrentMap() % 5), 680 - _deltaY * (App::get()->getCurrentMap() / 5)));
	addChild(_arrow, 1);
	// Create the actions
	auto moveDown = MoveBy::create(.7, Point(0, -15));
	auto moveUp = MoveBy::create(.7, Point(0, 15));
	_arrow->runAction(RepeatForever::create(Sequence::create(moveDown, moveUp, NULL)));
#endif

	int ID = App::get()->getMenuBgrAudioID();
	if (ID != 0)
		if (AudioEngine::getState(ID) == AudioEngine::AudioState::PAUSED)
			AudioEngine::resume(ID);
		else
			CCLOG("houston we have a problem in MENU ");

	this->schedule(schedule_selector(WorldLevels::update));

	return true;
}


void WorldLevels::update(float dt)
{
	// if is running
	//CCLOG("check_1up");
#ifdef _WIN32
	App::get()->_player->check_controller();
#endif	

}