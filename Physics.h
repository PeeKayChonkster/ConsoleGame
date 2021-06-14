#pragma once
#include "Map.h"
#include <optional>




enum Direction { UP, DOWN, LEFT, RIGHT};




class fcoor
{
public:
	double X;
	double Y;
	fcoor() noexcept;
	fcoor(double x, double y) noexcept;
	operator icoor() const;
};



class Vector
{
public:
	double Xcomponent;
	double Ycomponent;

	Vector() noexcept;
	Vector(const Vector& other) noexcept;
	Vector(const double X, const double Y);
	Vector operator-();
	friend Vector operator+(const Vector& V1, const Vector& V2);
	friend Vector operator-(const Vector& V1, const Vector& V2);
	friend Vector operator*(const Vector& V, const double d);
	friend Vector operator/(const Vector& V, const double d);
	friend Vector operator*(const Vector& V, const int i);
	Vector operator+=(const Vector& other);
	void flip() noexcept;
};





///////////////////////////// Class Physics /////////////////////////////////////

class Physics
{
private:
	std::map<std::string, Vector> forces;
	fcoor eCoord;
	fcoor prevCoor;
	const double mass;
	Vector speed;
	bool collisions[4];
	const Map* currMap;
public:
	friend class Entity;
	Physics();
	Physics(const Physics&) = delete;
	Physics(const fcoor coord, const double m, Map* map);
	Physics(fcoor coord,double m, Vector s, Map* map);
	void setForce(std::string name ,Vector F) noexcept;
	void unsetForce(std::string name) noexcept;
	void resetForces() noexcept;
	void x_resetForces(std::string name) noexcept;		//clear all forces except of one
	void setCollision(Direction dir) noexcept;
	void unsetCollision(Direction dir) noexcept;
	void clearCollisions() noexcept;
	Vector calcSpeedIncPerFrame() noexcept;
	void moveEntityPerFrame() noexcept;
	fcoor getCoordinates() const noexcept;
	void checkIfCollision() noexcept;
	void setCollisionForces() noexcept;
	const Map* getCurrMap() const noexcept;
	Vector getSpeed() const noexcept;
	void dampSpeed();
	Vector getForceResultVec() noexcept;
	void assignAirFricForces() noexcept;
	std::optional<Vector> getForce(std::string forceName) noexcept;
	void calculateMovement();
	void assignMoveForces(std::vector<Action> directions);
	void assignMoveForces(std::map<Action, double> directionsGamepad);
	void assignGravForce();
	bool isOutOfBounds() const noexcept;
	double mapValue(double number, double interv1Beg, double interv1End, double interval2Beg, double interval2End);
	bool wantsToMove() noexcept;
	void setCurrMap(const Map* newMap, fcoor newCoord);
};


							/*

/////////////////////////// Exception //////////////////////////

class vecExcept : public std::exception
{
public:
	int line;
	std::string file;
	vecExcept(int line, std::string file) : line(line), file(file) {}
};
							*/