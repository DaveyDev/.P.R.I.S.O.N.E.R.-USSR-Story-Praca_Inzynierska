//Wiktor
#include "objects.h"
#include "../../../lib/raylib.h"
#include "../textures.h"
#include <stdlib.h>
#include "../global.h"
#include <stdio.h>
#include "../items/idList.h"
#include "../map/map.h"
#include "../sound/soundManager.h"

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

void drawPlaceable(int placeableIndex, Rectangle block) {
    int placeableWidth = 32;  // Width of the tree in the texture atlas
    int placeableHeight = 32; // Height of the tree in the texture atlas
    int stonesPerRow = placeableTileset.width / placeableWidth;

    // Calculate the x and y coordinates in the atlas
    int row = placeableIndex / stonesPerRow;
    int col = placeableIndex % stonesPerRow;

    Rectangle stoneSource = { col * placeableWidth, row * placeableHeight, placeableWidth, placeableHeight };
    DrawTextureRec(placeableTileset, stoneSource, (Vector2){ block.x, block.y}, WHITE);  // Adjust y offset as needed
}

bool isWallLike(int object) {
    return (object == StoneWall || object == GREY_DOOR || object == LIGHTGREY_DOOR); // Add more IDs if needed
}
bool isWallLikeDetail(int detail) {
    return (detail == GREY_DOOR || detail == LIGHTGREY_DOOR || detail == OPEN_GREY_DOOR || detail == OPEN_LIGHTGREY_DOOR); // Add more IDs if needed
}

Rectangle calculateWallTile(int row, int col) {
    
    
    int topWall = (row > 0 && isWallLike(objects[row-1][col]) || isWallLikeDetail(details[row-1][col])) ? StoneWall : 0;
    int bottomWall = (isWallLike(objects[row+1][col]) || isWallLikeDetail(details[row+1][col])) ? StoneWall : 0;
    int leftWall = (col > 0 && isWallLike(objects[row][col-1]) || isWallLikeDetail(details[row][col-1])) ? StoneWall : 0;
    int rightWall = (isWallLike(objects[row][col+1]) || isWallLikeDetail(details[row][col+1])) ? StoneWall : 0;


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


void openDoor(int row, int col){
    if(details[row][col] == GREY_DOOR) details[row][col] = OPEN_GREY_DOOR;
    if(details[row][col] == LIGHTGREY_DOOR) details[row][col] = OPEN_LIGHTGREY_DOOR;
    playOpenDoorSound();

}
void closeDoor(int row, int col){
    if(details[row][col] == OPEN_GREY_DOOR) details[row][col] = GREY_DOOR;
    if(details[row][col] == OPEN_LIGHTGREY_DOOR) details[row][col] = LIGHTGREY_DOOR;
    playCloseDoorSound();
}