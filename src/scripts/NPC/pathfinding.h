#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "../../../lib/raylib.h"
#include "npc.h"

#define MAX_PATH_LENGTH 128



int findPath(Vector2 startPos, Vector2 goalPos, Vector2 *pathOut, int maxPathLength,
             NPC *self, NPC *others, int npcCount);
bool isTileWalkable(int row, int col); // Make sure this is visible to this module

#endif
