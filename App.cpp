#include "App.h"

extern const int FramesPerSecond = 60;
extern const int MaxEntities = 5;

App::App() : clock(), graphycs()
{
	currMap = new Map(10, 10);
	currMap->diskIn(L"Pond.MAP");
	playerPtr = new Player(L"Пашок", { 3.0, 3.0 }, { '@', int(colors::BrightWhite) | BACKGROUND_BLUE | BACKGROUND_INTENSITY }, 50.0, currMap);
	playerPtr->setColor(int(colors::BrightWhite) + 144);
	graphycs.setMap(currMap);
}



void App::play()
{	
	try
	{
		while (true)
		{
			clock.waitForFrame(FramesPerSecond);
			playerPtr->ePhysics.assignMoveForces(playerPtr->controls.readControlsKeyboard());
			playerPtr->ePhysics.assignMoveForces(playerPtr->controls.readControlsGamepad());
			playerPtr->ePhysics.assignGravForce();
			playerPtr->ePhysics.calculateMovement();
			graphycs.drawLevel(*playerPtr->getCurrMap());
			graphycs.drawPlayer(*playerPtr);
			graphycs.drawGameInformation(*playerPtr);
			graphycs.sendScreenToConsole();
			if (playerPtr->ePhysics.isOutOfBounds())
			{
				changeLevel();
			}
		}
	}
	catch (CGException& ex)
	{
		std::wstringstream wss;
		wss << "EXCEPTION: " << ex.message << " File: " << ex.file.c_str() << "; Line:" << ex.line << ";\n";
		OutputDebugStringW(wss.str().c_str());
		exit(1);
	}
}


void App::changeLevel()
{

	if (playerPtr->ePhysics.getCoordinates().X > currMap->getLevW() - 1)	//right level
	{
		double tilesFromFloorY = double(currMap->getLevH() - playerPtr->ePhysics.getCoordinates().Y);
		std::wstring levName = currMap->getNameRIGHT();
		currMap = new Map(10, 10);
		currMap->diskIn(levName);
		graphycs.setMap(currMap);
		playerPtr->ePhysics.setCurrMap(currMap, { 0, currMap->getLevH() - tilesFromFloorY });
		playerPtr->setColor(int(colors::BrightWhite) + currMap->getBackground());
		graphycs.setScreenCenter(playerPtr->ePhysics.getCoordinates());
		graphycs.changeScreenEffect(currMap);
	}
	else
	if (playerPtr->ePhysics.getCoordinates().X < 1.0)						//left level
	{
		double tilesFromFloorY = double(currMap->getLevH() - playerPtr->ePhysics.getCoordinates().Y);
		std::wstring levName = currMap->getNameLEFT();
		currMap = new Map(10, 10);
		currMap->diskIn(levName);
		graphycs.setMap(currMap);
		playerPtr->ePhysics.setCurrMap(currMap, { double(currMap->getLevW() - 1), currMap->getLevH() - tilesFromFloorY });
		playerPtr->setColor(int(colors::BrightWhite) + currMap->getBackground());
		graphycs.setScreenCenter(playerPtr->ePhysics.getCoordinates());
		graphycs.changeScreenEffect(currMap);
	}
	else
	if (playerPtr->ePhysics.getCoordinates().Y < 1.0)						//upper level
	{
		double tilesFromFloorX = double(playerPtr->ePhysics.getCoordinates().X);
		std::wstring levName = currMap->getNameUP();
		currMap = new Map(10, 10);
		currMap->diskIn(levName);
		graphycs.setMap(currMap);
		playerPtr->ePhysics.setCurrMap(currMap, { tilesFromFloorX,  double(currMap->getLevH() - 1) });
		playerPtr->setColor(int(colors::BrightWhite) + currMap->getBackground());
		graphycs.setScreenCenter(playerPtr->ePhysics.getCoordinates());
		graphycs.changeScreenEffect(currMap);
	}
	else
	if (playerPtr->ePhysics.getCoordinates().Y > currMap->getLevH() - 1)						//lower level
	{
		double tilesFromFloorX = double(playerPtr->ePhysics.getCoordinates().X);
		std::wstring levName = currMap->getNameDOWN();
		currMap = new Map(10, 10);
		currMap->diskIn(levName);
		graphycs.setMap(currMap);
		playerPtr->ePhysics.setCurrMap(currMap, { playerPtr->ePhysics.getCoordinates().X, 1.0 });
		playerPtr->setColor(int(colors::BrightWhite) + currMap->getBackground());
		graphycs.setScreenCenter(playerPtr->ePhysics.getCoordinates());
		graphycs.changeScreenEffect(currMap);
	}
}
