#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// to make win32api compile faster and reduce the size of win headers
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <XInput.h>
#include <cmath>

#include "cocos2d.h"
USING_NS_CC;

// Now, the XInput Library
#pragma comment(lib, "XInput.lib")

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  17849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

#define GEN(type,var) \
	const type & get##var() const { return var; }; \
	void set##var(const type & val) { var = val; }; 

class CXBOXController
{
public:
	enum Buttons {
		BUTTON_B,
		BUTTON_Y,
		BUTTON_A,
		BUTTON_X,

		BUTTON_START,
		BUTTON_BACK,

		BUTTON_LB,
		BUTTON_RB,

		BUTTON_RIGHT,
		BUTTON_LEFT,
		BUTTON_TOP,
		BUTTON_BOTTOM,

		TRIGGER_LEFT,
		TRIGGER_RIGHT,

		// NEVER TOUCH THIS ONE
		BUTTON_NUM
	};

	std::vector<std::function<void()>> buttons;


	CXBOXController(int);

	XINPUT_STATE GetState();

	bool IsConnected();

	void Vibrate(int leftVal = 0, int rightVal = 0);

	void test();

	const Vec2 & leftThumbTest();

	void rightThumbTest();

	//////////////////////////////////////////////////////////
	bool getBtnState(int);
	void setBtnState(int, bool);
	bool isBtnPressed(int);
	bool hasBtnBeenPressed(int);

	void clear_controller();
	void check_controller();

	void setButtomFunction(std::function<void()> func, Buttons btn) { buttons[btn] = func; };

	
private:

	XINPUT_STATE	_controllerState;
	int				_controllerNum;

	int				_is_btn_pressed[BUTTON_NUM];

	


	

};




#endif