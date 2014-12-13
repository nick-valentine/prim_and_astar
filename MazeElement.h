#ifndef MAZEELEMENT_H
#define MAZEELEMENT_H

#include <SFML/Graphics.hpp>

#define index( x, y, width ) ((x) * width + (y))

class MazeElement
{
 public:
	enum state
	{
		None = 0,
		Wall = 1,
		Floor = 2
	};	
	
	MazeElement();
	//x,y as in index for a grid and not as in coordinates
	MazeElement( int x, int y );
	
	void draw( sf::RenderWindow &window );
	
	void toggle();
	void timed_toggle();
	state getState();
	void setState( state newState );
	
	void setColor( sf::Color c );
	void resetColor();
	
	static int getHeight();
	static int getWidth();
	
	static sf::Color floorColor();
	static sf::Color wallColor();
 private:
 	const static unsigned int width = 10;
 	const static unsigned int height = 10;
 	const static int time_to_toggle = 200;
 	
 	const static sf::Color floor;
 	const static sf::Color wall;
 
	state state_;
	sf::RectangleShape rect_;
	sf::Clock toggle_timer_;
};


#endif //MAZEELEMENT_H
