#include "Physics.h"

#define MOVE_FORCE_UP "MoveForceUP"
#define MOVE_FORCE_DOWN "MoveForceDOWN"
#define MOVE_FORCE_LEFT "MoveForceLEFT"
#define MOVE_FORCE_RIGHT "MoveForceRIGHT"
#define MOVE_FORCE_JUMP "MoveForceJUMP"
#define MOVE_FORCE_STICKX "MoveForceSTICKX"
#define MOVE_FORCE_STICKY "MoveForceSTICKY"
#define GRAV_FORCE "GravForce"
#define STATIC_FORCE "StaticForce"
#define COLLISION_FORCE "CollisionForce"
#define AIR_FRIC_FORCE "AirFricForce"

extern const int FramesPerSecond;

const static float gravConst = 50.0f;
const float moveConst = 10000.0f;
const float speedDampMin = 40.0f;
const float airFricCoef = 0.1f;
const float bouncinessFloor = 1.0f;
const float bouncinessWalls = 1.4f;
const double dampConst = 5.0;



////////////////////// fcoor stuff ///////////////////////////////

fcoor::fcoor() noexcept : X(0.0), Y(0.0) {}

fcoor::fcoor(double x, double y) noexcept : X(x), Y(y) {}

fcoor::operator icoor() const
{
	return { int(X), int(Y) };
}



////////////////////// Vector stuff ////////////////////////////

Vector::Vector() noexcept : Xcomponent(0.0), Ycomponent(0.0) {}

Vector::Vector(const Vector& other) noexcept
{
	Xcomponent = other.Xcomponent;
	Ycomponent = other.Ycomponent;
}

Vector::Vector(const double X, const double Y) : Xcomponent(X), Ycomponent(Y) {}

Vector Vector::operator-()
{
	return Vector(-Xcomponent, -Ycomponent);
}


Vector operator+(const Vector& V1, const Vector& V2)
{
	return Vector(V1.Xcomponent + V2.Xcomponent, V1.Ycomponent + V2.Ycomponent);
}

Vector operator-(const Vector& V1, const Vector& V2)
{
	return Vector(V1.Xcomponent - V2.Xcomponent, V1.Ycomponent - V2.Ycomponent);
}


Vector operator*(const Vector& V, const double d)
{
	return Vector(V.Xcomponent * d, V.Ycomponent * d);
}

Vector operator*(const Vector& V, const int i)
{
	return Vector(V.Xcomponent * i, V.Ycomponent * i);
}

Vector operator/(const Vector& V, const double d)
{
	return Vector(V.Xcomponent / d, V.Ycomponent / d);
}

Vector Vector::operator+=(const Vector& other)
{
	Xcomponent += other.Xcomponent;
	Ycomponent += other.Ycomponent;
	return *this;
}

void Vector::flip() noexcept
{
	Xcomponent = -Xcomponent;
	Ycomponent = -Ycomponent;
}




////////////////////// Physics stuff ///////////////////////////

Physics::Physics() : mass(0.0), speed(), currMap(nullptr)
{
	for (int i = 0; i < 4; i++)
		collisions[i] = false;
}

Physics::Physics(const fcoor coord, const double m, Map* map) : eCoord(coord), mass(m), speed(), currMap(map)
{
	for (int i = 0; i < 4; i++)
		collisions[i] = false;
}

Physics::Physics(fcoor coord, double m, Vector s, Map* map) : eCoord(coord), mass(m), speed(s), currMap(map)
{
	for (int i = 0; i < 4; i++)
		collisions[i] = false;
}

void Physics::setForce(std::string name,Vector F) noexcept
{
	forces.insert({ name, F });
}

void Physics::unsetForce(std::string name) noexcept
{
	forces.erase(name);
}

void Physics::resetForces() noexcept
{
	forces.clear();
}

void Physics::x_resetForces(std::string name) noexcept		//reset all forces except one
{
	Vector temp = forces[name];
	forces.clear();
	forces.insert({ name, temp });
}

void Physics::setCollision(Direction dir) noexcept
{
	collisions[dir] = true;
}

void Physics::unsetCollision(Direction dir) noexcept
{
	collisions[dir] = false;
}

