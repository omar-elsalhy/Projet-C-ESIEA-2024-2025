#pragma once

//Function for grid creation and initialization
char** createGrid();


//Displays the grid in its current state
int dispGrid(char** grid);


//Initialise grid with the glider pattern
int initGrid(char** grid);


//Free a grid
void freeGrid(char** grid);


//Copy a grid into another one
char** copyGrid(char** grid);


//Update grid after 1 iteration
char** updateGrid(char** grid);

