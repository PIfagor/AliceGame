#ifndef _WORLD_LEVELS_H_
#define _WORLD_LEVELS_H_
#include "cocos2d.h"
#include "Enums.h"

class WorldLevels : public cocos2d::LayerColor
{
public:
	virtual ~WorldLevels();

	static cocos2d::Scene* createScene(bool inGame = false);

	virtual bool init() override;
	void update(float);


	CREATE_FUNC(WorldLevels);

private:
	WorldLevels();
	DISALLOW_COPY_AND_ASSIGN(WorldLevels);

	std::vector<cocos2d::Sprite*> _levels;
	cocos2d::EventListener*	_listener;
	int _deltaX;
	int _deltaY;
	int CW; // curent world
	cocos2d::Sprite* 	_arrow;
};



#endif 
