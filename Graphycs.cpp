#include "Graphycs.h"
#include <ctime>



Graphycs::Graphycs() : sCenter({ 0, 0 }), sHeight(40), sWidth(120)
{
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	moveTrigX = (50.0 / 120.0) * sWidth;				//auto-adjustable camera triggers
	moveTrigY = (15.0 / 40.0) * sHeight;
	screen.resize(sHeight * sWidth);
}

Graphycs::Graphycs(int h, int w) : sCenter({ 0, 0 }), sHeight(h), sWidth(w)
{
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	moveTrigX = (50.0 / 120.0) * sWidth;
	moveTrigY = (15.0 / 40) * sHeight;
	screen.resize(sHeight * sWidth);
}

int Graphycs::getScreenW() const noexcept
{
	return sWidth;
}

int Graphycs::getScreenH() const noexcept
{
	return sHeight;
}

void Graphycs::drawLevel(const Map& map)
{
	std::vector<CHAR_INFO> S = map.makeScreenString(sCenter, sHeight, sWidth);
	screen = S;
}

void Graphycs::drawPlayer(Player& player)
{
	int screenPlayerX = int(player.ePhysics.getCoordinates().X - sCenter.X);
	int screenPlayerY = int(player.ePhysics.getCoordinates().Y - sCenter.Y);
	if (screenPlayerX > sWidth || screenPlayerX < 0.0 || screenPlayerY > sHeight || screenPlayerY < 0.0) THROW_CGEXCEPTION(L"Graphycs exception. Player is out of camera reach.");

	if (screenPlayerX <= moveTrigX && sCenter.X > 0)
		sCenter.X--;												//move camera on X axis
	if ((sWidth - screenPlayerX) <= moveTrigX && sWidth < (currMap->getLevW() - sCenter.X))
		sCenter.X++;
	if (screenPlayerY <= moveTrigY && sCenter.Y > 0)
		sCenter.Y--;													//move camera on Y axis
	if ((sHeight - screenPlayerY) <= moveTrigY && sHeight < (currMap->getLevH() - sCenter.Y))
		sCenter.Y++;
	screen[screenPlayerY * sWidth + screenPlayerX] = player.getSymb();

}

void Graphycs::drawEntity(Entity& entity)
{
	if (entityOnTheScreen(entity))
	{
		int screenEntityX = (entity.ePhysics.getCoordinates().X - sCenter.X);
		int screenEntityY = (entity.ePhysics.getCoordinates().Y - sCenter.Y);
		screen[screenEntityY * sWidth + screenEntityX] = entity.getSymb();
	}
}


void Graphycs::sendScreenToConsole()
{
	SHORT bufCoorX = sWidth;
	SHORT bufCoorY = sHeight;
	SMALL_RECT rect = { 0, 0, sWidth, sHeight };
	SetConsoleCursorPosition(hConsole, { -1, 0 });
	WriteConsoleOutput(hConsole, screen.data(), { bufCoorX, bufCoorY }, { 0, 0 }, &rect);
}

void Graphycs::setMap(Map* newMap)
{
	currMap = newMap;
}

void Graphycs::drawGameInformation(const Player& P)
{
	std::wstring bufCoor;
	std::wstring bufSpeed;
	std::wstringstream oss1, oss2;
	oss1.precision(2);
	oss1.setf(std::ios::fixed);
	oss2.precision(2);
	oss2.setf(std::ios::fixed);
	oss1 << '(' << P.ePhysics.getCoordinates().X << "/" << P.ePhysics.getCoordinates().Y << ") <-coord";
	bufCoor = oss1.str().c_str();
	oss2 << '(' << P.ePhysics.getSpeed().Xcomponent << "/" << P.ePhysics.getSpeed().Ycomponent << ") <-speed";
	bufSpeed = oss2.str();
	printOnScreen(icoor(3, 3), oss1.str());
	printOnScreen(icoor(3, 4), oss2.str());
}

void Graphycs::printOnScreen(icoor coord, std::wstring str)
{
	DWORD bul;
	SHORT bufCoorX = coord.X;
	SHORT bufCoorY = coord.Y;
	std::vector<CHAR_INFO> buf = stringToCharInfo(str);
	for (int i = 0; i < buf.size(); i++)
	{
		screen[bufCoorY * sWidth + bufCoorX + i] = buf[i];
	}
}

bool Graphycs::entityOnTheScreen(Entity& entity) const noexcept
{
	return ((entity.ePhysics.getCoordinates().X >= sCenter.X && entity.ePhysics.getCoordinates().X <= (sCenter.X + sWidth)) && (entity.ePhysics.getCoordinates().Y >= sCenter.Y && entity.ePhysics.getCoordinates().Y <= (sCenter.Y + sHeight)));
}

