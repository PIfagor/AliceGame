#include "Splash.h"
#include "Applic.h"

USING_NS_CC;

Splash::Splash()
{
}

Splash::~Splash()
{
}

Scene* Splash::createScene()
{
	auto scene = Scene::create();
	auto layer = Splash::create();
	scene->addChild(layer);

	return scene;
}

bool Splash::init()
{

	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	auto sprite = Sprite::create("splash.png");

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->addChild(sprite, 0);

	// set delay and go to next scene
	std::function<void(Node*)> nextScene = std::bind(&App::gotoMenu, App::get(), this);
	CCFiniteTimeAction *seq1 = CCSequence::create(CCDelayTime::create(2.0), CCCallFuncN::create(nextScene), NULL);

	this->runAction(seq1);


	return true;
}