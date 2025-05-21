#include "pathfinding.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../../../lib/raymath.h"
#include "../map/map.h"
#include "../map/objects.h"
#include "npc.h"
#include "../items/idList.h"

struct Node {
    int row, col;
    int gCost, hCost;
    int fCost;
    struct Node* parent;
    bool closed, open;
};

typedef struct Node Node;


#define MAX_NODES 10000
#define TILE_SIZE 32

static Node nodes[MAX_NODES];

static int heuristic(int row1, int col1, int row2, int col2) {
    return abs(row1 - row2) + abs(col1 - col2);
}

int findPath(Vector2 startPos, Vector2 goalPos, Vector2 *pathOut, int maxPathLength,
             NPC *self, NPC *others, int npcCount) {
    if (rows <= 0 || cols <= 0) return 0;

int startRow = (int)(startPos.y / TILE_SIZE);
int startCol = (int)(startPos.x / TILE_SIZE);
int goalRow = (int)(goalPos.y / TILE_SIZE);
int goalCol = (int)(goalPos.x / TILE_SIZE);

if (startRow < 0 || startCol < 0 || goalRow < 0 || goalCol < 0 ||
    startRow >= rows || startCol >= cols || goalRow >= rows || goalCol >= cols)
    return 0;


    int nodeCount = 0;

    Node *grid[100][100] = {0};

    // Init all nodes
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Node *n = &nodes[nodeCount++];
            if (nodeCount >= MAX_NODES) {
            printf("Too many nodes! Map too big or MAX_NODES too small\n");
            return 0;
            }

            n->row = r;
            n->col = c;
            n->gCost = 0;
            n->hCost = heuristic(r, c, goalRow, goalCol);
            n->fCost = 0;
            n->parent = NULL;
            n->closed = false;
            n->open = false;
            grid[r][c] = n;
        }
    }

    Node *startNode = grid[startRow][startCol];
    startNode->open = true;

    Node *openList[MAX_NODES];
    int openCount = 0;
    openList[openCount++] = startNode;

    while (openCount > 0) {
        // Find lowest fCost node
        int lowest = 0;
        for (int i = 1; i < openCount; i++) {
            if (openList[i]->fCost < openList[lowest]->fCost) lowest = i;
        }

        Node *current = openList[lowest];

        // Remove from open list
        openCount--;
        for (int i = lowest; i < openCount; i++) openList[i] = openList[i+1];

        current->closed = true;

        // Reached goal
        if (current->row == goalRow && current->col == goalCol) {
            int count = 0;
            while (current && count < maxPathLength) {
                pathOut[count++] = (Vector2){ current->col * TILE_SIZE + TILE_SIZE / 2,
                                              current->row * TILE_SIZE + TILE_SIZE / 2 };
                current = current->parent;
            }

            // Reverse path
            for (int i = 0; i < count / 2; i++) {
                Vector2 temp = pathOut[i];
                pathOut[i] = pathOut[count - i - 1];
                pathOut[count - i - 1] = temp;
            }

            return count;
        }

        // Check neighbors
        int offsets[8][2] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // cardinal
            {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // diagonals
        };

        for (int i = 0; i < 8; i++) {

            int nr = current->row + offsets[i][0];
            int nc = current->col + offsets[i][1];

            // Prevent cutting corners diagonally through walls
            //if (abs(offsets[i][0]) + abs(offsets[i][1]) == 2) { // diagonal
            //    int r1 = current->row + offsets[i][0];
            //    int c1 = current->col;
            //    int r2 = current->row;
            //    int c2 = current->col + offsets[i][1];
            //    if (!isTileWalkable(r1, c1) || !isTileWalkable(r2, c2)) continue;
            //}

            if (abs(offsets[i][0]) == 1 && abs(offsets[i][1]) == 1) { // diagonal
                int r1 = current->row + offsets[i][0];
                int c1 = current->col;
                int r2 = current->row;
                int c2 = current->col + offsets[i][1];
                if (!isTileWalkable(r1, c1) || !isTileWalkable(r2, c2)) continue;
            }


            //if (!isTileWalkable(nr, nc)) continue;
            if (!isTileWalkable(nr, nc)) continue;

            // NEW: avoid other NPCs
            if (self && others && npcCount > 0) {
    Vector2 tileCenter = { nc * TILE_SIZE + TILE_SIZE / 2, nr * TILE_SIZE + TILE_SIZE / 2 };

    for (int i = 0; i < npcCount; i++) {
        if (&others[i] == self) continue;

        Vector2 otherPos = others[i].position;
        float dist = Vector2Distance(tileCenter, otherPos);
        if (dist < 20.0f) {
            continue; // this tile is too close to another NPC
        }
    }
}



            Node *neighbor = grid[nr][nc];
            if (neighbor->closed) continue;

            //int tentativeG = current->gCost + 1;
            int moveCost = (abs(offsets[i][0]) + abs(offsets[i][1]) == 2) ? 14 : 10; // diagonal = ~1.4x
            int tentativeG = current->gCost + moveCost;

            if (!neighbor->open || tentativeG < neighbor->gCost) {
                neighbor->gCost = tentativeG;
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = current;

                if (!neighbor->open) {
                    neighbor->open = true;
                    openList[openCount++] = neighbor;
                }
            }
        }
    }

    return 0; // No path found
}


bool isTileWalkable(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) return false;
    if (isWallLike(objects[row][col])) return false;

    if (
        (objects[row][col] != 0 
        && objects[row][col] != PATROL_BLOCK
        && objects[row][col] != WINNING_BLOCK
        && objects[row][col] != GREY_DOOR
        && objects[row][col] != OPEN_GREY_DOOR
        && objects[row][col] != LIGHTGREY_DOOR
        && objects[row][col] != OPEN_LIGHTGREY_DOOR
        && objects[row][col] != STONE_FLOOR
        && objects[row][col] != OAK_WOODEN_FLOOR
        && objects[row][col] != WOODEN_FLOOR)
        ||
        (details[row][col] == IRON_BARS
        || details[row][col] == LEFT_TABLE
        || details[row][col] == MIDDLE_TABLE
        || details[row][col] == RIGHT_TABLE
        || details[row][col] == SMALL_TABLE)
    ) return false;

    return true;
}



