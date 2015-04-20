#include <memory>
#include <sstream>

#include "json/prettywriter.h"	
#include "json/filestream.h"
#include "json/reader.h"
#include "json/stringbuffer.h"


#include "Splash.h"

#include "WorldMap.h"
#include "WorldLevels.h"

#include "LevelMap.h"
#include "MainMenu.h"
#include "Applic.h"
#include "Utilits.h"
#include "Menuscene.h"

USING_NS_CC;

App::App() :
_canTouch(true),
_resolution(Resolution::REGULAR_RES),
_result(PAUSE),
#ifdef _WIN32
 _player(new CXBOXController(1)),
#endif
_reloadOnTheFly(false)

{
	CCLOG("App constructor");
	_currentWorld = 0;
	_currentMap = 0;

	//for debug
	setReloadOnTheFly(true);


	loadUserData();



}

App::~App()
{
	CCLOG("App destructor");

}

void App::showSplash()
{
	auto director = Director::getInstance();
	auto splashScene = Splash::createScene();
	director->runWithScene(splashScene);
}

void App::gotoMenu(Ref* node)
{
	auto director = Director::getInstance();
	auto menuScene = MainMenu::createScene();
	director->replaceScene(menuScene);

	CCLOG("REPLACING with Main menu");
}

void App::gotoWorldMap(Ref* node)
{
	auto director = Director::getInstance();
	auto mapScene = WorldMap::createScene();
	auto transition = TransitionFade::create(2.0f, mapScene);
	director->replaceScene(transition);

	CCLOG("REPLACING with World  map");
}

//void App::gotoWorldOne(Ref* node)
//{
//	auto director = Director::getInstance();
//	auto mapScene = WorldOne::createScene();
//	director->replaceScene(mapScene);
//
//	CCLOG("REPLACING with World  One");
//}

void App::gotoWorldLevels(Ref* node)
{
	auto director = Director::getInstance();
	auto mapScene = WorldLevels::createScene();
	director->replaceScene(mapScene);

	CCLOG("REPLACING with Levels");
}

//void App::gotoWorldTwo(Ref* node)
//{
//	auto director = Director::getInstance();
//	auto mapScene = WorldTwo::createScene();
//	director->replaceScene(mapScene);
//
//	CCLOG("REPLACING with World  Two");
//}
//void App::gotoWorldThree(Ref* node)
//{
//	auto director = Director::getInstance();
//	auto mapScene = WorldThree::createScene();
//	director->replaceScene(mapScene);
//
//	CCLOG("REPLACING with World  Three");
//}


void App::startLevel(Ref* pSender)
{
	
	auto director = Director::getInstance();
	auto lvlScene = MenuScene::createScene();
	
	director->replaceScene(lvlScene);

	CCLOG("REPLACING with Level  map");

};


void App::resumeGame(Ref* pSender)
{
	CCLOG("Resuming game");
}

App::userData::userData()

{
	for (size_t i = 0; i < COUNT_WORLDS; i++)
	{
		_complitedLvls.push_back(std::vector<bool>());
		_availableLvls.push_back(std::vector<bool>());
		for (size_t j = 0; j < COUNT_LVLS_ON_1_WORLD; j++)
		{
			_complitedLvls[i].push_back(0);
			_availableLvls[i].push_back(1);
		}

	}
	CCLOG("Config struct created");
}

std::string App::userData::toStr() const
{
	std::stringstream ss;
	for (size_t i = 0; i < COUNT_WORLDS; i++)
	{
		ss << "Complited levels\n";
		ss << i << "-Wolrd: ";
		for (size_t j = 0; j < COUNT_LVLS_ON_1_WORLD; j++)
		{
			ss << _complitedLvls[i][j]<<"\t";
		}
		ss << "\n";
		ss << "Available levels\n";
		ss << i << "-Wolrd: ";
		for (size_t j = 0; j < COUNT_LVLS_ON_1_WORLD; j++)
		{
			ss << _availableLvls[i][j] << "\t";
		}
		ss << "\n\n";
	}
	return ss.str();
}