void Physics::clearCollisions() noexcept
{
	for (int i = 0; i < 4; i++)
		collisions[i] = false;
}

Vector Physics::calcSpeedIncPerFrame() noexcept				//Calculate speed increment since last frame
{
	Vector speedInc = (getForceResultVec() / mass / double(FramesPerSecond));
	return speedInc;
}

void Physics::moveEntityPerFrame() noexcept				//increments displacement based on speed
{
	Vector speedInc = calcSpeedIncPerFrame();
	speed += speedInc;
	dampSpeed();
	Vector displacement;
	displacement = speed / double(FramesPerSecond);
	eCoord.X += displacement.Xcomponent;
	eCoord.Y += displacement.Ycomponent;
}

fcoor Physics::getCoordinates() const noexcept
{
	return eCoord;
}

void Physics::checkIfCollision() noexcept
{
	float XcollisionCoef = 0.9;		// how far collision checks
	float YcollisionCoef = 0.9;
	wchar_t tileUP = currMap->getTile(icoor(eCoord.X, eCoord.Y - YcollisionCoef)).Char.UnicodeChar;
	wchar_t tileDOWN = currMap->getTile(icoor(eCoord.X, eCoord.Y + YcollisionCoef)).Char.UnicodeChar;
	wchar_t tileLEFT = currMap->getTile(icoor(eCoord.X - XcollisionCoef, eCoord.Y)).Char.UnicodeChar;
	wchar_t tileRIGHT = currMap->getTile(icoor(eCoord.X + XcollisionCoef, eCoord.Y)).Char.UnicodeChar;
	if (abs(speed.Xcomponent) > 60.0) XcollisionCoef = 0.99 * int(abs(speed.Xcomponent) / FramesPerSecond);	// coefficient grows with speed accordingly
	if (abs(speed.Ycomponent) > 60.0) YcollisionCoef = 0.99 * int(abs(speed.Ycomponent) / FramesPerSecond);
	if (tileUP == '#' || tileUP == 'g')
	{
		collisions[UP] = true;
	}
	else
	{
		collisions[UP] = false;
	}
	if (tileDOWN == '#' || tileDOWN == 'g')
	{
		collisions[DOWN] = true;
	}
	else
	{
		collisions[DOWN] = false;
	}
	if (tileLEFT == '#' || tileLEFT == 'g')
	{
		collisions[LEFT] = true;
	}
	else
	{
		collisions[LEFT] = false;
	}
	if (tileRIGHT == '#' || tileRIGHT == 'g')
	{
		collisions[RIGHT] = true;
	}
	else
	{
		collisions[RIGHT] = false;
	}
	
}

