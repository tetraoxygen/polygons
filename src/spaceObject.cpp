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
#include <cmath>

// MARK: - Constructors

// --------------------------- 
SpaceObject::SpaceObject() {
	type = ASTEROID;
	radius = 20;
	position = { .x = WINDOW_WIDTH / 2.0, .y = WINDOW_HEIGHT / 2.0 };
	velocity = { .x = WINDOW_WIDTH / 2.0, .y = WINDOW_HEIGHT / 2.0 };
	angleDeg = 0;
};

// --------------------------- 
SpaceObject::SpaceObject(SpaceObjType type, double radius, Point position, Point velocity, double angle) {
	this->type = type;
	this->radius = 20;
	this->position = { .x = WINDOW_WIDTH / 2.0, .y = WINDOW_HEIGHT / 2.0 };
	this->velocity = { .x = WINDOW_WIDTH / 2.0, .y = WINDOW_HEIGHT / 2.0 };
	this->angleDeg = 0;
	
	this->setRadius(radius);
	this->setPosition(position.x, position.y);
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
bool SpaceObject::setPosition(double x, double y) {
	while (x < 0) {
		x = x + WINDOW_WIDTH;
	}
	
	while (x > WINDOW_WIDTH) {
		x = x - WINDOW_WIDTH;
	}
	
	this->position.x = x;
	
	while (y < 0) {
		y = y + WINDOW_HEIGHT;
	}
	
	while (y > WINDOW_HEIGHT) {
		y = y - WINDOW_HEIGHT;
	}
	
	this->position.y = y;

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
	if (type == SHIP) {
		this->setAngle(this->angleDeg + deltaDeg);
	}
}

// MARK: - Accessors

// --------------------------- 
double SpaceObject::getRadius() const {
	return radius;
}

// --------------------------- 
Point SpaceObject::getPosition() const {
	return position;
}

// --------------------------- 
Point SpaceObject::getVelocity() const {
	return velocity;
}

// --------------------------- 
double SpaceObject::getAngle() const {
	return angleDeg;
}

// --------------------------- 
SpaceObjType SpaceObject::getType() const {
	return type;
}

bool SpaceObject::isInCenter() const {
	Point centerPosition = Point { .x = WINDOW_WIDTH / 2.0, .y = WINDOW_HEIGHT / 2.0 };
	double centerRadius = 100;
	
	double radiiSum = centerRadius + radius;
	
	double xDistance = abs(position.x - centerPosition.x);
	double yDistance = abs(position.y - centerPosition.y);

	double totalDistance = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
	return radiiSum > totalDistance;
}

// MARK: - Others

// --------------------------- 
void SpaceObject::updatePosition() {
	this->setPosition((this->velocity.x + this->position.x), (this->velocity.y + this->position.y));
}

// --------------------------- 
void SpaceObject::applyThrust(double thrustVector) {
	if (type == SHIP) {
		double forceX = cos((angleDeg-90)*PI/180) * thrustVector;
		double forceY = sin((angleDeg-90)*PI/180) * thrustVector;
		velocity.x = velocity.x + forceX; 
		velocity.y = velocity.y + forceY;
	}
}

// --------------------------- 
void SpaceObject::explode() {
	this->setVelocity(0, 0);
	this->type = SHIP_EXPLODING;
}

// --------------------------- 
void SpaceObject::draw(sf::RenderWindow& win) {
	if (type==SHIP) {
		drawShip(win);
	} else if (type == SHIP_EXPLODING) {
		drawExplodingShip(win);
	} else {
		drawAsteroid(win);
	}
}

// --------------------------- 
void SpaceObject::drawAsteroid(sf::RenderWindow& win) {
	// Configure a graphics object to be used for drawing our object
	int points= 7;
	sf::CircleShape shape(radius, points); //radius from our SpaceObject
	
	sf::Vector2f midpoint(radius,radius);
	shape.setOrigin(midpoint);
	
	// Thin white lines, which look vaguely like a vector CRT. 
	shape.setFillColor(sf::Color(0, 0, 0)); 
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(255, 255, 255));

	// Apply our object position to the graphics object
	shape.setPosition(position.x, position.y);
	shape.setRotation(angleDeg);

	// Draw the shape to the window
	win.draw(shape);
	
	// Smooth Wrapping
	if ((WINDOW_WIDTH <= position.x + radius) || (0 >= (position.x - radius)) || (WINDOW_HEIGHT <= position.y + radius) || (0 >= (position.y - radius))) { // This is more of a bounding box than a radius, but that's probably for the best. 
		
		// Create a clone of the above CircleShape
		sf::CircleShape shapeDouble(radius, points);
		shapeDouble.setOrigin(midpoint);
		shapeDouble.setFillColor(sf::Color(0, 0, 0)); 
		shapeDouble.setOutlineThickness(1);
		shapeDouble.setOutlineColor(sf::Color(255, 255, 255));
		shapeDouble.setRotation(angleDeg);
		shapeDouble.setPosition(position.x, position.y);

		// X
		if (WINDOW_WIDTH <= position.x + radius) {
			shapeDouble.setPosition(shapeDouble.getPosition().x - WINDOW_WIDTH, shapeDouble.getPosition().y);
		}
		if (0 >= (position.x - radius)) {
			shapeDouble.setPosition(shapeDouble.getPosition().x + WINDOW_WIDTH, shapeDouble.getPosition().y);
		}
		
		// Y
		if ((WINDOW_HEIGHT <= position.y + radius)) {
			shapeDouble.setPosition(shapeDouble.getPosition().x, shapeDouble.getPosition().y - WINDOW_HEIGHT);
		}
		if (0 >= (position.y - radius)) {
			shapeDouble.setPosition(shapeDouble.getPosition().x, shapeDouble.getPosition().y + WINDOW_HEIGHT);
		}
		
		win.draw(shapeDouble);
	}
	
}

// --------------------------- 
void SpaceObject::drawShip(sf::RenderWindow& win) {
	if (type == SHIP){
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
	
		shipShape.setPosition(position.x, position.y);
		shipShape.setRotation(angleDeg);
		win.draw(shipShape);
	} else if (type == SHIP_EXPLODING) {
		drawExplodingShip(win);
	}
}

// --------------------------- 
void SpaceObject::drawExplodingShip(sf::RenderWindow& win) {
	if (type != SHIP_GONE){
		// Configure a graphics object to be used for drawing our object
		int points = 64;
		sf::CircleShape shape(radius, points); //radius from our SpaceObject
		
		sf::Vector2f midpoint(radius,radius);
		shape.setOrigin(midpoint);
		
		// Thin white lines, which look vaguely like a vector CRT. 
		shape.setFillColor(sf::Color(0, 0, 0)); 
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(255, 255, 255));
	
		// Apply our object position to the graphics object
		shape.setPosition(position.x, position.y);
		shape.setRotation(angleDeg);
	
		// Draw the shape to the window
		win.draw(shape);
		
		// Expand the circle.
		setRadius(getRadius() + 1.5);
		
		if (radius >= 80) {
			type = SHIP_GONE;
		}
	}
}