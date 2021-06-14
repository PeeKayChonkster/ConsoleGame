#include "Entity.h"


///////////////////////////////////// Entity functions /////////////////////////////////////

Entity::Entity(std::wstring n, fcoor c, CHAR_INFO ch, double mass, Map* map) : name(n), ePhysics(c, mass, map), symb(ch) {}


CHAR_INFO Entity::getSymb() const noexcept
{
	return symb;
}

const Map* Entity::getCurrMap() const
{
	return ePhysics.getCurrMap();
}

void Entity::setColor(int col)
{
	symb.Attributes = col;
}

/*bool Entity::entityIsDead() const noexcept
{
	return(ePhysics.isOutOfBounds());					// DEAD if out of bounds
}*/

///////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////// Player stuff  /////////////////////////////////// 
Player::Player(std::wstring n, fcoor c, CHAR_INFO ch, double mass, Map* map) : Entity(n, c, ch, mass, map) {}
/////////////////////////////////////////////////////////////////////////////////
