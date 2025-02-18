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
int dispGrid(char** grid)
{
	if (grid == NULL)
		return -1;


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

			else
			{
				//Displaying the grid
				printf("%c", grid[i - 1][j]);
			}
		}

		//Line end
		printf("%c\n", '|');
	}

	return 0;
}




//Initialise grid with the glider pattern
int initGrid(char** grid)
{
	if (grid == NULL)
		return -1;

	grid[0][1] = '#';
	grid[1][2] = '#';
	grid[2][0] = '#';
	grid[2][1] = '#';
	grid[2][2] = '#';


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




//Update grid after 1 iteration
char** updateGrid(char** grid)
{
	if (grid == NULL)
		return NULL;

	char** new_grid = copyGrid(grid);

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			//We first need to find each neighbour cell's coordinates
			//. . .
			//. # .
			//. . .


			int nbAliveNeighbours = 0;

			//Each of the 8 neighbours
			for (int k = 0; k < 3; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					//Coordinates of a neighbour
					int x = 0, y = 0;

					//Neighbour's x coordinate (lines)
					if (i == 0 && k == 0)//1st line and upper neighbours
						x = HEIGHT - 1;
					else if (i == (HEIGHT - 1) && k == 2)//last line and bottom neighbours
						x = 0;
					else if (k == 0)//1st line
						x = i - 1;
					else if (k == 1)//2nd line
						x = i;
					else if (k == 2)//3rd line
						x = i + 1;


					//Neighbour's y coordinate (columns)
					if (j == 0 && l == 0)//1st column and left neighbours
						y = WIDTH - 1;
					else if (j == (WIDTH - 1) && l == 2)//last column and right neighbours
						y = 0;
					else if (l == 0)//1st column
						y = j - 1;
					else if (l == 1)//2nd column
						y = j;
					else if (l == 2)//3rd column
						y = j + 1;


					//We verify if the neighbour is alive or not to increment the number of alive neighbours
					if ((k != 1 || l != 1) && grid[x][y] == '#')//all cases where the neighbour is alive, except the cell itself
						nbAliveNeighbours += 1;

				}
			}


			//Now, we update the current cell based on the number of neighbours it has and the rules

			//Rules reminder
			//Any living cell with two or three living neighbors stays alive in the next generation.
			//Any living cell with fewer than two living neighbors dies of loneliness.
			//Any living cell with more than three living neighbors dies of overpopulation.
			//Any dead cell with exactly three living neighbors becomes a living cell.

			if (grid[i][j] == '#')//any living cell
			{
				if (nbAliveNeighbours == 2 || nbAliveNeighbours == 3)//with two or three living neighbors
					new_grid[i][j] = '#';//stays alive in the next generation

				else if (nbAliveNeighbours < 2)//with fewer than two living neighbors
					new_grid[i][j] = ' ';//dies of loneliness

				else if (nbAliveNeighbours > 3)//with more than three living neighbors
					new_grid[i][j] = ' ';//dies of overpopulation
			}
			else if (grid[i][j] == ' ' && nbAliveNeighbours == 3)//any dead cell with exactly three living neighbors
				new_grid[i][j] = '#';//becomes a living cell

		}
	}

	//We free the old grid and return the new one
	freeGrid(grid);

	return new_grid;
}










//Load grid from file
int loadGrid(FILE* f, char** grid)
{
	//Saving coordinates of the pattern
	int x, y;
	if(fscanf(f, "%d", &y) == EOF)
		return -1;
	if(fscanf(f, "%d", &x) == EOF)
		return -1;


	//Verifying that the pattern fits in the grid
	if (x > HEIGHT || y > WIDTH)
	{
		printf("The pattern's coordinates are outside the grid.\n");
		return -1;
	}


	//Putting the pattern directly into the grid
	char line[255];
	
	//get the first \n
	if(fgets(line, 255, f) == NULL)
		return -1;

	
	int k = x;//line number
	while (!feof (f))
	{
		if(fgets(line, 255, f) == NULL)
			return -1;
		
		int i = 0;//character number (from file)
		int l = y;//column number
		while (line[i] != '\0')
		{
			if (line[i] != ' ' && line[i] != '\n')
			{
				if (line[i] == '.')
					grid[k%HEIGHT][l%WIDTH] = ' ';
				else if (line[i] == '#')
					grid[k%HEIGHT][l%WIDTH] = '#';
				else
					return -1;
				
				l++;//Go to next grid column to write in
			}
			i++;//Go to next character of the line from the file
		}
		k++;//Go to next grid line to write in
	}
	

	return 1;
}


