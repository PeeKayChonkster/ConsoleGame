#include "Controls.h"




/////////////////////////////// Controls stuff ////////////////////////////////
std::vector<Action> Controls::readControlsKeyboard()
{
	////////////Keyboard
	std::vector<Action> dirBuffer;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		dirBuffer.push_back(MOVE_UP);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		dirBuffer.push_back(MOVE_DOWN);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		dirBuffer.push_back(MOVE_LEFT);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		dirBuffer.push_back(MOVE_RIGHT);
	}
	if (GetAsyncKeyState(' ') & 0x8000)
	{
		dirBuffer.push_back(MOVE_JUMP);
	}
	///////////End of keyboard
	
	return dirBuffer;
}


std::map<Action, double> Controls::readControlsGamepad()			//Xinput controller
{
	std::map<Action, double> controlsGamepad;
	XInputGetState(1, &controllerState);
	//buttons
	switch (controllerState.Gamepad.wButtons)
	{
	case XINPUT_GAMEPAD_A:
		controlsGamepad.insert({ MOVE_JUMP,jumpConst });
		break;
	case XINPUT_GAMEPAD_B:
		break;
	case XINPUT_GAMEPAD_X:
		break;
	case XINPUT_GAMEPAD_Y:
		break;
	case XINPUT_GAMEPAD_DPAD_UP:
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		break;
	case XINPUT_GAMEPAD_START:
		break;
	case XINPUT_GAMEPAD_BACK:
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		break;
	}
	//end of buttons
	//thumbsticks
	if (controllerState.Gamepad.sThumbLX != 0)
	{
		controlsGamepad.insert({ STICK_MOVE_X,controllerState.Gamepad.sThumbLX });
	}
	if (controllerState.Gamepad.sThumbLY != 0)
	{
		controlsGamepad.insert({ STICK_MOVE_Y,controllerState.Gamepad.sThumbLY });
	}
	//end of thumbsticks
	return controlsGamepad;
}

//////////////////////////////////////////////////////////////////////////////