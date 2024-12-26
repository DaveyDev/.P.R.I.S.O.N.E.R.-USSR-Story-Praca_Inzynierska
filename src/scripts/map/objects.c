//Wiktor
#include "objects.h"
#include "../../raylib.h"
#include "../textures.h"
#include <stdlib.h>
#include "../global.h"
#include <stdio.h>

WallMapping wall_neighbours_to_atlas_coord[] = { // top, bottom, left, right
    {{StoneWall, StoneWall, StoneWall, StoneWall}, {1, 1}, {1, 1}},
    {{StoneWall, StoneWall, StoneWall, 0}, {0, 1}, {-1, 1}},
    {{StoneWall, StoneWall, 0, StoneWall}, {0, 1}, {1, 1}},
    {{StoneWall, StoneWall, 0, 0}, {2, 1}, {1, 1}},
    {{StoneWall, 0, StoneWall, StoneWall}, {1, 2}, {1, 1}},
    {{StoneWall, 0, StoneWall, 0}, {0, 2}, {-1, 1}}, 
    {{StoneWall, 0, 0, StoneWall}, {0, 2}, {1, 1}},
    {{StoneWall, 0, 0, 0}, {3, 1}, {1, 1}},
    {{0, StoneWall, StoneWall, StoneWall}, {2, 0}, {1, 1}},
    {{0, StoneWall, StoneWall, 0}, {0, 0}, {-1, 1}}, 
    {{0, StoneWall, 0, StoneWall}, {0, 3}, {1, 1}},
    {{0, StoneWall, 0, 0}, {3, 0}, {1, 1}},
    {{0, 0, StoneWall, StoneWall}, {1, 0}, {1, 1}},
    {{0, 0, StoneWall, 0}, {2, 2}, {-1, 1}},
    {{0, 0, 0, StoneWall}, {2, 2}, {1, 1}},
    {{0, 0, 0, 0}, {3, 2}, {1, 1}},
};


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
/*
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
}*/
Rectangle calculateWallTile(int row, int col) {
    int topWall = objects[row-1][col];
    int bottomWall = objects[row+1][col];
    int leftWall = objects[row][col-1];
    int rightWall = objects[row][col+1];

    int neighbours[4] = {topWall, bottomWall, leftWall, rightWall};

    for (int i = 0; i < sizeof(wall_neighbours_to_atlas_coord) / sizeof(wall_neighbours_to_atlas_coord[0]); i++) {
        if (neighbours[0] == wall_neighbours_to_atlas_coord[i].neighbors[0] &&
            neighbours[1] == wall_neighbours_to_atlas_coord[i].neighbors[1] &&
            neighbours[2] == wall_neighbours_to_atlas_coord[i].neighbors[2] &&
            neighbours[3] == wall_neighbours_to_atlas_coord[i].neighbors[3]) {

            Vector2 atlasCoord = wall_neighbours_to_atlas_coord[i].atlasCoord;
            int flipTileX = wall_neighbours_to_atlas_coord[i].flipTiles[0]; // Access flipTiles
            int flipTileY = wall_neighbours_to_atlas_coord[i].flipTiles[1]; // (if needed)
            
            return (Rectangle){ atlasCoord.x * 32, atlasCoord.y * 32, flipTileX * 32, flipTileY * 32 };
            //return (Rectangle){ 1, 1, 32, 32 };
        }
    }

    // Default rectangle for unmatched wall types
    return (Rectangle){ 0, 0, 32, 32 };
}

void drawWall(int row, int col, Texture2D wallSet) {
    Rectangle block = {
        col * 32,
        row * 32,
        32,
        32
    };


    
    Rectangle wallSource = calculateWallTile(row, col);
    // Assuming wallSource is a Rectangle
    //printf("Rectangle: x = %f, y = %f, width = %f, height = %f\n", 
        //wallSource.x, wallSource.y, wallSource.width, wallSource.height);

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
                case 3:  // Another tree
                    drawTree(2, block);
                    break;
                case 4:  // Another tree
                    drawTree(3, block);
                    break;
                case 5:  // Another tree
                    drawTree(0, block);
                    break;
                case 11:  // Wall
                    //drawWall(0, block);
                    //drawWall(row, col, objects, rows, cols, wallSet, block);
                    drawWall(row, col, wallSet);
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
        //if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            if (objects[row][col] == 0) {
                objects[row][col] = 11;  // Toggle to tree
            } else if (objects[row][col] == 11) {
                objects[row][col] = 0;  // Toggle to empty
            } else if (objects[row][col] == 4) {
                objects[row][col] = 0;
            }
        //}
    }
}

// Free the objects memory
void freeObjects(int **objects, int rows) {
    for (int i = 0; i < rows; i++) {
        free(objects[i]);
    }
    free(objects);
}
