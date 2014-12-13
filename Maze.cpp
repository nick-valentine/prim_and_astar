#include "Maze.h"

const sf::Color Maze::start_block( 0, 255, 0 );
const sf::Color Maze::stop_block( 255, 0, 0 );

Maze::Maze()
{
	maze_ = 0;
	width_ = -1;
	height_ = -1;
	maker_ = MazeMaker();
}

Maze::Maze( int width, int height )
{
	this->maze_ = new MazeElement[ height * width ];
	
	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			this->maze_[i * width + j] = MazeElement(i, j);
		}
	}
	
	maker_ = MazeMaker( maze_, width, height, width / 2, height / 2 );
	
	this->width_ = width;
	this->height_ = height;
	
	done_generating_ = false;
	auto_generate_ = false;
	solving_ = false;
	first_generate_run_ = true;
	first_solving_run_ = true;
}

Maze::Maze( const Maze &RHS )
{
	if( maze_ != 0 )
	{
		delete[] maze_;
	}
	
	width_ = RHS.width_;
	height_ = RHS.height_;
	
	done_generating_ = RHS.done_generating_;
	auto_generate_ = RHS.auto_generate_;
	
	maze_ = new MazeElement[ height_ * width_ ];
	
	
	for(int i = 0; i < width_ * height_; ++i)
	{
			maze_[i] = RHS.maze_[i];
	}
}

Maze::~Maze()
{
	delete[] maze_;
}

void Maze::draw( sf::RenderWindow &window)
{
	for(int i = 0; i < width_ * height_; ++i)
	{
		maze_[i].draw( window );
	}
}

void Maze::update( sf::RenderWindow &window )
{
	
	if( !done_generating_ )
	{
		if( auto_generate_ )
		{
			//reset the maze the first run
			if( first_generate_run_ )
			{
				for( int i = 0; i < height_ * width_; ++i)
				{
					maze_[i].setState( MazeElement::Wall );
				}
				
				maker_ = MazeMaker( maze_, width_, height_, width_ / 2, height_ / 2 );
				first_generate_run_ = false;
			}
		
			done_generating_ = !maker_.update( maze_ );
			
			if( done_generating_ )
			{
				start_x_ = maker_.getStartX();
				start_y_ = maker_.getStartY();
				stop_x_ = maker_.getStopX();
				stop_y_ = maker_.getStopY();
			}
		}

		if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
		{
			//if the mouse button is being pressed, we want to draw
			//so get the mouse position
			sf::Vector2i mouse = sf::Mouse::getPosition( window );
		
			//convert the mouse position to an x and y coordinate in our
			//array of MazeElements
			int x = mouse.x / MazeElement::getWidth();
			int y = mouse.y / MazeElement::getHeight();
			//It is possible for the mouse to stay clicked as it exits the screen
			//so make sure we only draw to indexable array elements
			if( x < width_ && y < height_ &&
				x > 0      && y > 0)
			{
				//Set drawing state to the opposite of the first element clicked on
				if( drawing_state_ == MazeElement::None )
				{
					if( maze_[ x * width_ + y].getState() == MazeElement::Wall)
					{
						drawing_state_ = MazeElement::Floor;
					}
					else if( maze_[ x * width_ + y].getState() == MazeElement::Floor)
					{
						drawing_state_ = MazeElement::Wall;
					}
				}
				
				//draw the current state out to wherever the mouse is
				maze_[ index( x, y, width_ ) ].setState( drawing_state_ );
			}
		}
		else
		{
			//if the mouse is no longer being pressed, change the drawing state
			drawing_state_ = MazeElement::None;
		}
		
		if( !auto_generate_ && !solving_ )
		{
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
			{
				done_generating_ = true;
			}
			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A ) )
			{
				auto_generate_ = true;
				first_generate_run_ = true;
				done_generating_ = false;
			}
			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::Num1 ) )
			{
				//place start block
			
			
				//if the mouse button is being pressed, we want to draw
				//so get the mouse position
				sf::Vector2i mouse = sf::Mouse::getPosition( window );
			
				//convert the mouse position to an x and y coordinate in our
				//array of MazeElements
				int x = mouse.x / MazeElement::getWidth();
				int y = mouse.y / MazeElement::getHeight();
			
				if( x < width_ && y < height_ &&
					x > 0      && y > 0)
				{
					//draw the current state out to wherever the mouse is
					maze_[ index( x, y, width_ ) ].setState( MazeElement::Floor );
					maze_[ index( start_x_, start_y_, width_ ) ].resetColor();
					start_x_ = x;
					start_y_ = y;
					maze_[ index( start_x_, start_y_, width_ ) ].setColor( start_block );
				}
			}
			else if( sf::Keyboard::isKeyPressed( sf::Keyboard::Num2 ) )
			{
				//place stop block
			
				//if the mouse button is being pressed, we want to draw
				//so get the mouse position
				sf::Vector2i mouse = sf::Mouse::getPosition( window );
			
				//convert the mouse position to an x and y coordinate in our
				//array of MazeElements
				int x = mouse.x / MazeElement::getWidth();
				int y = mouse.y / MazeElement::getHeight();
			
				if( x < width_ && y < height_ &&
					x > 0      && y > 0)
				{
					//draw the current state out to wherever the mouse is
					maze_[ index( x, y, width_ ) ].setState( MazeElement::Floor );
					maze_[ index( stop_x_, stop_y_, width_ ) ].resetColor();
					stop_x_ = x;
					stop_y_ = y;
					maze_[ index( stop_x_, stop_y_, width_ ) ].setColor( stop_block );
				}
			}
		}
	}
	else if( solving_ )
	{
		if( first_solving_run_ )
		{
			solver_ = AStar( maze_, height_, width_,
							 start_x_, start_y_,
							 stop_x_, stop_y_ );
							 
			 first_solving_run_ = false;
		}

	
		solving_ = !solver_.update( maze_ );
	}
	
	if( sf::Keyboard::isKeyPressed( sf::Keyboard::C ) )
	{
		//clear
		solving_ = false;
		auto_generate_ = false;
		done_generating_ = false;
		first_generate_run_ = true;
		first_solving_run_ = true;
		
		for( int i = 0; i < height_ * width_; ++i)
		{
			maze_[i].setState( MazeElement::Floor );
		}
	}
	else if( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
	{
		//solve
		solving_ = true;
		auto_generate_ = false;
		done_generating_ = true;
		first_generate_run_ = true;
		first_solving_run_ = true;
	}
	else if( sf::Keyboard::isKeyPressed( sf::Keyboard::E ) )
	{
		//edit
		solving_ = false;
		auto_generate_ = false;
		done_generating_ = false;
		first_generate_run_ = true;
		first_solving_run_ = true;
		
		for( int i = 0; i < height_ * width_; ++i)
		{
			maze_[i].resetColor();
		}
		maze_[ index( start_x_, start_y_, width_ ) ].setColor( start_block );
		maze_[ index( stop_x_, stop_y_, width_ ) ].setColor( stop_block );
	}
}

int Maze::getWidth() const
{
	return this->width_;
}

int Maze::getHeight() const
{
	return this->height_;
}
