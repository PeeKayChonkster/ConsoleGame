#pragma once
#include "Controls.h"
#include <Windows.h>
#include <math.h>
#include <fstream>


#define makeCoor(x, y) ((y) * levW + x)

const int maxLevH = 500;
const int maxLevW = 500;


//////////////// Tile names //////////////////
#define SOLID_WALL {'#', int(colors::Gray)}
#define EMPTY_SPACE {'.', int(colors::Black)}


/////////////////////////////////////////////

enum class colors {
	Black, Blue, Green, Aqua, Red, Purple, Yellow, White, Gray, LightBlue,
	LightGreen, LightAqua, LightRed, LightPurple, LightYellow, BrightWhite
};


/////////////// Class icoor /////////////////////
class icoor
{
public:
	int X;
	int Y;
	icoor() noexcept;
	icoor(int x, int y) noexcept;
	icoor(double x, double y) noexcept;
};
///////////////////////////////////////////////////


//////////////////// Class Map ////////////////////////

class Map
{
private:
	std::vector<CHAR_INFO> levMap;
	int levH, levW;
	std::wstring name;
	std::wstring nameLEFT;
	std::wstring nameRIGHT;
	std::wstring nameUP;
	std::wstring nameDOWN;
	int background;
public:
	Map(int levH, int levW);
	Map(const Map&) = delete;
	void fillSymb(icoor c, CHAR_INFO symb);
	std::vector<CHAR_INFO> makeScreenString(icoor c_center, int s_height, int s_width) const;
	int getLevH() const noexcept;
	int getLevW() const noexcept;
	CHAR_INFO getTile(icoor coord) const;
	bool diskIn(std::wstring levName);
	CHAR_INFO charToCharInfo(wchar_t ch) const noexcept;
	std::wstring getName() const noexcept;
	std::wstring getNameRIGHT() const noexcept;
	std::wstring getNameLEFT() const noexcept;
	std::wstring getNameUP() const noexcept;
	std::wstring getNameDOWN() const noexcept;
	int getBackground() const noexcept;
};
/////////////////////////////////////////////////////////////////

