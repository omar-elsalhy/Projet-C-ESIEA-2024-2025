#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"



int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Error in the number of arguments.\nProgram usage example: ./prog.exe nbIterations [-all/-last]\n");
		return 0;
	}


	//Number of iterations for the program's main loop
	int nbOfIterations = atoi(argv[1]);


	//-all or -last option
	char option[100];
	strcpy(option, argv[2]);


	char** myGrid = NULL;
	myGrid = createGrid();
	initGrid(myGrid);
	


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
		printf("Error in the number of arguments.\nProgram usage example: ./prog.exe nbIterations [-all/-last]\n");
	}
	


	//Free memory
	freeGrid(myGrid);

	return 0;
}