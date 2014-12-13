CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lsfml-system -lsfml-window -lsfml-graphics
EXECUTABLES=Maze
SOURCES=main.cpp MazeElement.cpp Maze.cpp MazeMaker.cpp AStar.cpp
OBJECTS=$(SOURCES:.cpp=.o)
HEADERS=$(SOURCES:.cpp=.h)

all: $(SOURCES) $(EXECUTABLES)

$(EXECUTABLES): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLES) $(LDFLAGS)
	
.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm *.o $(EXECUTABLES)
