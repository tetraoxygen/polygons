/** --------------------------------------------------------
* @file		spaceObjectTest.cpp
* @author	Charlie Welsh
* @version	1.0
*
* CS162-01 - Assignment 2.1
*	Tests the SpaceObject class, printing the results to stdout.
*
* ------------------------------------------------------------ */

#include "spaceObject.cpp"
#include <iostream>

void printValues(SpaceObject obj);

int main() {

	// Instantiate SpaceObject (without constructor)
	SpaceObject asteroid;
	printValues(asteroid);
	
	// Set location (in bounds)
	asteroid.setLocation(100, 200);
	printValues(asteroid);
	
	// Set location (out of bounds)
	asteroid.setLocation(1020, 2040);
	printValues(asteroid);
	
	// Instantiate SpaceObject (with constructor)
	SpaceObject foo(ASTEROID, 10, Point { .x = 100, .y = 2 }, Point { .x = 10, .y = -20 }, 0);
	printValues(foo);
	
	// Set radius (invalid)
	foo.setRadius(2405);
	printValues(foo);
	
	// Set radius (valid)
	foo.setRadius(32);
	printValues(foo);
	
	// Set velocity
	foo.setVelocity(-24, 42);
	printValues(foo);
	
	// Set angle (absolute)
	foo.setAngle(273.2);
	printValues(foo);
	
	// Change angle (relative)
	foo.changeAngle(-20);
	printValues(foo);
	
	// Update position based on velocity
	foo.updatePosition();	
	printValues(foo);
	
}

//get and print to cout all values of the given object
void printValues(SpaceObject obj) {
	Point location = obj.getLocation();
	std::cout << "Location: (" << location.x << ", " << location.y << ")" << std::endl;
	//TODO: get and print other values
	Point velocity = obj.getVelocity();
	std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")" << std::endl;
	
	double angle = obj.getAngle();
	std::cout << "Angle: " << angle << "°" << std::endl;
	
	double radius = obj.getRadius();
	std::cout << "Radius: " << radius << " pixels" << std::endl << std::endl;
}
