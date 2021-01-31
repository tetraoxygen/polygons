#include <SFML/Graphics.hpp>
#include "spaceObject.cpp"
//#include "spaceObject.h"
#include "constants.h"

//============================================================================
int main()
{
	//create graphics window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Polygons!", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60); 

//create SpaceObjects

	SpaceObject asteroid(
		ASTEROID, 
		60, 
		Point { 
			.x = WINDOW_WIDTH / 2, 
			.y = WINDOW_HEIGHT / 2 
		}, 
		Point { .x = 2, .y = 4 }, 
		24
	);
	
	SpaceObject asteroid2(
	ASTEROID, 
	60, 
	Point { 
		.x = WINDOW_WIDTH / 2 + 100, 
		.y = WINDOW_HEIGHT / 2 + 100 
	}, 
	Point { .x = 2, .y = 4 }, 
	24
);

	SpaceObject ship(
		SHIP, 
		40, 
		Point { 
			.x = WINDOW_WIDTH / 2, 
			.y = WINDOW_HEIGHT / 2 
		}, 
		Point { .x = 0, .y = 0 }, 
		0
	);

	//game loop
	while (window.isOpen()) {
		//handle user input (events and keyboard keys being pressed) ----
		sf::Event event;
		while (window.pollEvent(event))	{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			ship.changeAngle(-2); 
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))  
			ship.changeAngle(2);  
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))  
			ship.applyThrust();


		// update game objects ------------------------------------------
		asteroid.updatePosition(); //uncomment this in step 2.2
		asteroid2.updatePosition(); //uncomment this in step 2.2
		ship.updatePosition(); //uncomment this in step 3.2

		// draw new frame ------------------------------------------------
		window.clear();
		asteroid.draw(window);  //uncomment this in step 2.2 !!
		asteroid2.draw(window);  //uncomment this in step 2.2 !!

		ship.draw(window);  //uncomment this in step 3.2 !!
		window.display();
	}

	return 0;
}
