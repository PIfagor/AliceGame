#include "XBOXController.h"

#include <iostream>
using namespace std;

CXBOXController::CXBOXController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;

	for (int i = 0; i < BUTTON_NUM; ++i)
	{
		_is_btn_pressed[i] = false;
	}
	buttons = std::vector<std::function<void()>>(BUTTON_NUM);
}

XINPUT_STATE CXBOXController::GetState()
{
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
	XInputGetState(_controllerNum, &_controllerState);
	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	return (Result == ERROR_SUCCESS);
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	XINPUT_VIBRATION Vibration;
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	XInputSetState(_controllerNum, &Vibration);
}

void CXBOXController::test()
{
	XINPUT_STATE state = _controllerState;
	if (state.Gamepad.bLeftTrigger) {
		cout << "Left trigger: " << (int)state.Gamepad.bLeftTrigger << endl << endl;
	}

	if (state.Gamepad.bRightTrigger) {
		cout << "Right trigger: " << (int)state.Gamepad.bRightTrigger << endl << endl;
	}
}

const Vec2 & CXBOXController::leftThumbTest()
{
	XINPUT_STATE state = _controllerState;

	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

		normalizedLX = normalizedLX * 1000;
		normalizedLY = normalizedLY * 1000;

		std::stringstream ss;
		ss << "magnitude is: " << normalizedMagnitude << endl;
		ss << "LX = " << normalizedLX << endl;
		ss << "LY = " << normalizedLY << endl;

		//CCLOG(ss.str().c_str());

		return Vec2(normalizedLX, normalizedLY);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	return Vec2(0, 0);
	//repeat for right thumb stick
}


// CHANGE names to "right"
void CXBOXController::rightThumbTest()
{
	XINPUT_STATE state = _controllerState;

	float LX = state.Gamepad.sThumbRX;
	float LY = state.Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX*LX + LY*LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (magnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

		cout << "magnitude is: " << normalizedMagnitude << endl;
		cout << "RX = " << normalizedLX << endl;
		cout << "RY = " << normalizedLY << endl;
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	//repeat for right thumb stick
}


/////////////////////////////////////////////////////////////////////

bool CXBOXController::getBtnState(int btn)
{
	return _is_btn_pressed[btn];
}

void CXBOXController::setBtnState(int btn, bool state)
{
	_is_btn_pressed[btn] = state;
}

bool CXBOXController::isBtnPressed(int btn)
{
	switch (btn)
	{
	case CXBOXController::BUTTON_B:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B);
		break;
	case CXBOXController::BUTTON_Y:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y);
		break;
	case CXBOXController::BUTTON_X:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X);
		break;
	case CXBOXController::BUTTON_A:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A);
		break;
	case CXBOXController::BUTTON_START:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START);
		break;
	case CXBOXController::BUTTON_BACK:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
		break;
	case CXBOXController::TRIGGER_LEFT:
		return (this->GetState().Gamepad.bLeftTrigger);
		break;
	case CXBOXController::TRIGGER_RIGHT:
		return (this->GetState().Gamepad.bRightTrigger);
		break;
	case CXBOXController::BUTTON_LB:
		return (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case CXBOXController::BUTTON_RB:
		return (GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case CXBOXController::BUTTON_RIGHT:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case CXBOXController::BUTTON_LEFT:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case CXBOXController::BUTTON_TOP:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
		break;
	case CXBOXController::BUTTON_BOTTOM:
		return (this->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	default:
		return false;
		break;
	}
}

bool CXBOXController::hasBtnBeenPressed(int btn)
{
	if (isBtnPressed(btn))
	{
		setBtnState(btn, true);
	}
	else {
		if (getBtnState(btn)) {
			setBtnState(btn, false);
			return true;
		}
	}
	return false;
}


void CXBOXController::check_controller()
{

	if (this->IsConnected()) {
		Vec2 pt = this->leftThumbTest();
		for (auto i = 0; i < BUTTON_NUM; ++i)
		{
			if (buttons[i]!=nullptr)
			{
				if (this->hasBtnBeenPressed(i))		{ buttons[i](); }
			}
			
		}

		}
	else {
		
	}
}

void CXBOXController::clear_controller()
{
	for (auto  it = buttons.begin(); it !=buttons.end(); ++it)
	{
		*it = nullptr;
	}
}