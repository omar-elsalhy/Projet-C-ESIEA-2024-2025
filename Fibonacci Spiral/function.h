#pragma once

typedef enum Direction {
	UP,
	DOWN,
	RIGHT,
	LEFT
} Direction;


typedef enum Rotation {
	NOROT = 1,
	RIGHTROT=2,
	UTURN = 4,
	LEFTROT=8
} Rotation;


typedef struct Turmite {
	int currentState;//initialised to 0
	int	x;//x position, initialised to WIDTH/2
	int y;//y position, initialised to HEIGHT/2
	int currentColor;//initialised to 0
	Direction dir;//initialised to UP
}Turmite;





//Function for grid creation and initialization
char** createGrid();


//Displays the grid in its current state
int dispGrid(char** grid, Turmite* turm);


//Free a grid
void freeGrid(char** grid);


//Copy a grid into another one
char** copyGrid(char** grid);


//Function to create the rules table (not used, but might be useful for scalability)
int** createRuleTable();


//Turmite creation and initialisation
Turmite* createTurmite();


//Function to calculate the new direction depending on the current direction and the rotation
Direction newDir(Direction currentDir, Rotation rot);


//Function to calculate the new turmite's position on the grid
int moveTurmite(Turmite* turm);


//Function to update the turmite based on its current state and the color of the cell it is on
int updateTurmite(Turmite* turm, char** grid);


