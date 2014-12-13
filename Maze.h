#ifndef MAZE_H
#define MAZE_H

#include <iostream>

#include "MazeMaker.h"
#include "MazeElement.h"
#include "AStar.h"

class Maze
{
 public:
 	Maze();
 	Maze( int width, int height );
 	Maze( const Maze &RHS);
 	~Maze();
 	
 	void draw( sf::RenderWindow &window );
 	void update( sf::RenderWindow &window );
 	
 	int getWidth() const;
 	int getHeight() const;
 private:
	static const sf::Color start_block;
	static const sf::Color stop_block;
 
 	MazeElement *maze_;
 	MazeMaker maker_;
 	AStar solver_;
 	
 	int width_;
 	int height_;
 	
 	int start_x_;
 	int start_y_;
 	int stop_x_;
 	int stop_y_;
 	
 	bool solving_;
 	bool done_generating_;
 	bool first_solving_run_;
 	bool first_generate_run_;
 	bool auto_generate_;
 	
 	MazeElement::state drawing_state_;
};

#endif //MAZE_H
