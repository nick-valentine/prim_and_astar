#include "MazeElement.h"

const sf::Color MazeElement::floor( 255, 255, 255 );
const sf::Color MazeElement::wall( 0, 0, 0 );

MazeElement::MazeElement()
{
	this->state_ = Wall;
	this->rect_.setPosition( 0, 0 );
	this->rect_.setSize( sf::Vector2f( width, height ) );
	this->rect_.setFillColor( wall );
}

MazeElement::MazeElement( int x, int y )
{
	this->state_ = Wall;
	this->rect_.setPosition( x * width, y * height );
	this->rect_.setSize( sf::Vector2f( width, height ) );
	this->rect_.setFillColor( wall );
}

void MazeElement::draw( sf::RenderWindow &window )
{
	window.draw( this->rect_ );
}

void MazeElement::toggle()
{
	if( this->state_ == Wall )
	{
		this->state_ = Floor;
		this->rect_.setFillColor( floor );
	}
	else
	{
		this->state_ = Wall;
		this->rect_.setFillColor( wall );
	}
}

void MazeElement::timed_toggle()
{
	if( toggle_timer_.getElapsedTime().asMilliseconds() > time_to_toggle )
	{
		toggle();
		toggle_timer_.restart();
	}
}

MazeElement::state MazeElement::getState()
{
	return this->state_;
}

void MazeElement::setState( MazeElement::state newState )
{
	this->state_ = newState;
	if( newState == Wall )
	{
		this->rect_.setFillColor( wall );
	}
	else
	{
		this->rect_.setFillColor( floor );
	}
}

void MazeElement::setColor( sf::Color c )
{
	this->rect_.setFillColor( c );
}

void MazeElement::resetColor()
{
	if( this->state_ == Wall )
	{
		this->rect_.setFillColor( wall );
	}
	else
	{
		this->rect_.setFillColor( floor );
	}
}

int MazeElement::getHeight()
{
	return height;
}

int MazeElement::getWidth()
{
	return width;
}

sf::Color MazeElement::floorColor()
{
	return floor;
}

sf::Color MazeElement::wallColor()
{
	return wall;
}
