#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"



int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Error in the number of arguments.\nProgram usage example: ./prog.exe pattern.txt nbIterations [-all/-last]\n");
		return 0;
	}


	//Number of iterations for the program's main loop
	int nbOfIterations = atoi(argv[2]);


	//-all or -last option
	char option[100];
	strcpy(option, argv[3]);


	//Grid creation
	char** myGrid = NULL;
	myGrid = createGrid();


	//Reading pattern from file into myGrid
	FILE* f = NULL;
	f = fopen(argv[1], "r");
	if (!f)
	{
		printf("Error in the file name or file loading.\nProgram usage example: ./prog.exe pattern.txt nbIterations [-all/-last]\n");
		freeGrid(myGrid);
		return 0;
	}


	//Loading the pattern into the grid while handling errors
	if (loadGrid(f, myGrid) == -1)
	{
		printf("Error while loading the pattern.\n");
		freeGrid(myGrid);
		fclose(f);
		return 0;
	}



	//Either show all steps or the last one, depending on the chosen mode
	if (strcmp(option, "-all") == 0)
	{
		dispGrid(myGrid);
		for (int i = 0; i < nbOfIterations; i++)
		{
			myGrid = updateGrid(myGrid);
			dispGrid(myGrid);
		}
	}
	else if (strcmp(option, "-last") == 0)
	{
		for (int i = 0; i < nbOfIterations; i++)
		{
			myGrid = updateGrid(myGrid);
		}
		dispGrid(myGrid);
	}
	else
	{
		printf("Error in the number of arguments.\nProgram usage example: ./prog.exe pattern.txt nbIterations [-all/-last]\n");
	}



	//Free memory
	freeGrid(myGrid);
	myGrid=NULL;
	
	fclose(f);
	f=NULL;

	return 0;
}
