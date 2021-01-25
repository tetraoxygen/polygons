const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;

struct Point {
	double x;
	double y;
};

enum SpaceObjType { SHIP, ASTEROID, PHOTON_TORPEDO }; 


class SpaceObject {

public:
	SpaceObject();
	SpaceObject(SpaceObjType type, double radius, Point location, Point velocity, double angle);
	// --------------------------- 
	// Mutators
	bool setRadius(int radius);
	bool setLocation(double x, double y);
	bool setVelocity(double velocityX, double velocityY);
	bool setAngle(double angDeg);

	// Changes the angle by given amount.
	void changeAngle(double deltaDeg);

	// --------------------------- 
	// Accessors
	double getRadius() const;
	Point getLocation() const;
	Point getVelocity() const;
	double getAngle() const;

	//============================================
	//others
	void updatePosition();

	private: 
		SpaceObjType type;	  //type of object
		Point location;	  //current location (x,y)
		Point velocity;	  //current velocity (in pixels/frame) (Programmer's note: this seems remarkably naive. The game becomes unplayable on a suitably fast machine. This should be swapped out for pixels/millisecond or something. That still doesn't fix the fact that this will look ugly as sin on Retina displays though.)
		double angleDeg;	  //angle object is facing (in degrees)
		double radius;        //gross radius of object (for collision detection)
};
