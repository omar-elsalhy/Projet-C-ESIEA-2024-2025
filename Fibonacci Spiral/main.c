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


	//Grid creation
	char** myGrid = NULL;
	myGrid = createGrid();

	//Turmite creation
	Turmite* myTurmite = NULL;
	myTurmite = createTurmite();



	//Either show all steps or the last one, depending on the chosen mode
	if (strcmp(option, "-all") == 0)
	{
		dispGrid(myGrid, myTurmite);
		for (int i = 0; i < nbOfIterations; i++)
		{
			updateTurmite(myTurmite, myGrid);
			moveTurmite(myTurmite);
			//updateGrid(myGrid, myTurmite);
			dispGrid(myGrid, myTurmite);
		}
	}
	else if (strcmp(option, "-last") == 0)
	{
		for (int i = 0; i < nbOfIterations; i++)
		{
			updateTurmite(myTurmite, myGrid);
			moveTurmite(myTurmite);
		}
		dispGrid(myGrid, myTurmite);
	}
	else
	{
		printf("Error in the number of arguments.\nProgram usage example: ./prog.exe pattern.txt nbIterations [-all/-last]\n");
	}



	//Free memory
	freeGrid(myGrid);
	free(myTurmite);
	myTurmite = NULL;

	return 0;
}