#pragma once
#include "Graphycs.h"
#include "Clock.h"
#include <thread>



class App
{
public:
	App();
	void play();
	void changeLevel();
private:
	Clock clock;
	Player* playerPtr;
	Graphycs graphycs;
	Map* currMap;
};