/** --------------------------- 
* @file		spaceObject.h
* @author	Charlie Welsh
* @version	1.0
*
* CS162-01 - Assignment 2.1
*	Headers for the SpaceObject class, its methods, and the screen size.
*
* --------------------------- */

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H

#include "constants.h"
#include <SFML/Graphics.hpp>

struct Point {
	double x;
	double y;
};

enum SpaceObjType { SHIP, ASTEROID, PHOTON_TORPEDO, SHIP_EXPLODING, SHIP_GONE }; 


class SpaceObject {
	public:
	
		/**
		* Initializes a SpaceObject with a type of ASTEROID, a radius of 20, a position of (0, 0), a velocity of (0, 0), and an angle of 0°
		*/
		SpaceObject();
		
		/**
		* Initializes a SpaceObject
		* 	@param type - the type of the object (defaults to ASTEROID if input is invalid)
		* 	@param radius - the radius of the object (defaults to 20 if given parameter is invalid)
		* 	@param position - the position of the object (defaults to (0, 0) if the input is invalid. If the input is out of bounds, it's wrapped around the screen until it's back on screen)
		* 	@param velocity - the velocity of the object (defaults to (0, 0) if the input is invalid.)
		* 	@param angle - the angle of the object (defaults to 0° if input is invalid.)
		*/
		SpaceObject(SpaceObjType type, double radius, Point position, Point velocity, double angle);
		// --------------------------- 
		// Mutators
		
		/**
		* Sets the radius of a SpaceObject.
		*	@param letter - the character to be checked
		*	@return true if radius is valid (not negative and under half the screen height),
		*		false otherwise
		*/
		bool setRadius(int radius);
		
		/**
		* Sets the position of a SpaceObject.
		*	@param x - the x coordinate of the new position
		*	@param y - the y coordinate of the new position
		*	@return true upon completion
		*/
		bool setPosition(double x, double y);
		
		/**
		* Sets the velocity of a SpaceObject. If the input is out of bounds, it's wrapped around the screen until it's back in bounds
		*	@param x - the x change (in pixels/frame) of the new velocity
		*	@param y - the y change (in pixels/frame) of the new velocity
		*	@return true upon completion
		*/
		bool setVelocity(double velocityX, double velocityY);
		
		/**
		* Sets the angle of a SpaceObject.
		*	@param angDeg - the new angle (in degrees) of the SpaceObject
		*	@return true upon completion
		*/
		bool setAngle(double angDeg);
	
		/**
		* Changes the angle of a SpaceObject by some delta
		*	@param deltaDeg - the number of degrees to change the angle of the SpaceObject by
		*/
		void changeAngle(double deltaDeg);
	
		// --------------------------- 
		// Accessors
		
		/**
		* Returns the radius of the SpaceObject
		*	@return the radius (double) of the SpaceObject
		*/
		double getRadius() const;
		/**
		* Returns the position of the SpaceObject
		*	@return the position (Point) of the SpaceObject
		*/
		Point getPosition() const;
		/**
		* Returns the velocity of the SpaceObject
		*	@return the velocity (Point) of the SpaceObject
		*/
		Point getVelocity() const;
		/**
		* Returns the angle of the SpaceObject
		*	@return the angle (double) of the SpaceObject
		*/
		double getAngle() const;
	
		/**
		* Returns the type of the SpaceObject
		*	@return the type (enum SpaceObjType) of the SpaceObject
		*/
		SpaceObjType getType() const;
		
		bool isInCenter() const;
		
		// --------------------------- 
		// Others
		
		/**
		*	Updates the position of the SpaceObject based on its velocity.
		*/
		void updatePosition();

		/**
		*	Adds thrust in the direction that the ship is currently facing. 
		*/
		void applyThrust();
		
		/**
		*	Sets the ship's type to SHIP_EXPLODING, and sets the ship's velocity to 0.
		*/
		void explode();

		/** 
		*	Draws the SpaceObject on the given window
		*	
		*	@param win - the window on which we’ll draw the ship
		*/
		void draw(sf::RenderWindow& win);

		/**
		*	Draws the SpaceObject on the given window as an ASTEROID
		*	
		*	@param win - the window on which we’ll draw the ship
		*/
		void drawAsteroid(sf::RenderWindow& win);

		/**
		*	Draws the SpaceObject on the given window as a ship
		*	
		* 	@param win - the window on which we’ll draw the ship
		*/
		void drawShip(sf::RenderWindow& win);
		
		/**
		*	Draws the SpaceObject on the given window as an explosion
		*	
		* 	@param win - the window on which we’ll draw the exploding ship
		*/
		void drawExplodingShip(sf::RenderWindow& win);

	private: 
		SpaceObjType type;	// The type of the object in space
		Point position;		// The current position of the object in space (stored as a point)
		Point velocity;		// The current velocity of the object in space (in pixels/frame) (Programmer's note: this seems remarkably naive. The game becomes unplayable on a suitably fast machine (though the frame cap of 60fps solves this, but frame caps are bad, mmkay?). This should be swapped out for pixels/millisecond or something. That still doesn't fix the fact that this will look ugly as sin on Retina displays though. NOTE: I tried it. It does.)
		double angleDeg;	// The angle the object is facing (in degrees)
		double radius;		// The gross radius of the object (for collision detection)
};

#endif
