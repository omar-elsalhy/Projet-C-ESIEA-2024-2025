#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"


#define WIDTH 100
#define HEIGHT 50
//grid[HEIGHT][WIDTH]
//grid[50][100] for example


//Function for grid creation and initialization
char** createGrid()
{
	//Grid creation
	char** grid = (char**)malloc(HEIGHT * sizeof(char*));
	if (grid == NULL)
		return NULL;

	for (int i = 0; i < HEIGHT; i++)
	{
		grid[i] = (char*)malloc(WIDTH * sizeof(char));

		if (grid[i] == NULL)
			return NULL;
	}



	//Grid initialization
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			grid[i][j] = ' ';
		}
	}


	return grid;
}




//Displays the grid in its current state
int dispGrid(char** grid, Turmite* turm)
{
	if (grid == NULL)
		return -1;

	int X = turm->x;
	int Y = turm->y;

	//Display the grid with edges
	for (int i = 0; i < (HEIGHT + 2); i++)
	{
		//Line beginning
		printf("%c", '|');

		//Rest of the line
		for (int j = 0; j < WIDTH; j++)
		{
			if (i == 0 || i == (HEIGHT + 1))
			{
				printf("%c", '-');
			}

			else if (i-1==X && j==Y)//Where the turmite is located
			{
				if(turm->currentState==0)//. in red if the cell is dead
					printf("\033[1;31m.\033[0m");
				else if (turm->currentState == 1)//# in red if the cell is alive
					printf("\033[1;31m#\033[0m");
			}

			else
			{
				//Displaying the grid
				if(grid[i - 1][j] == '#')//Living cell in green
					printf("\033[1;32m#\033[0m");

				else if (grid[i - 1][j] == ' ')//Blank cell
					printf(" ");
			}
		}

		//Line end
		printf("%c\n", '|');
	}

	return 0;
}




//Free a grid
void freeGrid(char** grid)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		free(grid[i]);
		grid[i] = NULL;
	}
	free(grid);
	grid = NULL;
}




//Copy a grid into another new one
char** copyGrid(char** grid)
{
	if (grid == NULL)
		return NULL;

	char** new_grid = NULL;
	new_grid = createGrid();

	//Copy grid
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			new_grid[i][j] = grid[i][j];
		}
	}

	return new_grid;
}




//Function to create the rules table
int** createRuleTable()
{
	//Table creation
	int** tab = (int**)malloc(HEIGHT * sizeof(int*));
	if (tab == NULL)
		return NULL;

	for (int i = 0; i < HEIGHT; i++)
	{
		tab[i] = (int*)malloc(WIDTH * sizeof(int));

		if (tab[i] == NULL)
			return NULL;
	}


	//Filling the rules table with the default values
	int tab2[4][5] = { {0, 0, 1, 8, 1}, {0, 1, 1, 8, 1}, {1, 0, 1, 2, 1}, {1, 1, 0, 1, 0} };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			tab[i][j] = tab2[i][j];
		}
	}

	return 0;
}




//Turmite creation and initialisation
Turmite* createTurmite()
{
	//Turmite creation
	Turmite* turmite = (Turmite*)malloc(1 * sizeof(Turmite));
	if (turmite == NULL)
		return NULL;

	//Turmite initialisation
	turmite->currentState = 0;
	turmite->x = HEIGHT / 2;
	turmite->y = WIDTH / 2;
	turmite->currentColor = 0;
	turmite->dir = UP;

	return turmite;
}




//Function to calculate the new direction depending on the current direction and the rotation
Direction newDir(Direction currentDir, Rotation rot)
{
	//By default - just in case
	Direction dir = UP;

	//4 cases
	if (rot == NOROT)
		dir = currentDir;

	else if (rot == RIGHTROT)
	{
		if (currentDir == UP)
			dir = RIGHT;
		else if (currentDir == RIGHT)
			dir = DOWN;
		else if (currentDir == DOWN)
			dir = LEFT;
		else if (currentDir == LEFT)
			dir = UP;
	}

	else if (rot == UTURN)
	{
		if (currentDir == UP)
			dir = DOWN;
		else if (currentDir == RIGHT)
			dir = LEFT;
		else if (currentDir == DOWN)
			dir = UP;
		else if (currentDir == LEFT)
			dir = RIGHT;
	}

	else if (rot == LEFTROT)
	{
		if (currentDir == UP)
			dir = LEFT;
		else if (currentDir == RIGHT)
			dir = UP;
		else if (currentDir == DOWN)
			dir = RIGHT;
		else if (currentDir == LEFT)
			dir = DOWN;
	}


	return dir;
}




//Function to calculate the new turmite's position on the grid
int moveTurmite(Turmite* turm)
{
	if (!turm)
		return -1;

	if (turm->dir == UP)
		turm->x = (turm->x - 1 + HEIGHT) % HEIGHT;

	else if (turm->dir == DOWN)
		turm->x = (turm->x + 1) % HEIGHT;

	else if (turm->dir == RIGHT)
		turm->y = (turm->y + 1) % WIDTH;

	else if (turm->dir == LEFT)
		turm->y = (turm->y - 1 + WIDTH) % WIDTH;

	return 1;
}




//Function to update the turmite based on its current state and the color of the cell it is on
int updateTurmite(Turmite* turm, char **grid)
{
	if (!turm || !grid)
		return -1;

	//Rules table
	//int tab[4][5] = { {0, 0, 1, 8, 1}, {0, 1, 1, 8, 1}, {1, 0, 1, 2, 1}, {1, 1, 0, 1, 0} };

	//First update the turm's current state per the cell's current state
	if(grid[turm->x][turm->y] == 0)
		turm->currentState = 0;
	else if (grid[turm->x][turm->y] == 1)
		turm->currentState = 1;


	//Reading the current cell's color
	//current state is = grid[turm->x][turm->y]
	int currentColor = -1;
	if (grid[turm->x][turm->y] == ' ')
		currentColor = 0;
	else if (grid[turm->x][turm->y] == '#')
		currentColor = 1;
	

	//Update the state, color, and direction of the turmite based on its current state and the color of the cell it is on
	//4 cases
	//Case 1 and 2
	if (turm->currentState == 0)
	{
		turm->currentColor = 1;
		turm->currentState = 1;
		turm->dir = newDir(turm->dir, LEFTROT);
	}

	//Case 3
	else if (turm->currentState == 1 && currentColor == 0)
	{
		turm->currentColor = 1;
		turm->currentState = 1;
		turm->dir = newDir(turm->dir, RIGHTROT);
	}

	//Case 4
	else if (turm->currentState == 1 && currentColor == 1)
	{
		turm->currentColor = 0;
		turm->currentState = 0;
		turm->dir = newDir(turm->dir, NOROT);
	}



	//Put the turmite in the grid
	//Update the cells state : alive or not
	if (turm->currentState == 0)
	{
		grid[turm->x][turm->y] = ' ';
	}
	else if (turm->currentState == 1)
	{
		grid[turm->x][turm->y] = '#';
	}



	return 1;
}

