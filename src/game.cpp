/** --------------------------- 
* @file		game.cpp
* @author	Charlie Welsh
* @version	1.0
*
* CS162-01 - Assignment 2.1
*	Runs an actual game of Polygons. SpaceObjects are instantiated here. 
*
* --------------------------- */

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "spaceObject.cpp"
#include "constants.h"

Point getRandomPosition();
Point getRandomVelocity();

bool objectsIntersect(SpaceObject *object1, SpaceObject *object2);

int main()
{
	//create graphics window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Polygons!", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60); 

	//create SpaceObjects
	
	SpaceObject *asteroids[MAX_ASTEROIDS] = {};
	
	SpaceObject *ship = new SpaceObject(
		SHIP, 
		40, 
		Point { 
			.x = WINDOW_WIDTH / 2, 
			.y = WINDOW_HEIGHT / 2 
		}, 
		Point { .x = 0, .y = 0 }, 
		0
	);
	
	srand(42);
	
	for ( int i = 0; i < 5; i++) {
		asteroids[i] = new SpaceObject(
			ASTEROID,
			60,
			getRandomPosition(),
			getRandomVelocity(),
			0
		);
	}
	
	//game loop
	while (window.isOpen()) {
		//handle user input (events and keyboard keys being pressed) ----
		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();
				
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::A) {
					ship->changeAngle(-10);
				}
				if (event.key.code == sf::Keyboard::D) {
					ship->changeAngle(10);
				}
				if (event.key.code == sf::Keyboard::W) {
					ship->applyThrust();
				}
			}
		}
		
		// Update game objects
		ship->updatePosition();
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			// check if the asteroid is not null
			if (asteroids[i]) {
				asteroids[i]->updatePosition();
			}
		}
		
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			// check if the asteroid is not null
			if (asteroids[i]) {
				if (objectsIntersect(asteroids[i], ship) == true) {
					std::cout << "Crash!" << std::endl;
				}
			}
		}

		// Draw new frame
		window.clear();
		ship->draw(window);
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			// check if the asteroid is not null
			if (asteroids[i]) {
				asteroids[i]->draw(window);
			}
		}
		window.display();
	}

	return 0;
}

Point getRandomPosition() {
	double x = rand() % WINDOW_WIDTH;
	double y = rand() % WINDOW_HEIGHT;
	
	Point result = Point { .x = x, .y = y };
	return Point { .x = x, .y = y };	
}

Point getRandomVelocity() {
	double x = rand() % 255;
	double y = rand() % 255;
	
	
	// Simpler to take the upper half of the range, since then we don't have to implement extra subtraction logic. 
	if (x > 127) {
		x = x * -1;
	}
	if (y > 127) {
		y = y * -1;
	}
	
	// Divide to a float that ranges from -1 to 1. 
	x = x / 127.0;
	y = y / 127.0;
	
	return Point { .x = x, .y = y };	
}

bool objectsIntersect(SpaceObject *object1, SpaceObject *object2) {
	Point firstLocation = object1->getLocation();
	Point secondLocation = object2->getLocation();
	
	double firstRadius = object1->getRadius();
	double secondRadius = object2->getRadius();
	
	double radiiSum = firstRadius + secondRadius;
	
	double xDistance = abs(firstLocation.x - secondLocation.x);
	double yDistance = abs(firstLocation.y - secondLocation.y);

	double totalDistance = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
	if (totalDistance > radiiSum) {
		return false;
	} else {
		return true;
	}
}