#ifndef MAP_H
#define MAP_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../raylib.h"


// Declare pointers for the map and map dimensions
extern int **map;  // A pointer to a 2D array
extern int rows, cols;  // Map dimensions
//extern time_t **lastChangeTimes;

void loadMap(const char *filename);
void drawMap(Camera2D camera);
void updateMap(Camera2D camera);
void freeMap();  // Function to free allocated memory

#endif // MAP_H