std::string App::getUserData()
{
	std::string localDataDir = FileUtils::getInstance()->getWritablePath();
	std::string localUserDataPath = localDataDir + CONFIG_FILENAME;

	if (!FileUtils::getInstance()->isFileExist(localUserDataPath)) {
		CCLOG("UserData file doesn't exist. Creating new and copying content");

		std::string defaultUserData = FileUtils::getInstance()->getStringFromFile(DEFAULT_CONFIG_FILENAME);
		writeTextToFile(defaultUserData, localUserDataPath);
	}
	else {
		CCLOG("UserData file exists");
	}

	std::string userData = FileUtils::getInstance()->getStringFromFile(localUserDataPath);

	return userData;
}

void App::loadUserData()
{
	std::string configFileContent = getUserData();

	rapidjson::Document doc;
	rapidjson::Value value;
	if (doc.Parse<0>(configFileContent.c_str()).HasParseError()) {
		CCLOG("Confid parse error");
		return;
	}

	const rapidjson::Value & worldsArr = doc["worldAndLevels"];
	assert(worldsArr.IsArray());


	const rapidjson::Value & worldsAvaibleArr = doc["worldAndAvalibleLevels"];
	assert(worldsAvaibleArr.IsArray());

	for (size_t i = 0; i < COUNT_WORLDS; i++)
	{
		
		const rapidjson::Value & levels = worldsArr[i];
		assert(worldsArr.IsArray());

		const rapidjson::Value & avaibleLevels = worldsAvaibleArr[i];
		assert(worldsAvaibleArr.IsArray());

		for (size_t j = 0; j < COUNT_LVLS_ON_1_WORLD; j++)
		{
			const rapidjson::Value & objValue = levels[j];
			assert(objValue.IsBool());
			_userData._complitedLvls[i][j] = objValue.GetBool();

			const rapidjson::Value & objValue2 = avaibleLevels[j];
			assert(objValue2.IsBool());
			_userData._availableLvls[i][j] = objValue2.GetBool();
			
		}
		
	}

		
	CCLOG(("Local config file:" + configFileContent).c_str());
	CCLOG(_userData.toStr().c_str());
}

void App::saveUserData()
{
	CCLOG(">>>>>>>>>>>>Saving user data file");

	std::string localDataDir = FileUtils::getInstance()->getWritablePath();
	std::string localUserDataPath = localDataDir + CONFIG_FILENAME;

	std::string configFileContent = FileUtils::getInstance()->getStringFromFile(localUserDataPath);
	rapidjson::Document doc;
	if (doc.Parse<0>(configFileContent.c_str()).HasParseError()) {
		CCLOG("User data parse error (While saving)");
		return;
	}

	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	doc.SetObject();

	rapidjson::Value worldsArr(rapidjson::kArrayType);
	rapidjson::Value worldsAvaibleArr(rapidjson::kArrayType);

	for (size_t i = 0; i < COUNT_WORLDS; i++)
	{
		rapidjson::Value levels(rapidjson::kArrayType);

		for (size_t j = 0; j < COUNT_LVLS_ON_1_WORLD; j++)
		{
			
			rapidjson::Value objValue;
			objValue.SetBool(_userData._complitedLvls[i][j]);

			levels.PushBack(objValue, allocator);
		}
		worldsArr.PushBack(levels, allocator);
	}

	doc.AddMember("worldAndLevels", worldsArr, allocator);

	for (size_t i = 0; i < COUNT_WORLDS; i++)
	{
		rapidjson::Value levels(rapidjson::kArrayType);

		for (size_t j = 0; j < COUNT_LVLS_ON_1_WORLD; j++)
		{

			rapidjson::Value objValue;
			objValue.SetBool(_userData._availableLvls[i][j]);

			levels.PushBack(objValue, allocator);
		}
		worldsAvaibleArr.PushBack(levels, allocator);
	}

	
	doc.AddMember("worldAndAvalibleLevels", worldsAvaibleArr, allocator);

	
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	doc.Accept(writer);
	std::string str = strbuf.GetString();

	writeTextToFile(str, localUserDataPath);


	CCLOG("Config file saved");
}


