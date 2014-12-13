#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <math.h>
#include <iostream>

#include "MazeElement.h"

class AStar
{
 public:
 	AStar();
 	AStar( MazeElement *maze, int width, int height, int start_x, int start_y, int end_x, int end_y );
 	
 	bool update( MazeElement *maze );
 private:
 	const static unsigned int move_cost = 10;
 	const static sf::Color visited;
 	const static sf::Color current;
 	const static sf::Color path;
 
 	void calculate_heuristic( MazeElement *maze);
 	
 	//add neighbors to open list
 	void add_open_list( MazeElement *maze, int x, int y );
 	
 	//find open node with smallest g+h value and returns the index
 	int find_smallest_open();
 	
 	//new x, new y, calling x, calling y
 	void add_element( int nx, int ny, int cx, int cy );
 	
 	bool valid_element( int x, int y );
 	
 	bool in_closed_list( int x, int y );
 	bool in_open_list( int x, int y );
 
 	std::vector< int > heuristic_;
 	std::vector< int > movement_cost_;
 	std::vector< int > g_plus_h_;
 	std::vector< int > parent_x_;
 	std::vector< int > parent_y_;
 	
 	std::vector< int > open_list_x_;
 	std::vector< int > open_list_y_;
 	
 	std::vector< int > closed_list_x_;
 	std::vector< int > closed_list_y_;
 	
 	int height_;
 	int width_;
 	int start_x_;
 	int start_y_;
 	int end_x_;
 	int end_y_;
 	
 	bool found_end_;
 	bool found_begin_;
 	int tb_x_;
 	int tb_y_;
};

#endif //ASTAR_H
