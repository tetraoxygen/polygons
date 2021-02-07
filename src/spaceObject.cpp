/** --------------------------- 
* @file		spaceObject.cpp
* @author	Charlie Welsh
* @version	1.0
*
* CS162-01 - Assignment 2.1
*	Implements the SpaceObject class and its methods.
*
* --------------------------- */

#include "spaceObject.h"

// MARK: - Constructors

// --------------------------- 
SpaceObject::SpaceObject() {
	type = ASTEROID;
	radius = 20;
	location = { .x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2 };
	velocity = { .x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2 };
	angleDeg = 0;
};

// --------------------------- 
SpaceObject::SpaceObject(SpaceObjType type, double radius, Point location, Point velocity, double angle) {
	this->type = type;
	this->radius = 20;
	this->location = { .x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2 };
	this->velocity = { .x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2 };
	this->angleDeg = 0;
	
	this->setRadius(radius);
	this->setLocation(location.x, location.y);
	this->setVelocity(velocity.x, velocity.y);
	this->setAngle(angle);
};

// MARK: - Mutators

// --------------------------- 
bool SpaceObject::setRadius(int radius) {
	if ((radius < (WINDOW_HEIGHT / 2)) && (radius > 0)) {
		this->radius = radius;
		return true;
	} else {
		return false;
	}
}

// --------------------------- 
bool SpaceObject::setLocation(double x, double y) {
	while (x < 0) {
		x = x + WINDOW_WIDTH;
	}
	
	while (x > WINDOW_WIDTH) {
		x = x - WINDOW_WIDTH;
	}
	
	this->location.x = x;
	
	while (y < 0) {
		y = y + WINDOW_HEIGHT;
	}
	
	while (y > WINDOW_HEIGHT) {
		y = y - WINDOW_HEIGHT;
	}
	
	this->location.y = y;

	return true;
}

// --------------------------- 
bool SpaceObject::setVelocity(double velocityX, double velocityY) {
	this->velocity.x = velocityX;
	this->velocity.y = velocityY;
	
	return true;
}

// --------------------------- 
bool SpaceObject::setAngle(double angDeg) {
	this->angleDeg = angDeg;
	return true;
}

// --------------------------- 
void SpaceObject::changeAngle(double deltaDeg) {
	this->setAngle(this->angleDeg + deltaDeg);
}

// MARK: - Accessors

// --------------------------- 
double SpaceObject::getRadius() const {
	return radius;
}

// --------------------------- 
Point SpaceObject::getLocation() const {
	return location;
}

// --------------------------- 
Point SpaceObject::getVelocity() const {
	return velocity;
}

// --------------------------- 
double SpaceObject::getAngle() const {
	return angleDeg;
}

// MARK: - Others

// --------------------------- 
void SpaceObject::updatePosition() {
	this->setLocation((this->velocity.x + this->location.x), (this->velocity.y + this->location.y));
}

// --------------------------- 
void SpaceObject::draw(sf::RenderWindow& win) {
	if (type==SHIP)
	drawShip(win);
	else 
	drawAsteroid(win);
}

// --------------------------- 
void SpaceObject::drawAsteroid(sf::RenderWindow& win) {
	
	// Configure a graphics object to be used for drawing our object
	int points= 7;
	sf::CircleShape shape(radius, points); //radius from our SpaceObject
	
	sf::Vector2f midpoint(radius,radius);
	shape.setOrigin(midpoint);
	
	// Thin white lines, look vaguely like a vector CRT. 
	shape.setFillColor(sf::Color(0, 0, 0)); 
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(255, 255, 255));

	// Apply our object position to the graphics object
	shape.setPosition(location.x, location.y);
	shape.setRotation(angleDeg);

	// Draw the shape to the window
	win.draw(shape);
	
	// Smooth Wrapping
	if ((WINDOW_WIDTH <= location.x + radius) || (0 >= (location.x - radius)) || (WINDOW_HEIGHT <= location.y + radius) || (0 >= (location.y - radius))) { // This is more of a bounding box than a radius, but that's probably for the best. 
		
		// Create a clone of the above CircleShape
		sf::CircleShape shapeDouble(radius, points);
		shapeDouble.setOrigin(midpoint);
		shapeDouble.setFillColor(sf::Color(0, 0, 0)); 
		shapeDouble.setOutlineThickness(1);
		shapeDouble.setOutlineColor(sf::Color(255, 255, 255));
		shapeDouble.setRotation(angleDeg);
		shapeDouble.setPosition(location.x, location.y);

		// X
		if (WINDOW_WIDTH <= location.x + radius) {
			shapeDouble.setPosition(shapeDouble.getPosition().x - WINDOW_WIDTH, shapeDouble.getPosition().y);
		}
		if (0 >= (location.x - radius)) {
			shapeDouble.setPosition(shapeDouble.getPosition().x + WINDOW_WIDTH, shapeDouble.getPosition().y);
		}
		
		// Y
		if ((WINDOW_HEIGHT <= location.y + radius)) {
			shapeDouble.setPosition(shapeDouble.getPosition().x, shapeDouble.getPosition().y - WINDOW_HEIGHT);
		}
		if (0 >= (location.y - radius)) {
			shapeDouble.setPosition(shapeDouble.getPosition().x, shapeDouble.getPosition().y + WINDOW_HEIGHT);
		}
		
		win.draw(shapeDouble);
	}
	
}

// --------------------------- 
void SpaceObject::drawShip(sf::RenderWindow& win) {
	// Draw ship
	sf::ConvexShape shipShape;
	shipShape.setPointCount(3);
	shipShape.setPoint(0, sf::Vector2f(10, 0));
	shipShape.setPoint(1, sf::Vector2f(0, 25));
	shipShape.setPoint(2, sf::Vector2f(20, 25));

	sf::Vector2f midpoint(10,15);
	shipShape.setOrigin(midpoint);

	shipShape.setFillColor(sf::Color(0, 0, 0));
	shipShape.setOutlineThickness(1);
	shipShape.setOutlineColor(sf::Color(255, 255, 255));

	shipShape.setPosition(location.x, location.y);
	shipShape.setRotation(angleDeg);
	win.draw(shipShape);
}

// --------------------------- 
void SpaceObject::applyThrust() {
	double engineThrust = 0.05;
	double forcex = cos((angleDeg-90)*PI/180) * engineThrust;
	double forcey = sin((angleDeg-90)*PI/180) * engineThrust;
	velocity.x = velocity.x + forcex; 
	velocity.y = velocity.y + forcey;
}