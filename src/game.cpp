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
#include <chrono>
#include "spaceObject.cpp"
#include "constants.h"

Point getRandomPosition();
Point getRandomVelocity();

/**
*	Gets if two SpaceObjects intersect
*		@param object1 - the first object to be checked
*		@param object2 - the second object to be checked
*		@return true if the objects intersect, false otherwise
*/
bool objectsIntersect(SpaceObject *object1, SpaceObject *object2);

/**
*	Returns the index of the first nullptr member of an array of SpaceObject()s
*		@param arr - the array of SpaceObjects to check for nullptrs
*		@param maxSize - the size of the array
*		@return an index if a nullptr can be found in the array, -1 otherwise
*/
int firstNullMember(SpaceObject* arr[], int maxSize);

/**
*	Returns the number of milliseconds since the Unix Epoch
*		@return current epoch time in milliseconds (unsigned long)
*/
unsigned long millisecondsSinceEpoch();

/**
*	Blows an asteroid up and removes the attacking photon torpedo
*		@param photons - the array of photons (SpaceObject*) to delete from
*		@param asteroids - the array of asteroids (SpaceObject*) to delete from
*		@param photonIndex - the index (int) of the photon to delete
*		@param asteroidIndex - the index (int) of the asteroid to delete
*/
void blowAsteroidUp(SpaceObject* photons[], SpaceObject* asteroids[], int photonIndex, int asteroidIndex);

/**
*	Adds an asteroid (SpaceObject) to an array of SpaceObjects in the first available spot (a nullptr)
*		@param asteroids - the array to add to
*		@param oldAsteroidIndex - the size of the array
*		@param velocity - the velocity of the new object
*		@return the index of the added object (int)
*/
int addAsteroid(SpaceObject* asteroids[], int oldAsteroidIndex, Point velocity);

int main() {
	unsigned long photonTimestamp = 0; 
	
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
			// Check if a torpedo has been fired too recently
			if ((millisecondsSinceEpoch() - photonTimestamp) >= (1000 / MAX_PHOTONS_PER_SECOND)) {
				int i = 0;
				while (photons[i] != nullptr) {
					i++;
				}
				// In addition, make sure we're not creating too many photons (that goes into the asteroids[] memory because C++ isn't memory safe)
				if (i < MAX_PHOTONS && ship->getType() == SHIP) {
					photons[i] = new SpaceObject(
						PHOTON_TORPEDO,
						2,
						ship->getPosition(),
						ship->getVelocity(),
						ship->getAngle()
					);
					photons[i]->applyThrust(5);
					photonTimestamp = millisecondsSinceEpoch();
				}
			}
		}
		
		// Update game objects
		ship->updatePosition();
		
		bool objectsInCenter = false;
		
		// Iterate through the asteroids
		for (int i = 0; i < MAX_ASTEROIDS; i++) {
			// Check if the asteroid is not null
			if (asteroids[i]) {
				asteroids[i]->updatePosition();
				if (objectsIntersect(asteroids[i], ship) == true && ship->getType() == SHIP) {
					ship->explode();
				}
				if (asteroids[i]->isInCenter()) objectsInCenter = true;
			}
		}
		
		// Iterate through the photons
		for (int i = 0; i < MAX_PHOTONS; i++) {
			// Check if the photon is not null
			if (photons[i]) {
				// Check if the photon torpedo has reached its end of life, and if so, delete it
				if (photons[i]->getTimesDrawn() >= PHOTON_LIFESPAN) {
					delete photons[i];
					photons[i] = 0;
				// Otherwise, update its position
				} else {
					photons[i]->updatePosition();
				}
			}
		}
		
		if (!objectsInCenter && ship->getType() == SHIP_GONE) {
			// Delete the pointer to avoid a memory leak. We're not setting the pointer to NULL, because we set it to a new SpaceObject immediately after.
			delete ship;
			ship = new SpaceObject(
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

		// Is this a janky solution? Yes. Is there a better way to do it? Not that I can think of. 
		// Iterate through the photons, then the asteroids, check if both the given torpedo and the given asteroid actually exist, and if they do, check if they're intersecting. If they are, delete both and set them to null pointers.
		for (int i = 0; i < MAX_PHOTONS; i++) {
			for (int a = 0; a < MAX_ASTEROIDS; a++) {
				if (photons[i] && asteroids[a]) {
					if (objectsIntersect(photons[i], asteroids[a]) == true) {
						blowAsteroidUp(photons, asteroids, i, a);
					}
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

// --------------------------- 
int firstNullMember(SpaceObject* arr[], int maxSize) {
	int i = 0;
	while (arr[i] != nullptr) {
		i++;
	}
	if (i > maxSize) {
		return -1;
	}
	return i;
}

// --------------------------- 
unsigned long millisecondsSinceEpoch() {
	return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

// --------------------------- 
void blowAsteroidUp(SpaceObject* photons[], SpaceObject* asteroids[], int photonIndex, int asteroidIndex) {
	// Get rid of the photon, we won't be needing it.
	delete photons[photonIndex];
	photons[photonIndex] = nullptr;
	
	if (ASTEROID_RADIUS / 4.0 < asteroids[asteroidIndex]->getRadius()) {
		// Create our first asteroid, then save it to a variable. We'll need that later. 
		int freeIndex = addAsteroid(asteroids, asteroidIndex, getRandomVelocity());
		// Create our second asteroid, and make it identical to the first one, except take its velocity and reverse it. 
		addAsteroid(asteroids, asteroidIndex, asteroids[freeIndex]->getVelocity().reverse());
	}
	
	delete asteroids[asteroidIndex];
	asteroids[asteroidIndex] = nullptr;
}


int addAsteroid(SpaceObject* asteroids[], int oldAsteroidIndex, Point velocity) {
	int freeIndex = firstNullMember(asteroids, MAX_ASTEROIDS);
	if (freeIndex != -1) {
		asteroids[freeIndex] = new SpaceObject(
			ASTEROID,
			asteroids[oldAsteroidIndex]->getRadius() / 2,
			asteroids[oldAsteroidIndex]->getPosition(),
			velocity,
			asteroids[oldAsteroidIndex]->getAngle()
		);
	}
	return freeIndex;
}