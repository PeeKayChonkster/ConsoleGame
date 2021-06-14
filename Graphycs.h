#pragma once
#include "Entity.h"
#include <sstream>



class Graphycs
{
private:
	HANDLE hConsole;
	icoor sCenter;
	std::vector<CHAR_INFO> screen;
	const int sHeight, sWidth;
	int moveTrigX, moveTrigY;
	Map* currMap;
public:
	Graphycs();
	Graphycs(int h, int w);
	int getScreenW() const noexcept;
	int getScreenH() const noexcept;
	void drawLevel(const Map& map);
	void drawPlayer(Player& player);
	void drawEntity(Entity& entity);
	void drawGameInformation(const Player& P);
	void sendScreenToConsole();
	void setMap(Map* nemMap);
	void printOnScreen(icoor coord, std::wstring str);
	bool entityOnTheScreen(Entity& entity) const noexcept;
	std::vector<CHAR_INFO> stringToCharInfo(std::wstring& str) const;
	void changeScreenEffect(Map* newMap);
	void setScreenCenter(icoor newCoor);
	void setScreenCenter(fcoor playerCoor);
};