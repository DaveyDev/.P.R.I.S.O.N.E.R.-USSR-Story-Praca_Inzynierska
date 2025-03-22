//Wiktor
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../lib/raylib.h"
#include "map.h"
#include "../textures.h"
#include "../global.h"
#include "objects.h"
#include "../player/player.h"




TileMapping neighbours_to_atlas_coord[] = {
    {{Grass, Grass, Grass, Grass}, {2, 1}},
    {{Dirt, Dirt, Dirt, Grass}, {1, 3}},
    {{Dirt, Dirt, Grass, Dirt}, {0, 0}},
    {{Dirt, Grass, Dirt, Dirt}, {0, 2}},
    {{Grass, Dirt, Dirt, Dirt}, {3, 3}},
    {{Dirt, Grass, Dirt, Grass}, {1, 0}},
    {{Grass, Dirt, Grass, Dirt}, {3, 2}},
    {{Dirt, Dirt, Grass, Grass}, {3, 0}},
    {{Grass, Grass, Dirt, Dirt}, {1, 2}},
    {{Dirt, Grass, Grass, Grass}, {1, 1}},
    {{Grass, Dirt, Grass, Grass}, {2, 0}},
    {{Grass, Grass, Dirt, Grass}, {2, 2}},
    {{Grass, Grass, Grass, Dirt}, {3, 1}},
    {{Dirt, Grass, Grass, Dirt}, {2, 3}},
    {{Grass, Dirt, Dirt, Grass}, {0, 1}},
    {{Dirt, Dirt, Dirt, Dirt}, {0, 3}}
};



void loadMap(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        exit(EXIT_FAILURE);
    }

    // Read the number of rows and columns from the file
    fscanf(file, "%d %d", &rows, &cols);

    // Allocate memory for the map, objects, and the lastChangeTimes arrays
    map = (int **)malloc(rows * sizeof(int *));
    objects = (int **)malloc(rows * sizeof(int *));
    details = (int **)malloc(rows * sizeof(int *));
    
    
    for (int i = 0; i < rows; i++) {
        map[i] = (int *)malloc(cols * sizeof(int));
        objects[i] = (int *)malloc(cols * sizeof(int)); // Additional array for objects
        details[i] = (int *)malloc(cols * sizeof(int)); //2nd layer read
    }

    // Read the tile and object values into the map and objects arrays
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int tile, object, detail;
            //fscanf(file, "%d:%d", &tile, &object);  // Read tile:object format
            fscanf(file, "%d:%d:%d", &tile, &object, &detail);
            map[row][col] = tile;
            objects[row][col] = object;  // Store 1st map layer
            details[row][col] = detail;  // Store 2nd map layer
            
        }
    }

    fclose(file);
}

void saveMap(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    // Write the number of rows and columns
    fprintf(file, "%d %d\n", rows, cols);

    // Write tile and object data in "tile:object" format
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            //fprintf(file, "%d:%d ", map[row][col], objects[row][col]);
            fprintf(file, "%d:%d:%d ", map[row][col], objects[row][col], details[row][col]);

        }
        fprintf(file, "\n"); // Newline after each row
    }

    fclose(file);
}


Rectangle calculateTile(int row, int col) {


    // Ensure we're not going out of bounds
    if (row >= rows - 1 || col >= cols - 1) {
        return (Rectangle){ 0, 0, 32, 32 };  // Return default if out of bounds
    } 

    // Get the tile types for the 2x2 neighbors
    int topLeft = map[row][col];
    int topRight = map[row][col + 1];
    int bottomLeft = map[row + 1][col];
    int bottomRight = map[row + 1][col + 1];

    int neighbours[4] = {topLeft, topRight, bottomLeft, bottomRight};

    // Loop through the mappings and find a match
    for (int i = 0; i < sizeof(neighbours_to_atlas_coord) / sizeof(neighbours_to_atlas_coord[0]); i++) {
        if (neighbours[0] == neighbours_to_atlas_coord[i].neighbors[0] &&
            neighbours[1] == neighbours_to_atlas_coord[i].neighbors[1] &&
            neighbours[2] == neighbours_to_atlas_coord[i].neighbors[2] &&
            neighbours[3] == neighbours_to_atlas_coord[i].neighbors[3]) {
            
            // Return the corresponding atlas coordinates
            Vector2 atlasCoord = neighbours_to_atlas_coord[i].atlasCoord;
            return (Rectangle){ atlasCoord.x * 32, atlasCoord.y * 32, 32, 32 };
        }
    }

    // Default if no match is found
    return (Rectangle){ 0, 0, 32, 32 };
}



