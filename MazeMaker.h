#ifndef MAZEMAKER_H
#define MAZEMAKER_H

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>

#include "MazeElement.h"

#define CHANGE_COLOR
#define DENSE_MAZE

class MazeMaker
{
 public:
 	
 	enum Side
 	{
 		None = 0,
 		North = 1,
 		East = 2,
 		West = 3,
 		South = 4
 	};
 
 	MazeMaker();
 	MazeMaker( MazeElement *maze, int width, int height, int x, int y );
 	
 	bool update( MazeElement *maze );
 	
 	int getStartX();
 	int getStartY();
 	int getStopX();
 	int getStopY();
 
 private:
 	void add_walls( MazeElement *maze, int x, int y );
 	Side find_touching_maze( MazeElement *maze, int x, int y );
 	int count_touching_floors( MazeElement *maze, int x, int y );
 	bool traverse( Side goTo, int *x, int *y );
 	Side invert_side( Side x );
 	int find_maze_list( int x, int y );
 	int find_wall_list( int x, int y );
 	Side get_min_adj_weight( int x, int y );
 	
 	#ifdef CHANGE_COLOR
	 	static const sf::Color wall_list;
	 	static const sf::Color maze_list;
 	#endif
 	
 		static const sf::Color start_block;
 		static const sf::Color stop_block;
 
 	int width_;
 	int height_;
 
 	std::vector<int> random_weights_;
 
 	std::vector<int> wall_list_x_;
 	std::vector<int> wall_list_y_;
 	std::vector<int> maze_list_x_;
 	std::vector<int> maze_list_y_;
 	
 	int start_x_;
 	int start_y_;
 	
 	int stop_x_;
 	int stop_y_;
};

#endif //MAZEMAKER_H
