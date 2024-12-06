//Wiktor
#ifndef OBJECTS_H
#define OBJECTS_H

#include "../../raylib.h"

// Function declarations for object handling
void drawTree(int treeIndex, Rectangle block);
void drawWall(int wallIndex, Rectangle block);
void drawObjects(int **objects, int rows, int cols, Texture2D treesTileset, Texture2D wallSet, Camera2D camera);
void updateObjects(int **objects, int rows, int cols, Vector2 worldMousePos);
void freeObjects(int **objects, int rows);

#endif
