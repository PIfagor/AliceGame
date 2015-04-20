#ifndef _APPLIC_H_
#define _APPLIC_H_

//#include "Macro.h"
#include "cocos2d.h"
//#include "Map.h"
//#include "Location.h"
#include "Enums.h"
#include "Constants.h"
#include "json/document.h"
#include "AudioEngine.h"

#ifdef _WIN32
#include "XBOXController.h";
#endif



class User;

class App {
public:
	// data that needs to be saved is located here
	struct userData {
		std::vector<std::vector<bool>> _complitedLvls;
		std::vector<std::vector<bool>> _availableLvls;
		userData();

		std::string toStr() const;
	};


	virtual ~App();

	static App * get() {
		static App app;
		return &app;
	}

	void showSplash();
	void gotoMenu(cocos2d::Ref*);
	void startGame(cocos2d::Ref*);
	void startLevel(cocos2d::Ref*);
	void resumeGame(cocos2d::Ref*);
	void gotoWorldMap(cocos2d::Ref*);
	void gotoLevelMap(cocos2d::Ref*);
	void gotoWorldLevels(cocos2d::Ref*);
	void gotoWorldOne(cocos2d::Ref*);
	void gotoWorldTwo(cocos2d::Ref*);
	void gotoWorldThree(cocos2d::Ref*);
	void loadUserData();
	void saveUserData();

	void loadGameData();

	int getCurrentWorld() { return _currentWorld;};
	int getCurrentMap() { return _currentMap; };

	void setCurrentWorld(int data) { _currentWorld = data; };
	void setCurrentMap(int data) { _currentMap = data; };
 	
	int	 getMenuBgrAudioID(){ return _menuBGAID; };
	void setMenuBgrAudioID(int data) { _menuBGAID = data; };

	void incrementCurrnetMap() { _currentMap++;};
	void decrementCurrnetMap() { _currentMap--;};
	void incrementCurrnetWorld() { _currentWorld++; };
	void decrementCurrnetWorld() { _currentWorld--; };

	const userData & getConfig() const { return _userData; };
	const void  setConfig(bool isCompite) { _userData._complitedLvls[_currentWorld][_currentMap] = isCompite; };
	const std::shared_ptr<User> & getUser() const { return _user; };
	//void setCurrentMap(rpg::Map* map) { _currentMap = map; };
	//rpg::Map * getCurrentMap() const { return _currentMap; };

	void setCanTouch(const bool & state) { _canTouch = state; };
	const bool & canTouch() { return _canTouch; };
	void setResolution(const Resolution& res) { _resolution = res; };

	//Location & getCurrentLocation();
	const bool & reloadOnTheFly() const { return _reloadOnTheFly; };
	void setReloadOnTheFly(const bool & state) { _reloadOnTheFly = state; };

	void	setResult(Results res)	{ _result = res; };
	Results getResult()				{ return _result; };
	
#ifdef _WIN32
	CXBOXController *	_player;
#endif

private:
	App();
	DISALLOW_COPY_AND_ASSIGN(App);

	std::string getUserData();
	void loadSearthPaths(const rapidjson::Document &);
	//void loadLocations(const rapidjson::Document &);

	////////////////////////////////////////////////
	// FIELDS
	userData						_userData;
	std::shared_ptr<User>			_user;
	//rpg::Map*						_currentMap;
	bool                            _canTouch;
	Resolution						_resolution;
	//std::vector<Location>			_locations;
	bool							_reloadOnTheFly;
	int								_currentWorld;
	int								_currentMap;
	int								_menuBGAID; //menu background audio ID

	Results							_result;

};

#endif