#ifndef _LEVEL_MAP_H_
#define _LEVEL_MAP_H_

#include "cocos2d.h"

class LevelMap 
{
public:
	virtual ~LevelMap();

	


	static LevelMap * get() {
		static LevelMap lvlMap;
		return &lvlMap;
	}

#ifdef _WIN32
	//void installControlerButtonsOnEachWorld(Sprite& _arrow, int _deltaX, int _deltaY);
#endif
private:
	LevelMap();
	DISALLOW_COPY_AND_ASSIGN(LevelMap);

	
};

#endif 
