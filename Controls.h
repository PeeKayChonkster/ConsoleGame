#pragma once
#pragma comment(lib, "XInput.lib")
#include "CGException.h"
#include <Windows.h>
#include <Xinput.h>
#include <vector>
#include <map>


const float jumpConst = 60000.0f;

enum Action { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, MOVE_JUMP, STICK_MOVE_X, STICK_MOVE_Y };

class Controls
{
private:
	XINPUT_STATE controllerState;
public:
	//Controls() {}
	std::vector<Action> readControlsKeyboard();
	std::map<Action, double> readControlsGamepad();


};