void Physics::setCollisionForces() noexcept
{
	const double correctionFloor = bouncinessFloor;				//bounciness
	const double correctionWalls = bouncinessWalls;
	for (int i = 0; i < 4; i++)
	{
		if (collisions[i])
		{
			Vector collisionForce;
			Vector staticForce;
			switch (i)
			{
			case 0:
				if (forces[MOVE_FORCE_UP].Ycomponent < 0.0)								//force is up with keyboard
				{
					staticForce.Ycomponent = -forces[MOVE_FORCE_UP].Ycomponent;
				}
				if (forces[MOVE_FORCE_STICKY].Ycomponent < 0.0)							//force is up with stick
				{
					staticForce.Ycomponent = -forces[MOVE_FORCE_STICKY].Ycomponent;
				}
				if (forces[MOVE_FORCE_JUMP].Ycomponent < 0.0)
				{
					staticForce.Ycomponent = -forces[MOVE_FORCE_JUMP].Ycomponent;
				}
				if (speed.Ycomponent < 0.0)
				{
					collisionForce.Ycomponent = -(speed.Ycomponent * mass * double(FramesPerSecond) * correctionWalls);
					break;
				}
				break;
			case 1:
				staticForce.Ycomponent = -forces[GRAV_FORCE].Ycomponent;
				if (forces[MOVE_FORCE_DOWN].Ycomponent > 0.0)								//force is down with keyboard
				{
					staticForce.Ycomponent += -forces[MOVE_FORCE_DOWN].Ycomponent;
				}
				if (forces[MOVE_FORCE_STICKY].Ycomponent > 0.0)								//force is down with stick
				{
					staticForce.Ycomponent += -forces[MOVE_FORCE_STICKY].Ycomponent;
				}
				if (speed.Ycomponent > 0.0)
				{
					collisionForce.Ycomponent = -(speed.Ycomponent * mass * double(FramesPerSecond) * correctionFloor);
					break;
				}
				break;
			case 2:
				if (forces[MOVE_FORCE_LEFT].Xcomponent < 0.0)								//force is left with keyboard
				{
					staticForce.Xcomponent = -forces[MOVE_FORCE_LEFT].Xcomponent;
				}
				if (forces[MOVE_FORCE_STICKX].Xcomponent < 0.0)								//force is left with stick
				{
					staticForce.Xcomponent = -forces[MOVE_FORCE_STICKX].Xcomponent;
				}
				if (speed.Xcomponent < 0.0)
				{
					collisionForce.Xcomponent = -(speed.Xcomponent * mass * double(FramesPerSecond) * correctionWalls);
					break;
				}
				break;
			case 3:
				if (forces[MOVE_FORCE_RIGHT].Xcomponent > 0.0)								//force is right with keyboard
				{
					staticForce.Xcomponent = -forces[MOVE_FORCE_RIGHT].Xcomponent;
				}
				if (forces[MOVE_FORCE_STICKX].Xcomponent > 0.0)								//force is right with stick
				{
					staticForce.Xcomponent = -forces[MOVE_FORCE_STICKX].Xcomponent;
				}
				if (speed.Xcomponent > 0.0)
				{
					collisionForce.Xcomponent = -(speed.Xcomponent * mass * double(FramesPerSecond) * correctionWalls);
					break;
				}
				break;
			}
			if (staticForce.Xcomponent != 0.0 || staticForce.Ycomponent != 0.0)
			{
				std::string nameSt = STATIC_FORCE;
				nameSt.push_back(i + 48);
				setForce(nameSt, staticForce);
			}
			if (collisionForce.Xcomponent != 0.0 || collisionForce.Ycomponent != 0.0)
			{
				std::string nameCol = COLLISION_FORCE;
				nameCol.push_back(i + 48);
				setForce(nameCol, collisionForce);
			}
		}
	}
}

const Map* Physics::getCurrMap() const noexcept
{
	return currMap;
}

Vector Physics::getSpeed() const noexcept
{
	return speed;
}

void Physics::dampSpeed()
{
	static int Xcount = 0, Ycount = 0;
	if (!wantsToMove() && abs(getSpeed().Xcomponent) < speedDampMin)     //if entity wants to move on X axis
	{
		Xcount++;
		speed.Xcomponent = speed.Xcomponent / Xcount;
	}
	else
		Xcount = 0;
	/*if (!getForce("MoveForce0").has_value() && !getForce("MoveForce1").has_value() && !getForce("MoveForce4").has_value() && abs(getSpeed().Ycomponent) < speedDampMin)  //if entity wants to move on Y axis
	{
		Ycount++;
		speed.Ycomponent = speed.Ycomponent / Ycount;
	}
	else
		Ycount = 0;*/
}

Vector Physics::getForceResultVec() noexcept
{
	Vector forceResult;
	std::map<std::string, Vector>::iterator iter;
	for (iter = forces.begin(); iter != forces.end(); iter++)
	{
		forceResult += iter->second;
	}
	return forceResult;
}

std::optional<Vector> Physics::getForce(std::string forceName) noexcept
{
	if (forces.find(forceName) != forces.end())
		return forces[forceName];
	else
		return {};
}

void Physics::assignAirFricForces() noexcept
{
	Vector eSpeed = getSpeed();
	if (eSpeed.Xcomponent != 0.0 || eSpeed.Ycomponent != 0.0)
	{
		Vector airFricForce;
		if (currMap->getTile(eCoord).Char.UnicodeChar == '~')			//if in water, 2 times friction
		{
			airFricForce.Xcomponent = -pow(2.5 * eSpeed.Xcomponent, 3) * airFricCoef;
			airFricForce.Ycomponent = -pow(2.5 * eSpeed.Ycomponent, 3) * airFricCoef;
		}
		else
		{
			airFricForce.Xcomponent = -pow(eSpeed.Xcomponent, 3) * airFricCoef;
			airFricForce.Ycomponent = -pow(eSpeed.Ycomponent, 3) * airFricCoef;
		}
		setForce(AIR_FRIC_FORCE, airFricForce);
	}
}


