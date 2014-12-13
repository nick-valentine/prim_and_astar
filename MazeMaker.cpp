#include "MazeMaker.h"

#ifdef CHANGE_COLOR
	const sf::Color MazeMaker::wall_list( 100, 255, 100 );
	const sf::Color MazeMaker::maze_list( 100, 255, 100 );
#endif

const sf::Color MazeMaker::start_block( 0, 255, 0 );
const sf::Color MazeMaker::stop_block( 255, 0, 0 );

MazeMaker::MazeMaker()
{
	width_ = -1;
	height_ = -1;
}

MazeMaker::MazeMaker( MazeElement *maze, int width, int height, int x, int y )
{
	this->width_ = width;
	this->height_ = height;
	
	maze_list_x_.push_back( x );
	maze_list_y_.push_back( y );
	maze[ index( x, y, width ) ].setState( MazeElement::Floor );
	maze[ index( x, y, width ) ].setColor( start_block );
	
	start_x_ = x;
	start_y_ = y;
	
	stop_x_ = -1;
	stop_y_ = -1;
	
	add_walls( maze, x, y );
	
	srand( time( NULL ) );
	
	for(int i = 0; i < height * width; ++i)
	{
		random_weights_.push_back( rand() % 1000 );
	}
}



bool MazeMaker::update( MazeElement *maze )
{
	//if done or if not properly constructed
	if( wall_list_x_.empty() || width_ == -1)
	{
		return false;
	}
	else
	{
		int min = 1002;
		int min_index = -1;
		int list_index = -1;
		int x, y;
		for(unsigned int i = 0; i < wall_list_x_.size(); ++i)
		{
			x = wall_list_x_[i];
			y = wall_list_y_[i];
			
			if( random_weights_[ index( x, y, width_ ) ] < min )
			{
				min = random_weights_[ index( x, y, width_ ) ];
				min_index = index( x, y, width_ );
				list_index = i;
			}
		}
		
		if(min_index >= 0)
		{
			int x = min_index / width_;
			int y = min_index % width_;
			bool making_floor = false;
		
			if(count_touching_floors( maze, x, y ) <= 1)
			{
				maze[ min_index ].setState( MazeElement::Floor );
				
				//keep track of last floor placed so it will be the stop block
				if( stop_x_ != -1 )
				{
					maze[ index( stop_x_, stop_y_, width_ ) ].resetColor();
				}
				stop_x_ = x;
				stop_y_ = y;
				maze[ index( stop_x_, stop_y_, width_ ) ].setColor(stop_block);
				making_floor = true;
				
				maze_list_x_.push_back( x );
				maze_list_y_.push_back( y );
				add_walls( maze, x, y );
			}
			random_weights_[min_index] = 1001;
			
			if( !making_floor )
			{
				maze[ min_index ].resetColor();
			}
			
			wall_list_x_[ list_index ] = wall_list_x_[ wall_list_x_.size() - 1 ];
			wall_list_y_[ list_index ] = wall_list_y_[ wall_list_y_.size() - 1 ];
			wall_list_x_.pop_back();
			wall_list_y_.pop_back();
		}
		return true;
	}
}

	
int MazeMaker::getStartX()
{
	return this->start_x_;
}

int MazeMaker::getStartY()
{
	return this->start_y_;
}

int MazeMaker::getStopX()
{
	return this->stop_x_;
}

int MazeMaker::getStopY()
{
	return this->stop_y_;
}

void MazeMaker::add_walls( MazeElement *maze, int x, int y )
{
	
	//wall on top of x,y
	if(y > 0 )
	{
		if( find_wall_list( x, y - 1 ) == -1 &&
			find_maze_list( x, y - 1 ) == -1 )
		{
			wall_list_x_.push_back( x );
			wall_list_y_.push_back( y - 1 );
			maze[ index( x, y - 1, width_ ) ].setState( MazeElement::Wall );
			#ifdef CHANGE_COLOR
				maze[ index( x, y - 1, width_ ) ].setColor( wall_list );
			#endif
		}
	}
	
	//wall below x,y
	if(y < height_ - 1 )
	{
		if( find_wall_list( x, y + 1 ) == -1 &&
			find_maze_list( x, y + 1 ) == -1 )
		{
			wall_list_x_.push_back( x );
			wall_list_y_.push_back( y + 1 );
			maze[ index( x, y + 1, width_ ) ].setState( MazeElement::Wall );
			#ifdef CHANGE_COLOR
				maze[ index( x, y + 1, width_ ) ].setColor( wall_list );
			#endif
		}
	}
	
	//wall left of x,y
	if( x > 0 )
	{
		if( find_wall_list( x - 1, y ) == -1 &&
			find_maze_list( x - 1, y ) == -1 )
		{
			wall_list_x_.push_back( x - 1 );
			wall_list_y_.push_back( y );
			maze[ index( x - 1, y, width_ ) ].setState( MazeElement::Wall );
			#ifdef CHANGE_COLOR
				maze[ index( x - 1, y, width_ ) ].setColor( wall_list );
			#endif
		}
	}
	
	//wall right of x,y
	if( x < width_ - 1 )
	{
		if( find_wall_list( x + 1, y ) == -1 &&
			find_maze_list( x + 1, y ) == -1 )
		{
			wall_list_x_.push_back( x + 1 );
			wall_list_y_.push_back( y );
			maze[ index( x + 1, y, width_ ) ].setState( MazeElement::Wall );
			#ifdef CHANGE_COLOR
				maze[ index( x + 1, y, width_ ) ].setColor( wall_list );
			#endif
		}
	}
}