void updateMap(Camera2D camera) {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);

    

    int col = (int)(worldMousePos.x / 32); 
    int row = (int)(worldMousePos.y / 32);

    // Early exit if the mouse is out of bounds
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return;
    }

    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (map[row][col] == Grass) {
            //map[row][col] = Dirt;
        } else if (map[row][col] == Dirt) {
            //map[row][col] = Grass;
        }
    }

    // Right click to toggle between objects (objects array)
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        updateObjects(objects, rows, cols, worldMousePos);
    }
    
}


void freeMap() {
    // Free the allocated memory for the map, 1st and 2nd layer
    for (int i = 0; i < rows; i++) {
        free(map[i]);
        free(objects[i]);
        free(details[i]);
    }
    free(map);
    free(objects);
    free(details);
}


void drawMap(Camera2D camera) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Calculate visible tile range
    int startCol = (int)((camera.target.x - 256 / camera.zoom) / 32);
    int startRow = (int)((camera.target.y - 256 / camera.zoom) / 32);
    int endCol = (int)((camera.target.x + screenWidth + 256 / camera.zoom) / 32);
    int endRow = (int)((camera.target.y + screenHeight + 256 / camera.zoom) / 32);

    // Clamp values to prevent out-of-bounds
    startCol = (startCol < 0) ? 0 : startCol;
    startRow = (startRow < 0) ? 0 : startRow;
    endCol = (endCol >= cols) ? cols - 1 : endCol;
    endRow = (endRow >= rows) ? rows - 1 : endRow;

    // Convert player screen position to world position
    Vector2 playerWorldPos = GetScreenToWorld2D(player.position, camera);
    int playerRow = (int)(playerWorldPos.y / 32) + 1;  // Convert world Y to tile row

    BeginMode2D(camera);

    // **Step 1: Draw background tiles**
    for (int row = startRow; row <= endRow; row++) {
        for (int col = startCol; col <= endCol; col++) {
            Vector2 position = { col * 32, row * 32 };
            Rectangle tileSource = calculateTile(row, col);
            DrawTextureRec(grassTileset, tileSource, position, WHITE);
        }
    }

    // **Step 2: Draw details layer (always under the player)**
    for (int row = startRow; row <= endRow; row++) {
        for (int col = startCol; col <= endCol; col++) {
            Vector2 position = { col * 32, row * 32 };
            Rectangle block = { position.x, position.y, 32, 32 };

            if (objects[row][col] >= 2000 && objects[row][col] <= 2999) {
                drawPlaceable(objects[row][col] - 2000, block);
            }
            if (details[row][col] >= 2000 && details[row][col] <= 2999) {
                drawPlaceable(details[row][col] - 2000, block);
            }
            
        }
    }

    // **Step 3: Draw objects and the player at the right moment**
    for (int row = startRow; row <= endRow; row++) {
        for (int col = startCol; col <= endCol; col++) {
            Vector2 position = { col * 32, row * 32 };
            Rectangle block = { position.x, position.y, 32, 32 };

            // **Draw the player at the correct row**
            if (playerRow == row) {
                EndMode2D();
                drawPlayer(&player);
                BeginMode2D(camera);
            }

            // **Draw objects**
            if (objects[row][col] >= 1000 && objects[row][col] <= 1999) {
                drawTree(objects[row][col] - 1000, block);
            } else if (objects[row][col] == 11) {
                drawWall(row, col, wallSet);
            }
        }
    }

    EndMode2D();
}


