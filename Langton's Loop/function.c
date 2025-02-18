#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"


#define WIDTH 500
#define HEIGHT 250
#define GRIDWIDTH 100
#define GRIDHEIGHT 50
//grid[HEIGHT][WIDTH]
//grid[250][500] for example


//Function for grid creation and initialization
int** createGrid()
{
	//Grid creation
	int** grid = (int**)malloc(HEIGHT * sizeof(int*));
	if (grid == NULL)
		return NULL;

	for (int i = 0; i < HEIGHT; i++)
	{
		grid[i] = (int*)malloc(WIDTH * sizeof(int));

		if (grid[i] == NULL)
			return NULL;
	}



	//Grid initialization
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			grid[i][j] = 0;//Or -1, we'll see
		}
	}


	return grid;
}




//Displays the grid in its current state
int dispGrid(int** grid)
{
	if (grid == NULL)
		return -1;

	//Calculating the coordinates of the grid's center
	int X1 = HEIGHT / 2 - GRIDHEIGHT / 2;//Start height
	int X2 = (HEIGHT / 2 + (GRIDHEIGHT / 2) + 2);//End height
	int Y1 = WIDTH / 2 - GRIDWIDTH / 2;//Start width
	int Y2 = WIDTH / 2 + GRIDWIDTH / 2;

	//Display the grid with edges
	for (int i = X1; i < X2 + 2; i++)
	{
		//Line beginning
		printf("%c", '|');

		//Rest of the line
		for (int j = Y1; j < Y2; j++)
		{
			if (i == X1 || i == (X2+1))
			{
				printf("%c", '-');
			}

			else
			{
				//Displaying the grid
				if (grid[i - 1][j] == 0)
					printf(" ");
				else
					printf("\033[1;3%dm\%d\033[0m", grid[i - 1][j], grid[i - 1][j]);
			}
		}

		//Line end
		printf("%c\n", '|');
	}

	return 0;
}




//Free a grid
void freeGrid(int** grid)
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
int** copyGrid(int** grid)
{
	if (grid == NULL)
		return NULL;

	int** new_grid = NULL;
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
int** updateGrid(int** grid, int* rules)
{
	if (grid == NULL)
		return NULL;

	int** new_grid = copyGrid(grid);

	if (!new_grid)
		return NULL;

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			//We first need to find the state of each of the 4 neighbours of the cell
			//. N .
			//W # E
			//. S .

			//The 4 neighbours' values
			int N = grid[(i-1+HEIGHT)%HEIGHT][j];
			int E = grid[i][(j+1)%WIDTH];
			int S = grid[(i+1)%HEIGHT][j];
			int W = grid[i][(j-1+WIDTH)%WIDTH];

			//Check the four symmetry variants
			int currentState = grid[i][j]*10000 + N*1000 + E*100 + S*10 + W*1;
			int currentState2 = grid[i][j] * 10000 + E * 1000 + S * 100 + W * 10 + N * 1;
			int currentState3 = grid[i][j] * 10000 + S * 1000 + W * 100 + N * 10 + E * 1;
			int currentState4 = grid[i][j] * 10000 + W * 1000 + N * 100 + E * 10 + S * 1;

			//-1 means the rule wasn't found in the rules table
			int new_state = newState(currentState, rules);
			if(new_state==-1)
				new_state = newState(currentState2, rules);
			if(new_state==-1)
				new_state = newState(currentState3, rules);
			if (new_state == -1)
				new_state = newState(currentState4, rules);
			if (new_state == -1)
				new_state = grid[i][j];//Last case: if there is no rule for this case
			
			new_grid[i][j] = new_state;
		}
	}

	//We free the old grid and return the new one
	freeGrid(grid);

	return new_grid;
}




//Load grid from file
int loadGrid(FILE* f, int** grid)
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
		while (line[i] != '\0' && line[i]!=10)//10 is ASCII code for LineFeed (Enter)
		{
			grid[k % HEIGHT][l % WIDTH] = line[i] - 48;//-48 to get an int from 0 not from 48(=0 in ASCII)
			l++;//Go to next grid column to write in
			i++;//Go to next character of the line from the file
		}
		k++;//Go to next grid line to write in
	}
	

	return 1;
}



 
//Creating and returning the rules table
int* createRulesTable()
{
	int* tab = NULL;
	tab = (int*)malloc(219 * sizeof(int));

	if (!tab)
		return NULL;

	int tab2[219] = { 
	0,
	12,
	20,
	30,
	50,
	63,
	71,
	112,
	122,
	132,
	212,
	220,
	230,
	262,
	272,
	320,
	525,
	622,
	722,
	1022,
	1120,
	2020,
	2030,
	2050,
	2125,
	2220,
	2322,
	5222,
	12321,
	12421,
	12525,
	12621,
	12721,
	12751,
	14221,
	14321,
	14421,
	14721,
	16251,
	17221,
	17255,
	17521,
	17621,
	17721,
	25271,
	100011,
	100061,
	100077,
	100111,
	100121,
	100211,
	100244,
	100277,
	100511,
	101011,
	101111,
	101244,
	101277,
	102026,
	102121,
	102211,
	102244,
	102263,
	102277,
	102327,
	102424,
	102626,
	102644,
	102677,
	102710,
	102727,
	105427,
	111121,
	111221,
	111244,
	111251,
	111261,
	111277,
	111522,
	112121,
	112221,
	112244,
	112251,
	112277,
	112321,
	112424,
	112621,
	112727,
	113221,
	122244,
	122277,
	122434,
	122547,
	123244,
	123277,
	124255,
	124267,
	125275,
	200012,
	200022,
	200042,
	200071,
	200122,
	200152,
	200212,
	200222,
	200232,
	200242,
	200250,
	200262,
	200272,
	200326,
	200423,
	200517,
	200522,
	200575,
	200722,
	201022,
	201122,
	201222,
	201422,
	201722,
	202022,
	202032,
	202052,
	202073,
	202122,
	202152,
	202212,
	202222,
	202272,
	202321,
	202422,
	202452,
	202520,
	202552,
	202622,
	202722,
	203122,
	203216,
	203226,
	203422,
	204222,
	205122,
	205212,
	205222,
	205521,
	205725,
	206222,
	206722,
	207122,
	207222,
	207422,
	207722,
	211222,
	211261,
	212222,
	212242,
	212262,
	212272,
	214222,
	215222,
	216222,
	217222,
	222272,
	222442,
	222462,
	222762,
	222772,
	300013,
	300022,
	300041,
	300076,
	300123,
	300421,
	300622,
	301021,
	301220,
	302511,
	401120,
	401220,
	401250,
	402120,
	402221,
	402326,
	402520,
	403221,
	500022,
	500215,
	500225,
	500232,
	500272,
	500520,
	502022,
	502122,
	502152,
	502220,
	502244,
	502722,
	512122,
	512220,
	512422,
	512722,
	600011,
	600021,
	602120,
	612125,
	612131,
	612225,
	700077,
	701120,
	701220,
	701250,
	702120,
	702221,
	702251,
	702321,
	702525,
	702720 };


	for (int i = 0; i < 219; i++)
		tab[i] = tab2[i];

	return tab;
}




//Search a value in the rules table and returns the new state of the cell
//Depends on the cell's current state, as well as its neighbours state
int newState(int currentState, int *rules)
{
	if (!rules)
		return 0;

	for (int i = 0; i < 219; i++)
	{
		if ((rules[i]/10) == currentState)
		{
			return (rules[i] % 10);
		}
	}

	//If rule not found
	return -1;
}