MazeMaker::Side MazeMaker::find_touching_maze( MazeElement *maze, int x, int y )
{
	if( x < width_ && maze[ index( x + 1, y, width_ ) ].getState() == MazeElement::Floor )
	{
		return East;
	}
	else if( x > 0 && maze[ index( x - 1, y, width_ ) ].getState() == MazeElement::Floor )
	{
		return West;
	}
	else if( y > 0 && maze[ index( x , y - 1, width_ ) ].getState() == MazeElement::Floor )
	{
		return North;
	}
	else if( y < height_ && maze[ index( x, y + 1, width_ ) ].getState() == MazeElement::Floor )
	{
		return South;
	}
	else
	{
		return None;
	}
}

int MazeMaker::count_touching_floors( MazeElement *maze, int x, int y )
{
	int i = 0;
	if( x < width_ && maze[ index( x + 1, y, width_ ) ].getState() == MazeElement::Floor )
	{
		i++;
	}
	if( x > 0 && maze[ index( x - 1, y, width_ ) ].getState() == MazeElement::Floor )
	{
		i++;
	}
	if( y > 0 && maze[ index( x , y - 1, width_ ) ].getState() == MazeElement::Floor )
	{
		i++;
	}
	if( y < height_ && maze[ index( x, y + 1, width_ ) ].getState() == MazeElement::Floor )
	{
		i++;
	}
	
	#ifndef DENSE_MAZE
	int j = 0;
	
	//count corners too
	if( x < width_ && y < height_ && maze[ index( x + 1, y + 1, width_ ) ].getState() == MazeElement::Floor )
	{
		j++;
	}
	if( x < width_ && y > 0 && maze[ index( x + 1, y - 1, width_ ) ].getState() == MazeElement::Floor )
	{
		j++;
	}
	if( x > 0 && y < height_ && maze[ index( x - 1 , y + 1, width_ ) ].getState() == MazeElement::Floor )
	{
		j++;
	}
	if( x > 0 && y > 0 && maze[ index( x - 1, y - 1, width_ ) ].getState() == MazeElement::Floor )
	{
		j++;
	}
	
	if( j % 2 != 0 )
	{
		i += j;
	}
	
	#endif //DENSE_MAZE
	
	return i;
}

bool MazeMaker::traverse( MazeMaker::Side goTo, int *x, int *y )
{
	int x_ = *x, y_ = *y;
	if( goTo == North )
	{
		*y = std::max( *y - 1, 0 );
	}
	else if( goTo == South )
	{
		*y = std::min( *y + 1, height_ - 1 );
	}
	else if( goTo == East )
	{
		*x = std::min( *x + 1, width_ - 1 );
	}
	else if( goTo == West )
	{
		*x = std::max( *x - 1, 0 );
	}
	
	if( x_ == *x && y_ == *y )
	{
		//traversal unsuccessful
		return false;
	}
	else
	{
		//traversal successful
		return true;
	}
}

MazeMaker::Side MazeMaker::invert_side( Side x )
{
	if( x == North )
	{
		return South;
	}
	else if( x == South )
	{
		return North;
	}
	else if( x == East )
	{
		return West;
	}
	else if(x == West )
	{
		return East;
	}
	else
	{
		return None;
	}
}

int MazeMaker::find_maze_list( int x, int y )
{
	for(unsigned int i = 0; i < maze_list_x_.size(); ++i)
	{
		if( x == maze_list_x_[i] && y == maze_list_y_[i] )
		{
			return i;
		}
	}
	return -1;
}

int MazeMaker::find_wall_list( int x, int y )
{
	for(unsigned int i = 0; i < wall_list_x_.size(); ++i)
	{
		if( x == wall_list_x_[i] && y == wall_list_y_[i] )
		{
			return i;
		}
	}
	return -1;
}

MazeMaker::Side MazeMaker::get_min_adj_weight( int x, int y )
{
	int top, bottom, left, right;
	top = bottom = left = right = 9999999;
	
	if( y > 0 )
	{
		top = random_weights_[ index( x, y - 1, width_ ) ];
	}
	
	if( y < height_ - 1 )
	{
		bottom = random_weights_[ index( x, y + 1, width_ ) ];
	}
	
	if( x > 0 )
	{
		left = random_weights_[ index( x - 1, y, width_) ];
	}
	
	if( x < width_ - 1 )
	{
		right = random_weights_[ index( x + 1, y, width_) ];
	}
	
	int min = 9999999;
	min = ( top < min ) ? top  : min;
	min = ( bottom < min ) ? bottom : min;
	min = ( left < min ) ? left : min;
	min = ( right < min ) ? right : min;
	
	if( min == top )
	{
		return North;
	}
	else if( min == bottom )
	{
		return South;
	}
	else if( min == left )
	{
		return West;
	}
	else if( min == right )
	{
		return East;
	}
	else
	{
		return None;
	}
}
