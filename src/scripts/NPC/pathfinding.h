#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "../../../lib/raylib.h"

#define MAX_PATH_LENGTH 128



int findPath(Vector2 start, Vector2 goal, Vector2 *pathOut, int maxPathLength);
bool isTileWalkable(int row, int col); // Make sure this is visible to this module

#endif
