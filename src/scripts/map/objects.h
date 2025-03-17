//Wiktor
#ifndef OBJECTS_H
#define OBJECTS_H

#include "../../../lib/raylib.h"

// Function declarations for object handling
void drawTree(int treeIndex, Rectangle block);
//void drawWall(int wallIndex, Rectangle block);
void drawWall(int row, int col, Texture2D wallSet);
void drawPlaceable(int placeableIndex, Rectangle block);
void drawObjects(int **objects, int rows, int cols, Texture2D treesTileset, Texture2D wallSet, Camera2D camera);
void updateObjects(int **objects, int rows, int cols, Vector2 worldMousePos);
void freeObjects(int **objects, int rows);
bool isWallLike(int object);
bool isWallLikeDetail(int detail);
void openDoor(int row, int col);
void closeDoor(int row, int col);

// Define your tile types
enum WallType {
    StoneWall = 11,
    //Dirt = 2
};

// Define a struct to represent tile neighbors and atlas coordinates
typedef struct {
    int neighbors[4];    // Stores the 4 neighbors (left, right, top, bottom)
    Vector2 atlasCoord;  // Stores the atlas coordinates
    int flipTiles[2];
} WallMapping;

extern WallMapping wall_neighbours_to_atlas_coord[];


#endif
