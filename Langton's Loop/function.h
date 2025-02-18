#pragma once

//Function for grid creation and initialization
int** createGrid();


//Displays the grid in its current state
int dispGrid(int** grid);


//Free a grid
void freeGrid(int** grid);


//Copy a grid into another new one
int** copyGrid(int** grid);


//Update grid after 1 iteration
int** updateGrid(int** grid, int* rules);


//Load grid from file
int loadGrid(FILE* f, int** grid);


//Creating and returning the rules table
int* createRulesTable();


//Search a value in the rules table and returns the new state of the cell
//Depends on the cell's current state, as well as its neighbours state
int newState(int currentState, int* rules);