void Physics::assignMoveForces(std::vector<Action> direction)			//for keyboard
{
	Vector moveForce;
	while (!direction.empty())
	{
		switch (direction.back())
		{
		case MOVE_UP:
			moveForce.Xcomponent = 0.0;
			moveForce.Ycomponent = -moveConst;
			setForce(MOVE_FORCE_UP, moveForce / 10.0);
			break;
		case MOVE_DOWN:
			moveForce.Xcomponent = 0.0;
			moveForce.Ycomponent = moveConst;
			setForce(MOVE_FORCE_DOWN, moveForce / 10.0);
			break;
		case MOVE_LEFT:
			moveForce.Xcomponent = -moveConst;
			moveForce.Ycomponent = 0.0;
			setForce(MOVE_FORCE_LEFT, moveForce);
			break;
		case MOVE_RIGHT:
			moveForce.Xcomponent = moveConst;
			moveForce.Ycomponent = 0.0;
			setForce(MOVE_FORCE_RIGHT, moveForce);
			break;
		case MOVE_JUMP:
			if (collisions[DOWN])
			{
				moveForce.Xcomponent = 0.0;
				moveForce.Ycomponent = -jumpConst;
				setForce(MOVE_FORCE_JUMP, moveForce);
			}
			break;
		}
		direction.pop_back();
	}
}

void Physics::assignMoveForces(std::map<Action, double> directionsGamepad)		//for gamepad
{
	Vector moveForce;
	std::map<Action, double>::iterator iter = directionsGamepad.begin();
	while (!directionsGamepad.empty())
	{
		switch (iter->first)
		{
		case MOVE_JUMP:
			if (collisions[DOWN])
			{
				moveForce.Xcomponent = 0.0;
				moveForce.Ycomponent = -jumpConst;
				setForce(MOVE_FORCE_JUMP, moveForce);
			}
			break;
		case STICK_MOVE_Y:
			/*moveForce.Xcomponent = 0.0;
			moveForce.Ycomponent = -mapValue(iter->second, -32768, 32767, -moveConst, moveConst);
			setForce(MOVE_FORCE_STICKY, moveForce / 10.0);*/
			break;
		case STICK_MOVE_X:
			moveForce.Xcomponent = mapValue(iter->second, -32768, 32767, -moveConst, moveConst);
			moveForce.Ycomponent = 0.0;
			setForce(MOVE_FORCE_STICKX, moveForce);
			break;
		}
		directionsGamepad.erase((iter++)->first);
	}
}

void Physics::assignGravForce()
{
	Vector gravForce(0.0, gravConst * mass);
	setForce(GRAV_FORCE, gravForce);
}

bool Physics::isOutOfBounds() const noexcept
{
	return (eCoord.X > double(currMap->getLevW() - 1) || eCoord.X < 0.0 ||
		eCoord.Y > double(currMap->getLevH() - 1) || eCoord.Y < 0.0);
}

void Physics::calculateMovement()
{
	checkIfCollision();		//assign collision flags
	setCollisionForces();	//assign forces based on collision flags
	assignAirFricForces();
	moveEntityPerFrame();	//assign speed increment and move entity
	resetForces();			//reset all forces
}

double Physics::mapValue(double number, double in1Beg, double in1End, double in2Beg, double in2End)
{
	if (in1Beg == in1End || in2Beg == in2End) THROW_CGEXCEPTION(L"Controls mapping exception. Interval = 0.");
	double interval1 = in1End - in1Beg;
	double interval2 = in2End - in2Beg;
	return(in2Beg + abs((number - in1Beg) / interval1) * interval2);
}

bool Physics::wantsToMove() noexcept
{
	return(getForce(MOVE_FORCE_LEFT).has_value() || getForce(MOVE_FORCE_RIGHT).has_value()
		|| getForce(MOVE_FORCE_STICKX).has_value());
}

void Physics::setCurrMap(const Map* newMap, fcoor newCoor)
{
	currMap = newMap;
	eCoord.X = newCoor.X;
	eCoord.Y = newCoor.Y;
}