#include "WorldMap.h"


#include "Applic.h"

#include "AudioEngine.h"
using namespace cocos2d::experimental;

USING_NS_CC;

#define BGMUS  AudioEngine::play2d(CCFileUtils::sharedFileUtils()->fullPathForFilename("cave.mp3").c_str(),true);

int audioID;

WorldMap::WorldMap()
{
	deltaX = 170;
	deltaY = 120;
}

WorldMap::~WorldMap()
{

	if (_listener) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	}
}

Scene* WorldMap::createScene(bool inGame)
{
	auto scene = Scene::create();
	auto layer = WorldMap::create();
	scene->addChild(layer);


	return scene;
}


bool WorldMap::init()
{

	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}
#ifdef _WIN32
	//install controler buttons
	App::get()->_player->clear_controller();
	App::get()->_player->setButtomFunction([]{ App::get()->gotoMenu(NULL); }, CXBOXController::BUTTON_BACK);
	App::get()->_player->setButtomFunction([]{ App::get()->gotoMenu(NULL); }, CXBOXController::BUTTON_B);
	
	App::get()->_player->setButtomFunction([this]
	
	{ if (App::get()->getCurrentWorld() <= 0)
	App::get()->setCurrentWorld(COUNT_WORLDS - 1);
	else
		App::get()->decrementCurrnetWorld();
	int neck = App::get()->getCurrentWorld() % 2 == 0 ? 0 : 1;
	_arrow->setPosition(Vec2(650 + deltaX*neck, 650 - deltaY*App::get()->getCurrentWorld())); },
		CXBOXController::BUTTON_LEFT);
	App::get()->_player->setButtomFunction([this]
	{if (App::get()->getCurrentWorld() >= COUNT_WORLDS - 1)
	App::get()->setCurrentWorld(0);
	else
		App::get()->incrementCurrnetWorld();
	int neck = App::get()->getCurrentWorld()%2 == 0 ? 0 : 1;
	_arrow->setPosition(Vec2(650 + deltaX*neck, 650 - deltaY*App::get()->getCurrentWorld())); },
		CXBOXController::BUTTON_RIGHT);
	App::get()->_player->setButtomFunction([this]{ openActiveWorld(App::get()->getCurrentWorld()); },
		CXBOXController::BUTTON_START);
	App::get()->_player->setButtomFunction([this]{ openActiveWorld(App::get()->getCurrentWorld()); },
		CXBOXController::BUTTON_A);
#endif


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("map/fon_main_2.png");
	assert(sprite != NULL);
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);


	//////////////////////////////

	auto world1 = MenuItemImage::create("map/overlay_1.png", "map/click_overlay_1.png",  [this](Ref* ref) { \
		openActiveWorld(0);
	}); 
	world1->setPosition(673, 577); 

	auto world2 = MenuItemImage::create("map/overlay_2.png", "map/click_overlay_2.png",  [this](Ref* ref) { \
		openActiveWorld(1);
	}); 
	world2->setPosition(817, 485);

	auto world3 = MenuItemImage::create("map/overlay_3.png", "map/click_overlay_3.png",  [this](Ref* ref) { \
		openActiveWorld(2);
	}); 
	world3->setPosition(676, 377);

	auto world4 = MenuItemImage::create("map/overlay_4.png", "map/click_overlay_4.png",  [this](Ref* ref) { \
		openActiveWorld(3);
	}); 
	world4->setPosition(809, 268);

	Menu* worldMenu  = Menu::create(world1, world2, world3, world4, nullptr);
    worldMenu->setPosition(Point::ZERO);
	addChild(worldMenu, 1);


	/// icons
	auto icon1 = Sprite::create("map/icon_1.png");
	icon1->setPosition(662, 569);
	addChild(icon1, 2);

	auto icon2 = Sprite::create("map/icon_2.png");
	icon2->setPosition(833, 472);
	addChild(icon2, 2);

	auto icon3 = Sprite::create("map/icon_3.png");
	icon3->setPosition(663, 374);
	addChild(icon3, 2);

	auto icon4 = Sprite::create("map/icon_4.png");
	icon4->setPosition(824, 251);
	addChild(icon4, 2);




	//auto city1 = Sprite::create("potlava.png");
	//city1->setPosition(Vec2(215 + deltaX * 0, 560 - deltaY * 0));
	//addChild(city1, 1);

	//auto city2 = Sprite::create("globuno.png");
	//city2->setPosition(Vec2(215 + deltaX * 1, 560 - deltaY * 1));
	//addChild(city2, 1);

	//auto city3 = Sprite::create("lusenko.png");
	//city3->setPosition(Vec2(215 + deltaX * 2, 560 - deltaY * 2));
	//addChild(city3, 1);

	//auto listener = cocos2d::EventListenerTouchOneByOne::create();
	//_listener = listener;
	//listener->setSwallowTouches(true);

	//listener->onTouchBegan = [city1, city2, city3, this](cocos2d::Touch* touch, cocos2d::Event* event)
	//{
	//	cocos2d::Vec2 p = touch->getLocation();
	//	if (city1->getBoundingBox().containsPoint(p) && App::get()->canTouch())
	//	{
	//		//App::get()->gotoWorldOne(NULL);
	//		openActiveWorld(0);
	//		return true;
	//	}
	//	if (city2->getBoundingBox().containsPoint(p) && App::get()->canTouch())
	//	{

	//		//App::get()->gotoWorldTwo(NULL);
	//		openActiveWorld(1);
	//		return true;
	//	}
	//	if (city3->getBoundingBox().containsPoint(p) && App::get()->canTouch())
	//	{
	//		//App::get()->gotoWorldThree(NULL);
	//		openActiveWorld(2);
	//		return true;
	//	}
	//	return false;
	//};


	//this->schedule(schedule_selector(WorldMap::update));

	//Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);

	this->schedule(schedule_selector(WorldMap::update));

#ifdef _WIN32
	 _arrow = Sprite::create("down_arrow.png");
	 int neck = App::get()->getCurrentWorld() % 2 == 0 ? 0 : 1;
	 _arrow->setPosition(Vec2(650 + deltaX*neck, 650 - deltaY*App::get()->getCurrentWorld())); 
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
	else
		{
			int ID = BGMUS;
			App::get()->setMenuBgrAudioID(ID);
		}
		

	return true;
}

void WorldMap::openActiveWorld(int number)
{

	//AudioEngine::stop(audioID);
	App::get()->setCurrentWorld(number);
	App::get()->gotoWorldLevels(NULL);
	/*switch (number)
	{
	case 0:
		App::get()->gotoWorldOne(NULL);
		break;
	case 1:
		App::get()->gotoWorldTwo(NULL);
		break;
	case 2:
		App::get()->gotoWorldThree(NULL);
		break;
	default:
		CCLOG("This world is not exitst");
		break;
	}*/
	return;
};


void WorldMap::update(float dt)
{
	// if is running
	//CCLOG("check_1up");
#ifdef _WIN32
	App::get()->_player->check_controller();
#endif	
	
}



