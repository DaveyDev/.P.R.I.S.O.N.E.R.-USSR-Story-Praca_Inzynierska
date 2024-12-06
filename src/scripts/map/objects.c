//Wiktor
#include "objects.h"
#include "../../raylib.h"
#include "../textures.h"
#include <stdlib.h>

// Draw tree function
void drawTree(int treeIndex, Rectangle block) {
    int treeWidth = 64;  // Width of the tree in the texture atlas
    int treeHeight = 144; // Height of the tree in the texture atlas
    int treesPerRow = treesTileset.width / treeWidth;

    // Calculate the x and y coordinates in the atlas
    int row = treeIndex / treesPerRow;
    int col = treeIndex % treesPerRow;

    Rectangle treeSource = { col * treeWidth, row * treeHeight, treeWidth, treeHeight };
    DrawTextureRec(treesTileset, treeSource, (Vector2){ block.x - 16, block.y - 120 }, WHITE);  // Adjust y offset as needed
}

// Draw wall function
void drawWall(int wallIndex, Rectangle block) {
    int wallWidth = 32;  // Width of the wall tile
    int wallHeight = 32; // Height of the wall tile
    int wallsPerRow = wallSet.width / wallWidth;

    // Calculate the x and y coordinates in the atlas
    int row = wallIndex / wallsPerRow;
    int col = wallIndex % wallsPerRow;

    Rectangle wallSource = { col * wallWidth, row * wallHeight, wallWidth, wallHeight };
    DrawTextureRec(wallSet, wallSource, (Vector2){ block.x, block.y }, WHITE);
}

// Function to draw all objects in the map
void drawObjects(int **objects, int rows, int cols, Texture2D treesTileset, Texture2D wallSet, Camera2D camera) {
    BeginMode2D(camera);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Rectangle block = { col * 32, row * 32, 32, 32 };

            switch (objects[row][col]) {
                case 1:  // Tree
                    drawTree(0, block);  // Adjust tree index if needed
                    break;
                case 2:  // Another tree
                    drawTree(1, block);
                    break;
                case 11:  // Wall
                    drawWall(0, block);
                    break;
            }
        }
    }
    EndMode2D();
}

// Update the objects based on input
void updateObjects(int **objects, int rows, int cols, Vector2 worldMousePos) {
    int col = (int)(worldMousePos.x / 32);
    int row = (int)(worldMousePos.y / 32);

    // Check boundaries
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            if (objects[row][col] == 0) {
                objects[row][col] = 1;  // Toggle to tree
            } else if (objects[row][col] == 1) {
                objects[row][col] = 0;  // Toggle to empty
            }
        }
    }
}

// Free the objects memory
void freeObjects(int **objects, int rows) {
    for (int i = 0; i < rows; i++) {
        free(objects[i]);
    }
    free(objects);
}
