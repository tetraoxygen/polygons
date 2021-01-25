#include "spaceObject.h"

SpaceObject::SpaceObject() {
	type = ASTEROID;
	radius = 20;
	location = { .x = 0, .y = 0 };
	velocity = { .x = 0, .y = 0 };
	angleDeg = 0;
};

SpaceObject::SpaceObject(SpaceObjType type, double radius, Point location, Point velocity, double angle) {
	this->type = type;
	this->radius = 20;
	this->location = { .x = 0, .y = 0 };
	this->velocity = { .x = 0, .y = 0 };
	this->angleDeg = 0;
	
	this->setRadius(radius);
	this->setLocation(location.x, location.y);
	this->setVelocity(velocity.x, velocity.y);
	this->setAngle(angle);
};

// MARK: - Mutators

bool SpaceObject::setRadius(int radius) {
	if ((radius < (SCREEN_HEIGHT / 2)) && (radius > 0)) {
		this->radius = radius;
		return true;
	} else {
		return false;
	}
}

bool SpaceObject::setLocation(double x, double y) {
	while (x < 0) {
		x = x + SCREEN_WIDTH;
	}
	
	while (x > SCREEN_WIDTH) {
		x = x - SCREEN_WIDTH;
	}
	
	this->location.x = x;
	
	while (y < 0) {
		y = y + SCREEN_HEIGHT;
	}
	
	while (y > SCREEN_HEIGHT) {
		y = y - SCREEN_HEIGHT;
	}
	
	this->location.y = y;

	return true;
}

bool SpaceObject::setVelocity(double velocityX, double velocityY) {
	this->velocity.x = velocityX;
	this->velocity.y = velocityY;
	
	return true;
}

bool SpaceObject::setAngle(double angDeg) {
	this->angleDeg = angDeg;
	return true;
}

void SpaceObject::changeAngle(double deltaDeg) {
	this->setAngle(this->angleDeg + deltaDeg);
}

// MARK: - Accessors

double SpaceObject::getRadius() const {
	return radius;
}

Point SpaceObject::getLocation() const {
	return location;
}

Point SpaceObject::getVelocity() const {
	return velocity;
}

double SpaceObject::getAngle() const {
	return angleDeg;
}

// MARK: - Others

void SpaceObject::updatePosition() {
	this->setLocation((this->velocity.x + this->location.x), (this->velocity.y + this->location.y));
}