void App::loadGameData()
{
	CCLOG("-------------------------------------------");
	CCLOG("Loading game data:");
	std::string gameDataContent = FileUtils::getInstance()->getStringFromFile(GAME_DATA_PATH);

	rapidjson::Document doc;
	if (doc.Parse<0>(gameDataContent.c_str()).HasParseError()) {
		CCLOG("Error parsing game data file");
		return;
	}

	loadSearthPaths(doc);
	//loadLocations(doc);

	CCLOG("-------------------------------------------");
}

void App::loadSearthPaths(const rapidjson::Document & doc)
{
	StrVec paths;
	std::string prefix;

	// slave paths depend on resolution, so we need to add prefix
	if (_resolution == Resolution::REGULAR_RES) {
		paths.push_back(REGULAR_RESOLUTION_PATH);
		prefix = REGULAR_RESOLUTION_PATH + "/";
	}
	else {
		paths.push_back(RETINA_RESOLUTION_PATH);
		prefix = RETINA_RESOLUTION_PATH + "/";
	}

	const rapidjson::Value & genPathArr = doc["generalSearchPaths"];
	for (rapidjson::SizeType i = 0; i < genPathArr.Size(); ++i) {
		std::string path = genPathArr[i].GetString();
		paths.push_back(path);
	}

	const rapidjson::Value & slavePathArr = doc["slaveSearchPaths"];
	for (rapidjson::SizeType i = 0; i < slavePathArr.Size(); ++i) {
		std::string path = prefix + slavePathArr[i].GetString();
		paths.push_back(path);
	}

	for (auto it = paths.begin(); it != paths.end(); ++it) {
		std::stringstream ss;
		ss << "Path added: " << *it;
		CCLOG(ss.str().c_str());
	}


	FileUtils::getInstance()->setSearchPaths(paths);
}

