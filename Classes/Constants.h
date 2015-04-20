#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <string>
#include <vector>

typedef std::vector<std::string>				StrVec;
typedef const std::string						STR;
typedef std::pair<std::string, unsigned int>	StrIntPair;
typedef std::vector<StrIntPair>					StrIntPairVec;

//#define STR const std::string 

STR CONFIG_FILENAME = "userData.json";
STR	DEFAULT_CONFIG_FILENAME = "defaultUserData.json";
STR REGULAR_RESOLUTION_PATH = "regularRes";
STR RETINA_RESOLUTION_PATH = "retinaRes";
STR GAME_DATA_PATH = "gameData.json";

// in pixels
const int DESIGN_WIDTH = 1024;
const int DESIGN_HEIGHT = 768;

const int SIDEBAR_WIDTH = 110;

//const int TILE_WIDTH = 90;
//const int TILE_HEIGHT = 73;

const int TILE_DELTA_X = 8;
const int TILE_DELTA_Y = 8;

// width, height in tiles
//const int MAP_WIDTH = 8;
//const int MAP_HEIGHT = 6;


const int COUNT_WORLDS = 4;
const int COUNT_LVLS_ON_1_WORLD = 13;

enum Resolution {
	REGULAR_RES,
	RETINA_RES
};


//enum TileType {
//	EMPTY,
//	EXIT,
//	QUEST,
//	ITEM,
//	MONSTER
//};

enum ItemType {
	NO_ITEM,
	MEDKIT,
	MANA
};

enum MonsterType {
	NO_MONSTER,
	FRANK,
	GOLEM,
	SKELETON
};




enum RenderOrder {
	MAP_BACK_ORDER = 1,
	TILE_ORDER,
	DECOR_ORDER,
	FEET_ORDER,
	TILE_ITEM_ORDER
};

enum SpriteTags {
	TILE_TAG
};



enum FeetDirection {
	FEET_UP,
	FEET_RIGHT,
	FEET_DOWN,
	FEET_LEFT
};


enum TileBatchNodes {
	ACTIVE_BATCH,
	INACTIVE_BATCH,
	OVERLAY_BATCH
};

#endif