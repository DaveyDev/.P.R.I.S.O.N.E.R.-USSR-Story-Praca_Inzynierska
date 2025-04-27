//Wiktor
#ifndef MAP_H
#define MAP_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../lib/raylib.h"
#include "../player/player.h"


// Declare pointers for the map and map dimensions
extern int **map;  // A pointer to a 2D array
extern int **objects;   // Objects (e.g., trees, placeables)
extern int **details;   // Additional layer (e.g., doors, overlays)
extern int rows, cols;  // Map dimensions
//extern time_t **lastChangeTimes;
//extern int objects;

// Define your tile types
enum TileType {
    Grass = 1,
    Dirt = 2
};

// Define a struct to represent tile neighbors and atlas coordinates
typedef struct {
    int neighbors[4];    // Stores the 4 neighbors (top-left, top-right, bottom-left, bottom-right)
    Vector2 atlasCoord;  // Stores the atlas coordinates
} TileMapping;

extern TileMapping neighbours_to_atlas_coord[];


void loadMap(const char *filename);
void saveMap(const char *filename);
void drawMap(Camera2D camera);
void updateMap(Camera2D camera);
void freeMap();  // Function to free allocated memory
Rectangle calcutaleTile(int row, int col);
void drawTree(int treeIndex, Rectangle block);
void editMap(Camera2D cameraE);
void findPatrolPointsFromMap();

#endif // MAP_H