//void App::loadLocations(const rapidjson::Document & doc)
//{
//	// in case you want to reload locations in runtime
//	_locations.clear();
//
//	const rapidjson::Value & locations = doc["locations"];
//	assert(locations.IsArray());
//
//	std::stringstream ss;
//	ss << "-------------------\n";
//	ss << "Starting parsing game data file: \n";
//
//	for (rapidjson::SizeType i = 0; i < locations.Size(); ++i) {
//		const rapidjson::Value & locObj = locations[i];
//		assert(locObj.IsObject());
//
//		Location loc;
//		loc.background = locObj["background"].GetString();
//		loc.music = locObj["backMusic"].GetString();
//
//		ss << "back: " << loc.background << "\n";
//		ss << "music:" << loc.music << "\n";
//
//		//////////////////////////////////////////////////////////////////
//		// parsing blocked tiles
//		const rapidjson::Value & blockedTiles = locObj["blockedTiles"];
//		assert(blockedTiles.IsArray());
//
//		for (rapidjson::SizeType i = 0; i < blockedTiles.Size(); ++i) {
//			const rapidjson::Value & tile = blockedTiles[i];
//			assert(tile.IsArray());
//			assert(tile.Size() == 2);
//
//			int x = tile[(rapidjson::SizeType)0].GetInt();
//			int y = tile[(rapidjson::SizeType)1].GetInt();
//
//			loc.blockedTiles.push_back(Vec2(x, y));
//
//			ss << "Blocked tile: [" << x << "," << y << "]\n";
//		}
//
//		///////////////////////////////////////////////////////////////////
//		// parsing decorations
//		const rapidjson::Value & decor = locObj["decor"];
//		assert(decor.IsArray());
//
//		for (rapidjson::SizeType i = 0; i < decor.Size(); ++i) {
//			const rapidjson::Value & item = decor[i];
//			assert(item.IsObject());
//
//			Decor dec;
//			dec.pos.x = item["x"].GetInt();
//			dec.pos.y = item["y"].GetInt();
//			dec.image = item["image"].GetString();
//			dec.opacity = item["opacity"].GetDouble();
//
//			ss << "Decor:\n";
//			ss << "x: " << dec.pos.x << ", y: " << dec.pos.y << ", opacity: " << dec.opacity << ", image: " << dec.image << "\n";
//
//			loc.decor.push_back(dec);
//		}
//
//		///////////////////////////////////////////////////////////////////
//		// parsing NPC
//		const rapidjson::Value & npcs = locObj["NPC"];
//		assert(npcs.IsArray());
//
//		for (rapidjson::SizeType i = 0; i < npcs.Size(); ++i) {
//			const rapidjson::Value & npc = npcs[i];
//			assert(npc.IsObject());
//
//			Unit unit;
//			unit.pos_x = npc["tileX"].GetInt();
//			unit.pos_y = npc["tileY"].GetInt();
//			unit.file = npc["file"].GetString();
//
//			ss << "NPC:\n";
//			ss << "posX: " << unit.pos_x << ", y: " << unit.pos_y << ", file:" << unit.file << "\n";
//
//			const rapidjson::Value & labels = npc["standLabels"];
//			assert(labels.IsArray());
//
//			for (rapidjson::SizeType i = 0; i < labels.Size(); ++i) {
//				const rapidjson::Value & lb = labels[i];
//				assert(lb.IsObject());
//
//				StrIntPair pair;
//				pair.first = lb["labelName"].GetString();
//				pair.second = lb["ammount"].GetInt();
//
//				unit.standLabels.push_back(pair);
//
//				ss << "Adding label: " << pair.first << " : " << pair.second << "\n";
//			} // label for
//
//			unit.attackLabel = npc["attackLabel"].GetString();
//			ss << "Adding attack label: " << unit.attackLabel << "\n";
//
//			loc.units.push_back(unit);
//
//		} // unit for
//		ss << "units loaded: " << loc.units.size() << "\n";
//		///////////////////////////////////////////////////////////////////
//		// parsing particles
//		const rapidjson::Value & particles = locObj["particles"];
//		assert(particles.IsArray());
//
//		for (rapidjson::SizeType i = 0; i < particles.Size(); ++i) {
//			const rapidjson::Value & part = particles[i];
//			assert(part.IsObject());
//
//			Particle particle;
//			particle.file = part["file"].GetString();
//			particle.pos.x = part["x"].GetInt();
//			particle.pos.y = part["y"].GetInt();
//
//			loc.particles.push_back(particle);
//
//			ss << "Particle: " << particle.file << ", x: " << particle.pos.x << ", y:" << particle.pos.y << "\n";
//		}
//
//
//		///////////////////////////////////////////////////////////
//		// parsing effects
//		const rapidjson::Value & effects = locObj["effects"];
//		assert(effects.IsArray());
//
//		for (rapidjson::SizeType i = 0; i < effects.Size(); ++i) {
//			const rapidjson::Value & part = effects[i];
//			assert(part.IsObject());
//
//			Effect effect;
//			effect.pos.x = part["x"].GetInt();
//			effect.pos.y = part["y"].GetInt();
//			effect.image = part["image"].GetString();
//			effect.opacityStart = part["opacityStart"].GetDouble();
//			effect.opacityEnd = part["opacityEnd"].GetDouble();
//			effect.time = part["time"].GetDouble();
//
//			ss << "Effect: " << effect.image << ", x: " << effect.pos.x << ", y: " << effect.pos.y << ", opacityStart:" << effect.opacityStart * 255 << ", opacityEnd: " << effect.opacityEnd * 255 << "\n";
//
//			loc.effects.push_back(effect);
//		}
//
//		///////////////////////////////////////////////////////////
//
//		_locations.push_back(loc);
//
//		CCLOG(ss.str().c_str());
//
//	} // location for
//}
//
//Location & App::getCurrentLocation()
//{
//	assert(_userData.city > 0);
//	assert(_locations.size() > 0);
//
//	return _locations.at(_userData.city - 1);
//}