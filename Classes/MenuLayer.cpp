#include "MenuLayer.h"


MenuLayer* MenuLayer::getMenu(cocos2d::Color4B var)
{
	MenuLayer* layer = MenuLayer::create();
	layer->initWithColor(var);

	return layer;
}

bool MenuLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}
		
	
	this->setResults(App::get()->getResult());
	return true;
}

void MenuLayer::setResults(Results r)
{
	auto wiew = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto callback = std::bind(&MenuLayer::menuToLevels, this, this);
	

	MenuItemImage* goToLevels;
	MenuItemImage* nextReload;
	MenuItemImage* backTMap;

	auto goNextMap = std::bind(&MenuLayer::nextMap, this, this);
	auto reload = std::bind(&MenuLayer::reloadMap, this, this);
	auto back = std::bind(&MenuLayer::backToMap, this, this);

	

	auto sprite = Sprite::create("menu_fon.png");
	sprite->setPosition(Vec2(wiew.width / 2 , wiew.height / 2 ));
	addChild(sprite, 0);

	/*switch (r)
	{
	case WIN:

		nextReload = MenuItemImage::create("next_lvl.png", "exit_pressed.png", goNextMap);
		break;
	case LOSE:

		nextReload = MenuItemImage::create("reload.png", "exit_pressed.png", reload);
		break;
	case PAUSE:
		nextReload = MenuItemImage::create("back_lvl.png", "exit_pressed.png", back);
		break;
	default:
		break;
	}*/


		
	
		goToLevels = MenuItemImage::create("menu_btn.png", "menu_psd.png", callback);
		goToLevels->setAnchorPoint(Vec2(0, 1));
		goToLevels->setPosition(Vec2(15, goToLevels->getBoundingBox().size.height +25));

		nextReload = MenuItemImage::create("replay_btn.png", "replay_psd.png", reload);
		nextReload->setAnchorPoint(Vec2(1, 0));
		nextReload->setPosition(Vec2(wiew.width -15, nextReload->getBoundingBox().size.height+50));

		backTMap = MenuItemImage::create("play_btn.png", "play_psd.png", back);
		backTMap->setAnchorPoint(Vec2(1, 0));
		backTMap->setPosition(Vec2(wiew.width - 15, 25));

		Menu* pMenu = Menu::create(goToLevels, nextReload, backTMap, NULL);
		pMenu->setPosition(Point::ZERO);
		addChild(pMenu, 1);
	
};

void MenuLayer::reloadMap(Ref *node)
{
	App::get()->startLevel(NULL);
};
void MenuLayer::nextMap(Ref *node)
{
	App::get()->incrementCurrnetMap();
	App::get()->startLevel(NULL);
};
void MenuLayer::backToMap(Ref *node)
{
	this->setVisible(false);
};



void MenuLayer::menuToLevels(Ref* pSender)
{
	App::get()->gotoWorldLevels(NULL);
}

void MenuLayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
	Director::getInstance()->end();

#endif
}