#include "AStar.h"

const sf::Color AStar::visited( 200, 200, 255 );
const sf::Color AStar::current( 0, 0, 255 );
const sf::Color AStar::path( 0, 0, 255 );

AStar::AStar()
{
	height_ = -1;
	width_ = -1;
	start_x_ = -1;
	start_y_ = -1;
	end_x_ = -1;
	end_y_ = -1;
}

AStar::AStar( MazeElement *maze, int width, int height, int start_x, int start_y, int end_x, int end_y )
{
	this->height_ = height;
	this->width_ = width;
	this->start_x_ = start_x;
	this->start_y_ = start_y;
	this->end_x_ = end_x;
	this->end_y_ = end_y;

	this->found_end_ = false;
	this->found_begin_ = false;
	this->tb_x_ = -1;
	this->tb_y_ = -1;

	calculate_heuristic( maze );

	for(int i = 0; i < width * height; ++i)
	{
		movement_cost_.push_back( 99999 );
		g_plus_h_.push_back( 99999 );
		parent_x_.push_back( -1 );
		parent_y_.push_back( -1 );
	}

	closed_list_x_.push_back( start_x_ );
	closed_list_y_.push_back( start_y_ );
	//parent_x_[ index( start_x_, start_y_, width_ ) ] = 0;
	//parent_y_[ index( start_x_, start_y_, width_ ) ] = 0;

	add_open_list( maze, start_x_, start_y_ );
}

bool AStar::update( MazeElement *maze )
{
	//only run if properly constructed and there are still elements to look at
	if( height_ != -1 && !found_begin_ )
	{
		if( !open_list_x_.empty() && !found_end_ )
		{
			//find next node to work on
			int next_node = find_smallest_open();
			int x = open_list_x_[ next_node ];
			int y = open_list_y_[ next_node ];

			if( x == end_x_ && y == end_y_ )
			{
				found_end_ = true;
				tb_x_ = parent_x_[ index( x, y, width_ ) ];
				tb_y_ = parent_y_[ index( x, y, width_ ) ];
				return found_begin_;
			}

			//add it to the closed list and remove from open list
			closed_list_x_.push_back( x );
			closed_list_y_.push_back( y );
			open_list_x_[ next_node ] = open_list_x_[ open_list_x_.size() - 1 ];
			open_list_y_[ next_node ] = open_list_y_[ open_list_y_.size() - 1 ];
			open_list_x_.pop_back();
			open_list_y_.pop_back();

			maze[ index( x, y, width_ ) ].setColor( visited );

			add_open_list( maze, x, y );
		}
		else if( found_end_ )
		{
			maze[ index( tb_x_, tb_y_, width_ ) ].setColor( path );
			tb_x_ = parent_x_[ index( tb_x_, tb_y_, width_ ) ];
			tb_y_ = parent_y_[ index( tb_x_, tb_y_, width_ ) ];

			if( tb_x_ == start_x_ && tb_y_ == start_y_ )
			{
				found_begin_ = true;
			}
			else if( tb_x_ == -1 || tb_y_ == -1 )
			{
				//std::cout<<"ERROR, INVALID PARENT!\n";
				found_begin_ = true;
			}
		}
	}
	return found_begin_;
}

void AStar::calculate_heuristic( MazeElement *maze )
{
	//only run if properly constructed
	if( height_ != -1 )
	{

		//calculates using the manhatten format

		for(int i = 0; i < width_ * height_; ++i)
		{
			if( maze[i].getState() != MazeElement::Wall )
			{
				int x = i / width_;
				int y = i % width_;

				x = abs( x - end_x_ );
				y = abs( y - end_y_ );

				heuristic_.push_back( x + y );
			}
			else
			{
				heuristic_.push_back( 99999 );
			}
		}
	}
}

void AStar::add_open_list( MazeElement * maze, int x, int y )
{
	//add top element
	if( valid_element( x, y - 1 ) && !in_closed_list( x, y - 1 ) &&
		maze[ index( x, y - 1, width_ ) ].getState() == MazeElement::Floor )
	{
		add_element( x, y - 1, x, y );
	}

	//add bottom element
	if( valid_element( x, y + 1 ) && !in_closed_list( x, y + 1 ) &&
		maze[ index( x, y + 1, width_ ) ].getState() == MazeElement::Floor )
	{
		add_element( x, y + 1, x, y );
	}

	//add left element
	if( valid_element( x - 1, y ) && !in_closed_list( x - 1, y ) &&
		maze[ index( x - 1, y, width_  ) ].getState() == MazeElement::Floor )
	{
		add_element( x - 1, y, x, y );
	}

	//add right element
	if( valid_element( x + 1, y ) && !in_closed_list( x + 1, y) &&
		maze[ index( x + 1, y, width_ ) ].getState() == MazeElement::Floor )
	{
		add_element( x + 1, y, x, y );
	}
}

int AStar::find_smallest_open()
{
	int smallest_gph = 99999;
	int smallest_index = -1;
	int gph;

	for(unsigned int i = 0; i < open_list_x_.size(); ++i)
	{
		gph = g_plus_h_[ index( open_list_x_[i], open_list_y_[i], width_ ) ];

		if( gph < smallest_gph )
		{
			smallest_gph = gph;
			smallest_index = i;
		}
	}

	return smallest_index;
}

bool AStar::in_closed_list( int x, int y )
{
	for(unsigned int i = 0; i < closed_list_x_.size(); ++i)
	{
		if( closed_list_x_[i] == x && closed_list_y_[i] == y )
		{
			return true;
		}
	}
	return false;
}

bool AStar::in_open_list( int x, int y )
{
	for(unsigned int i = 0; i < open_list_x_.size(); ++i)
	{
		if( open_list_x_[i] == x && open_list_y_[i] == y )
		{
			return true;
		}
	}
	return false;
}

void AStar::add_element(  int nx, int ny, int cx, int cy )
{
	if( !in_open_list( nx, ny ) )
	{
		open_list_x_.push_back( nx );
		open_list_y_.push_back( ny );
		parent_x_[ index( nx, ny, width_ ) ] = cx;
		parent_y_[ index( nx, ny, width_ ) ] = cy;
		g_plus_h_[ index( nx, ny, width_ ) ] = heuristic_[ index( nx, ny, width_ ) ] + move_cost;
	}
	else
	{
		int new_g = movement_cost_[ index( cx, cy, width_ ) ] + move_cost;
		if( new_g < movement_cost_[ index( nx, ny, width_ ) ] )
		{
			parent_x_[ index( nx, ny, width_ ) ] = cx;
			parent_y_[ index( nx, ny, width_ ) ] = cy;
		}
	}
}

bool AStar::valid_element( int x, int y )
{
	return x < width_ && x >= 0 && y < height_ && y >= 0;
}
