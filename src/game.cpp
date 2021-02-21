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
#include <iostream>
#include "spaceObject.cpp"
#include "constants.h"

Point getRandomPosition();
Point getRandomVelocity();

bool objectsIntersect(SpaceObject *object1, SpaceObject *object2);

int main() {

	// Booleans for simultaneous movement (left/right, thrust, and torpedoes should all work at the same time)
	bool leftPressed = false;
	bool rightPressed = false;
	bool thrustPressed = false;
	bool firePressed = false;
	
	//create graphics window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Polygons!", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60); 

	//create SpaceObjects
	
	SpaceObject *asteroids[MAX_ASTEROIDS] = {};
	
	SpaceObject *photons[MAX_PHOTONS] = {};
	
	SpaceObject *ship = new SpaceObject(
		SHIP, 
		40, 
		Point { 
			.x = WINDOW_WIDTH / 2.0, 
			.y = WINDOW_HEIGHT / 2.0 
		}, 
		Point { .x = 0, .y = 0 }, 
		0
	);
	
	srand(1238972139);
	
	for ( int i = 0; i < 5; i++) {
		asteroids[i] = new SpaceObject(
			ASTEROID,
			60,
			getRandomPosition(),
			getRandomVelocity(),
			(rand() % 360)
		);
	}
	
	//game loop
	while (window.isOpen()) {
		// Handle user input (events and keyboard keys being pressed) ----
		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();
				
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::A) {
					leftPressed = true;
				}
				if (event.key.code == sf::Keyboard::D) {
					rightPressed = true;
				}
				if (event.key.code == sf::Keyboard::W) {
					thrustPressed = true;
				}
				if (event.key.code == sf::Keyboard::Space) {
					firePressed = true;
				}
			}
			
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::A) {
					leftPressed = false;
				}
				if (event.key.code == sf::Keyboard::D) {
					rightPressed = false;
				}
				if (event.key.code == sf::Keyboard::W) {
					thrustPressed = false;
				}
				if (event.key.code == sf::Keyboard::Space) {
					firePressed = false;
				}
			}
		}
		
		// Update objects in response to user input
		if (leftPressed) {
			ship->changeAngle(-1.75);
		}
		if (rightPressed) {
			ship->changeAngle(1.75);
		}
		if (thrustPressed) {
			ship->applyThrust(0.04);
		}
		if (firePressed) {
			int i = 0;
			while (photons[i] != nullptr) {
				i++;
			}
			// Make sure we're not creating too many photons (that goes into the asteroids[] memory because C+++ isn't memory safe)
			if (i < MAX_PHOTONS) {
				photons[i] = new SpaceObject(
					PHOTON_TORPEDO,
					2,
					ship->getPosition(),
					ship->getVelocity(),
					ship->getAngle()
				);
				std::cout << "Photon velocity (before applying thrust): " << photons[i]->getVelocity().x << ", " << photons[i]->getVelocity().y << std::endl;
				photons[i]->applyThrust(5);
				std::cout << "Photon velocity (after applying thrust): " << photons[i]->getVelocity().x << ", " << photons[i]->getVelocity().y << std::endl;
			}
		}
		
		// Update game objects
		ship->updatePosition();
		
		bool objectsInCenter = false;
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			// check if the asteroid is not null
			if (asteroids[i]) {
				asteroids[i]->updatePosition();
				if (objectsIntersect(asteroids[i], ship) == true && ship->getType() == SHIP) {
					ship->explode();
				}
				if (asteroids[i]->isInCenter()) objectsInCenter = true;
			}
		}
		
		for (int i = 0; i < MAX_PHOTONS; i++) {
			// check if the photon is not null
			if (photons[i]) {
				photons[i]->updatePosition();
			}
		}
		
		if (!objectsInCenter && ship->getType() == SHIP_GONE) {
			// delete the pointer to avoid a memory leak. We're not setting the pointer to NULL, because we set it to a new SpaceObject immediately after.
			delete ship;
			ship =  new SpaceObject(
				SHIP, 
				40, 
				Point { 
					.x = WINDOW_WIDTH / 2.0, 
					.y = WINDOW_HEIGHT / 2.0 
				}, 
				Point { .x = 0, .y = 0 }, 
				0
			);
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
		for (int i = 0; i < MAX_PHOTONS; i++) {
			// check if the asteroid is not null
			if (photons[i]) {
				photons[i]->draw(window);
			}
		}
		
		window.display();
	}

	return 0;
}

Point getRandomPosition() {
	double x = rand() % WINDOW_WIDTH;
	double y = rand() % WINDOW_HEIGHT;
	
	// moving the result if it's too close to the center (within 200 pixels)
	
	// X
	if (((WINDOW_WIDTH / 2.0) + 100) < x && ((WINDOW_WIDTH / 2.0) - 100) > x) {
		if (rand() % 2) {
			x = x - 200;
		} else {
			x = x + 200;
		}
	}
	
	// Y
	if (((WINDOW_WIDTH / 2.0) + 100) < y && ((WINDOW_WIDTH / 2.0) - 100) > y) {
		if (rand() % 2) {
			x = x - 200;
		} else {
			x = x + 200;
		}
	}
	
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
	Point firstPosition = object1->getPosition();
	Point secondPosition = object2->getPosition();
	
	double firstRadius = object1->getRadius();
	double secondRadius = object2->getRadius();
	
	double radiiSum = firstRadius + secondRadius;
	
	double xDistance = abs(firstPosition.x - secondPosition.x);
	double yDistance = abs(firstPosition.y - secondPosition.y);

	double totalDistance = sqrt((xDistance * xDistance) + (yDistance * yDistance));
		
	return radiiSum > totalDistance;
}

