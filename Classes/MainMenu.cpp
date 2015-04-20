#include "MainMenu.h"
#include "Applic.h"

#include "SimpleAudioEngine.h"
#include "AudioEngine.h"

using namespace CocosDenshion;
//using namespace cocos2d::ui;
using namespace cocos2d::experimental;
USING_NS_CC;

MainMenu::MainMenu():
	_listener(nullptr),
	_tapTogo(nullptr)
{
}

MainMenu::~MainMenu()
{
	//CCLOG("ASSER:1");
	if (_listener) {
		Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
	}
	//CCLOG("ASSER:2");
}

Scene* MainMenu::createScene()
{
	auto scene = Scene::create();
	auto layer = MainMenu::create();
	scene->addChild(layer);

	return scene;
}

#define PLAY_THUNDER //SimpleAudioEngine::getInstance()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("thunder_2.mp3").c_str());
#define BGMUS //SimpleAudioEngine::getInstance()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename("rain.mp3").c_str(),true);

#define PLAY_THUNDER AudioEngine::play2d(CCFileUtils::sharedFileUtils()->fullPathForFilename("thunder_2.mp3").c_str());
#define BGMUS  AudioEngine::play2d(CCFileUtils::sharedFileUtils()->fullPathForFilename("rain.mp3").c_str(),true);

void MainMenu::playLights()
{
	int dt = rand() % 5 + 10;

	CCLOG("dt: %d", dt);
	
	auto seq1 = Sequence::create(DelayTime::create(dt), CallFunc::create([this] {
		//AudioManager::get()->playSound("ravens.mp3");
		//SimpleAudioEngine::getInstance()->playEffect("ravens.mp3");

		light1->setOpacity(255);
		auto fadeTo		= FadeTo::create(0.5, 0);
		PLAY_THUNDER;
		light1->runAction(Sequence::create(fadeTo, CallFunc::create([this] {
			
			light2->setOpacity(255);
			auto fadeTo2	= FadeTo::create(1, 0);
			light2->runAction(Sequence::create(fadeTo2, nullptr));
		}), nullptr));

		playLights();
	}), nullptr);
	this->runAction(seq1);
}


bool MainMenu::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

#ifdef _WIN32
	//install controler buttons
	App::get()->_player->clear_controller();
	App::get()->_player->setButtomFunction([]{ Director::getInstance()->end(); }, CXBOXController::BUTTON_BACK);
	App::get()->_player->setButtomFunction([this]{ App::get()->gotoWorldMap(NULL); },
		CXBOXController::BUTTON_START);
	App::get()->_player->setButtomFunction([this]{App::get()->gotoWorldMap(NULL); },
		CXBOXController::BUTTON_A);
	
#endif
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto sprite = Sprite::create("main/alice_main.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	addChild(sprite, 0);

	auto alice_overlay = Sprite::create("main/alice_overlay.png");
	alice_overlay->setPosition(Vec2(512, 464));
	addChild(alice_overlay, 1);

	auto fadeTo		= FadeTo::create(1, 100);
	auto fadeFrom	= FadeTo::create(0.9f, 255);
	alice_overlay->runAction( RepeatForever::create(Sequence::create(fadeTo, fadeFrom, NULL)) );

	auto glass_layer = Sprite::create("main/glass_layer.png");
	glass_layer->setPosition(Vec2(511, 448));
	glass_layer->setOpacity(255*0.7);
	addChild(glass_layer, 2);

	light1 = Sprite::create("main/lightning_overlay_1.png");
	light1->setPosition(513, 463);
	addChild(light1, 2);
	light1->setOpacity(0);
	playLights();

	light2 = Sprite::create("main/lightning_overlay_2.png");
	light2->setPosition(513, 463);
	addChild(light2, 2);
	light2->setOpacity(0);


	auto tapLayer = Sprite::create("main/tap_layer.png");
	tapLayer->setPosition(491, 399);
	addChild(tapLayer, 5);

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	_listener = listener;

	listener->setSwallowTouches(true);
	listener->onTouchBegan = [tapLayer](cocos2d::Touch* touch, cocos2d::Event* event)
	{
		cocos2d::Vec2 p = touch->getLocation();
		
		if (tapLayer->getBoundingBox().containsPoint(p)) {
			App::get()->gotoWorldMap(nullptr);
			return true;
		}

		return false;
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 30);
	/////////////////////////////
	auto clipper = ClippingNode::create();
    clipper->setContentSize(  Size(227, 363) );
	clipper->setAnchorPoint(Vec2(0.5, 0.5));
    clipper->setPosition( Vec2(514, 464) );
    addChild(clipper, 1);

    auto stencil = DrawNode::create();
    Vec2 figure[8];
	figure[0] = Vec2(46, 12);
	figure[1] = Vec2(14, 45);
	figure[2] = Vec2(14, 310);
	figure[3] = Vec2(60, 354);
	figure[4] = Vec2(170, 354);
	figure[5] = Vec2(217, 310);
	figure[6] = Vec2(217, 45);
	figure[7] = Vec2(170, 12);

    /*Color4F white(1, 1, 1, 1);
	stencil->drawPolygon(figure, 8, white, 1, white);
    clipper->setStencil(stencil);

	auto emitter = ParticleSystemQuad::create("main/RainParticle.plist");
	emitter->setScale(1.5);
	emitter->setPosition( Vec2(clipper->getContentSize().width/2 -50, clipper->getContentSize().height + 10) );
	clipper->addChild(emitter);*/

	/////////////////////////////////////

	auto moon_overlay = Sprite::create("main/moon_overlay.png");
	moon_overlay->setPosition(564, 630);
	addChild(moon_overlay, 1);
	moon_overlay->runAction( RepeatForever::create(Sequence::create(FadeTo::create(1.5f, 0), FadeTo::create(1.3f, 255), NULL)) );

	auto sky1 = Sprite::create("main/sky_overlay_1.png");
	sky1->setPosition(511, 555);
	addChild(sky1, 1);
	sky1->runAction( RepeatForever::create(Sequence::create(FadeTo::create(1.5f, 0), FadeTo::create(1.3f, 255), NULL)) );
	

	auto sky2 = Sprite::create("main/sky_overlay_2.png");
	sky2->setPosition(510, 559);
	addChild(sky2, 1);
	sky2->runAction( RepeatForever::create(Sequence::create(FadeTo::create(1.7f, 255), FadeTo::create(1.5f, 0), NULL)) );



	TTFConfig config("LetusPray-Regular.ttf",32);
	auto tapToGo = Label::createWithTTF(config,"Tap on the book to play",TextHAlignment::LEFT);
	tapToGo->setPosition(origin.x + visibleSize.width / 2, 250);
	_tapTogo = tapToGo;
	_tapTogo->setOpacity(0);
	addChild(_tapTogo, 10);

	_tapTogo->runAction(Sequence::create(DelayTime::create(7), FadeTo::create(3, 255), NULL));
	BGMUS;


	//for controller only
	this->schedule(schedule_selector(MainMenu::update));
	
	


	return true;

	}

void MainMenu::update(float dt)
{
	// if is running
	//CCLOG("check_1up");
#ifdef _WIN32
	App::get()->_player->check_controller();
#endif	

}
