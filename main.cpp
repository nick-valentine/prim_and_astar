#include <SFML/Graphics.hpp>
#include <iostream>

#include "Maze.h"


int main(int argc, char ** argv)
{
	int width, height;
	
	std::cout<<"Width: ";
	std::cin>>width;
	std::cout<<"Height: ";
	std::cin>>height;
	
	std::cout<<"Click to draw\nQ to be done drawing\nA to automatically make a maze\nC to clear\nS to solve\nE to edit\n1 to place start block\n2 to place stop block\n";
	
	//Size of window needed will be the number of blocks the user wants
	//multiplied by the size of each block
	int screenWidth = width * MazeElement::getWidth();
	int screenHeight = height * MazeElement::getHeight();
	
	sf::RenderWindow window(sf::VideoMode( screenWidth, screenHeight ), "Maze!");
	window.setFramerateLimit(30);
	
	Maze myMaze( width, height );
	
	while(window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		//Get input
		
		//Update
		myMaze.update(window);
		
		//Draw
		window.clear();
		myMaze.draw(window);
		window.display();
	}

	return 0;
}
