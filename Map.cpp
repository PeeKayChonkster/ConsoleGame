#include "Map.h"




////////////////////// icoor stuff ///////////////////////////////

icoor::icoor() noexcept : X(0), Y(0) {}

icoor::icoor(int x, int y) noexcept : X(x), Y(y) {}

icoor::icoor(double x, double y) noexcept : X(x), Y(y) {}

/////////////////////////////////////////////////////////////////


////////////////////// Map stuff //////////////////////////////////

Map::Map(int levH, int levW) : levH(levH), levW(levW)
{
	levMap.resize(levH * levW);
	for (int j = 0; j < levH; j++)
		for (int i = 0; i < levW; i++)
		{
			if (j >= 0 && j <= 2 || i >= 0 && i <= 2 || j <= levH - 1 && j >= levH - 1 - 2 || i <= levW - 1 && i >= levW - 1 - 2)
			{
				levMap[makeCoor(i, j)] = SOLID_WALL;
			}
			else
			{
				levMap[makeCoor(i, j)] = EMPTY_SPACE;
			}
		}
}

void Map::fillSymb(icoor c, CHAR_INFO symb)
{
	levMap[makeCoor(c.X, c.Y)] = symb;
}

std::vector<CHAR_INFO> Map::makeScreenString(icoor c_center, int s_height, int s_width) const
{
	if (s_height > levH || s_width > levW) THROW_CGEXCEPTION(L"MakeStringScreen Exception. Dimensions of the map smaller, than the screen.");
	std::vector<CHAR_INFO> screenStr;
	for (int j = 0; j < s_height; j++)
		for (int i = 0; i < s_width; i++)
			screenStr.push_back(levMap[makeCoor(c_center.X + i, c_center.Y + j)]);
	return screenStr;
}

int Map::getLevH() const noexcept
{	
	return levH;
}

int Map::getLevW() const noexcept
{
	return levW;
}

CHAR_INFO Map::getTile(icoor coord) const
{
	return levMap[makeCoor(coord.X, coord.Y)];
}


bool Map::diskIn(std::wstring levName)
{
	std::wifstream file;
	wchar_t buf;
	std::wstring newName;
	std::wstring newNameLEFT;
	std::wstring newNameRIGHT;
	std::wstring newNameUP;
	std::wstring newNameDOWN;
	std::string newBackground;
	bool nameFlag = false;
	int levelW = 0, levelH = 0;
	int count = 0;
	file.open(levName.c_str(), std::ios::app);
	if (!file.good()) THROW_CGEXCEPTION(L"Map disk reading exception. Couldn't open the file.");
	levMap.clear();
	while (true)
	{
		buf = file.get();
		if (buf == '%') 						// if buf == % -name ahead
		{
			nameFlag = true;
			buf = file.get();
		}
		switch (buf)
		{
		case'N':								//name of the current map
			buf = file.get();						
			while (nameFlag)
			{
				newName.push_back(buf);
				buf = file.get();
				if (buf == '%')					// if buf == % again - end of name
				{
					nameFlag = false;
					if (file.peek() == '\n') file.ignore(1);
				}
			}
			continue;
			break;
		case'L':									//name of the left map
			buf = file.get();
			while (nameFlag)
			{
				newNameLEFT.push_back(buf);
				buf = file.get();
				if (buf == '%')					// if buf == % again - end of name
				{
					nameFlag = false;
					if (file.peek() == '\n') file.ignore(1);
				}
			}
			continue;
			break;
		case'R':									//name of the right map
			buf = file.get();
			while (nameFlag)
			{
				newNameRIGHT.push_back(buf);
				buf = file.get();
				if (buf == '%')						// if buf == % again - end of name
				{
					nameFlag = false;
					if (file.peek() == '\n') file.ignore(1);
				}
			}
			continue;
			break;
		case'B':									//Background int
			buf = file.get();
			while (nameFlag)
			{
				newBackground.push_back(buf);
				buf = file.get();
				if (buf == '%')						// if buf == % again - end of name
				{
					nameFlag = false;
					if (file.peek() == '\n') file.ignore(1);
				}
			}
			continue;
			break;
		case'U':									//name of the upper map
			buf = file.get();
			while (nameFlag)
			{
				newNameUP.push_back(buf);
				buf = file.get();
				if (buf == '%')						// if buf == % again - end of name
				{
					nameFlag = false;
					if (file.peek() == '\n') file.ignore(1);
				}
			}
			continue;
			break;
		case'D':									//name of the lower map
			buf = file.get();
			while (nameFlag)
			{
				newNameDOWN.push_back(buf);
				buf = file.get();
				if (buf == '%')						// if buf == % again - end of name
				{
					nameFlag = false;
					if (file.peek() == '\n') file.ignore(1);
				}
			}
			continue;
			break;
		}
		if (buf == '\n')
		{
			if(levelW == 0)
				levelW = count;
			buf = file.get();
			count++;
		}
		levMap.push_back(charToCharInfo(buf));
		count++;
		if (file.eof()) break;
	}
	levelH = levMap.size() / levelW;
	name = newName;
	nameRIGHT = newNameRIGHT;
	nameLEFT = newNameLEFT;
	nameUP = newNameUP;
	nameDOWN = newNameDOWN;
	background = std::stoi(newBackground);
	levW = levelW;
	levH = levelH;
	return true;
}

CHAR_INFO Map::charToCharInfo(wchar_t ch) const noexcept
{
	switch(ch)
	{
	case '.':
		return CHAR_INFO{ L'.', int(colors::Black)};																//empty
		break;
	case 's':
		return CHAR_INFO{ L's', FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY };	//Sky
		break;
	case '#':
		return CHAR_INFO{ L'#', int(colors::Gray)};																	//wall
		break;
	case 'g':
		return CHAR_INFO{ L'g', int(colors::Gray) | BACKGROUND_INTENSITY };											//ground
		break;
	case '@':
		return CHAR_INFO{ L'@', int(colors::BrightWhite) };															//entity
		break;
	case '*':
		return CHAR_INFO{ L'*', int(colors::White) };																//
		break;
	case '~':
		return CHAR_INFO{ L'~', int(colors::Blue) | BACKGROUND_BLUE};												//water
		break;
	case 't':
		return CHAR_INFO{ L't', int(colors::Gray) | BACKGROUND_INTENSITY };											//tree trunk
		break;
	case 'l':
		return CHAR_INFO{ L'l', int(colors::LightGreen) | BACKGROUND_GREEN};										//leaves
		break;
	default:
		return CHAR_INFO{ L'!', int(colors::Red) };																	//error-type
	}
}

std::wstring Map::getName() const noexcept
{
	return name;
}

std::wstring Map::getNameRIGHT() const noexcept
{
	return nameRIGHT;
}

std::wstring Map::getNameLEFT() const noexcept
{
	return nameLEFT;
}

std::wstring Map::getNameUP() const noexcept
{
	return nameUP;
}

std::wstring Map::getNameDOWN() const noexcept
{
	return nameDOWN;
}

int Map::getBackground() const noexcept
{
	return background;
}
///////////////////////////////////////////////////////////////////////////////