std::vector<CHAR_INFO> Graphycs::stringToCharInfo(std::wstring& str) const
{
	std::vector<CHAR_INFO> buf;
	for (int i = 0; i < str.size(); i++)
	{
		buf.push_back(CHAR_INFO{ str.c_str()[i], int(colors::White) });
	}
	return buf;
}

void Graphycs::changeScreenEffect(Map* newMap)
{
	int count = 0;
	for (int i = 0; i < screen.size(); i++)
	{
		screen[i] = { '$', int(colors::White) };
		count++;
		if (count == 16)
		{
			sendScreenToConsole();
			count = 0;
		}
	}
	count = 0;
	std::vector<CHAR_INFO> buf = newMap->makeScreenString(sCenter, sHeight, sWidth);
	for (int i = 0; i < screen.size(); i++)
	{
		screen[i] = buf[i];
		count++;
		if (count == 16)
		{
			sendScreenToConsole();
			count = 0;
		}
	}
}

void Graphycs::setScreenCenter(icoor newCoor)
{
	sCenter = newCoor;
}

void Graphycs::setScreenCenter(fcoor playerCoor)
{

	if (playerCoor.X < moveTrigX && currMap->getLevH() - playerCoor.Y < moveTrigY)  //if lower left corner
	{
		sCenter = { 0,currMap->getLevH() - sHeight };
	} else
	if (playerCoor.X < moveTrigX && playerCoor.Y < moveTrigY)						//if upper left corner
	{
		sCenter = { 0,0 };
	} else
	if (currMap->getLevW() - playerCoor.X < moveTrigX && playerCoor.Y < moveTrigY)	//if upper right corner
	{
		sCenter = { currMap->getLevW() - sWidth, 0 };
	} else
	if (currMap->getLevW() - playerCoor.X < moveTrigX && currMap->getLevH() - playerCoor.Y < moveTrigY) //if lower right corner
	{
		sCenter = { currMap->getLevW() - sWidth, currMap->getLevH() - sHeight };
	} else
	if(playerCoor.X < moveTrigX)											//if not corner, from the left
	{
		if (currMap->getLevH() - playerCoor.Y <= sHeight - moveTrigY)	//if more to the down
		{
			sCenter = { 0,int(playerCoor.Y - (sHeight - moveTrigY)) };
		}
		else
		if(playerCoor.Y <= sHeight - moveTrigY)							//if more to the up
		{
			sCenter = { 0, int(playerCoor.Y - moveTrigY) };
		}
		else															//if more to the center
		{
			sCenter = { 0,int(playerCoor.Y - (sHeight / 2)) };			
		}
	} else
	if (currMap->getLevW() - playerCoor.X < moveTrigX)						//if not corner, from the right
	{
		if (currMap->getLevH() - playerCoor.Y <= sHeight - moveTrigY)	//if more to the down
		{
			sCenter = { currMap->getLevW() - sWidth,int(playerCoor.Y - (sHeight - moveTrigY)) };
		}
		else
		if (playerCoor.Y <= sHeight - moveTrigY)						//if more to the up
		{
				sCenter = { currMap->getLevW() - sWidth, int(playerCoor.Y - moveTrigY) };
		}
		else															//if more to the center
		{
				sCenter = { currMap->getLevW() - sWidth,int(playerCoor.Y - (sHeight / 2)) };
		}
	} else
	if (playerCoor.Y < moveTrigY)											//if not corner, from the up
	{
		if (currMap->getLevW() - playerCoor.X <= sWidth - moveTrigX)	//more to the right
		{
			sCenter = { int(playerCoor.X - (sWidth - moveTrigX)), 0 };
		}
		else
		if (playerCoor.X <= sWidth - moveTrigX)							//more to the left
		{
				sCenter = { int(playerCoor.X - moveTrigX), 0 };
		}
		else															//more to the center
		{
				sCenter = { int(playerCoor.X - (sWidth / 2)), 0 };
		}
	} else
	if (currMap->getLevH() - playerCoor.Y < moveTrigY)						//if not corner, from the down
	{
		if (currMap->getLevW() - playerCoor.X <= sWidth - moveTrigX)	//if more to the right
		{
			sCenter = { int(playerCoor.X - (sWidth - moveTrigX)), currMap->getLevH() - sHeight};
		}
		else
		if (playerCoor.X <= sWidth - moveTrigX)							//if more to the left
		{
			sCenter = { int(playerCoor.X - moveTrigX), currMap->getLevH() - sHeight};
		}
		else															//if more to the center
		{
			sCenter = { int(playerCoor.X - (sWidth / 2)), currMap->getLevH() - sHeight};
		}
	}
	else																//if not corner, center
	{
		sCenter = { playerCoor.X - sWidth, playerCoor.Y - sHeight };
	}
}



////////////////////////////////////////////////////////////////////////////////////