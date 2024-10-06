#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../raylib.h"
#include "map.h"
#include "textures.h"


// Global variables to store the map, objects, and their dimensions
int **map = NULL;
int **objects = NULL;
int rows = 0;
int cols = 0;
//time_t **lastChangeTimes = NULL;

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
    //lastChangeTimes = (time_t **)malloc(rows * sizeof(time_t *));
    
    for (int i = 0; i < rows; i++) {
        map[i] = (int *)malloc(cols * sizeof(int));
        objects[i] = (int *)malloc(cols * sizeof(int)); // Additional array for objects
        //lastChangeTimes[i] = (time_t *)malloc(cols * sizeof(time_t));
    }

    // Read the tile and object values into the map and objects arrays
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int tile, object;
            fscanf(file, "%d:%d", &tile, &object);  // Read tile:object format
            map[row][col] = tile;
            objects[row][col] = object;  // Store object
            //lastChangeTimes[row][col] = 0;  // Initialize time values if needed
        }
    }

    fclose(file);
}

void drawMap(Camera2D camera){
    BeginMode2D(camera);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Rectangle block = {
                col * 32,  // X position
                row * 32,  // Y position
                32,        // Width
                32         // Height
            };

            // Draw different textures based on the map values
            switch(map[row][col]){
                case 1: 
                    //DrawRectangle(block.x, block.y, 32, 32, GRAY);
                    ;
                    Rectangle tileSource = { 2 * 32, 1 * 32, 32, 32 };
                    DrawTextureRec(grassTileset, tileSource, (Vector2){ block.x, block.y}, WHITE);
                    break;
                case 2:
                    //DrawRectangle(block.x, block.y, 32, 32, LIGHTGRAY);
                    ;
                    Rectangle tileSource1 = { 0 * 32, 3 * 32, 32, 32 };
                    DrawTextureRec(grassTileset, tileSource1, (Vector2){ block.x, block.y}, WHITE);
                    break;
            }

            // Draw different objects based on the objects array values
            switch(objects[row][col]){
                case 1:  // Stone
                    DrawCircle(block.x + 16, block.y + 16, 10, DARKGRAY);  // Example for a stone
                    break;
                case 2:  // Tree
                    DrawCircle(block.x + 16, block.y + 16, 10, GREEN);  // Example for a tree
                    break;
            }
        }
    }
    EndMode2D();
}

void updateMap(Camera2D camera) {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);

    int col = (int)(worldMousePos.x / 32); 
    int row = (int)(worldMousePos.y / 32);

    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        // Left click to toggle between tiles (map)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (map[row][col] == 1) {
                map[row][col] = 2;
            } else if (map[row][col] == 2) {
                map[row][col] = 1;
            }
        }

        // Right click to toggle between objects (objects array)
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            if (objects[row][col] == 0) {
                objects[row][col] = 1;  // Toggle to Stone
            } else if (objects[row][col] == 1) {
                objects[row][col] = 0;  // Toggle to Tree 
            } 
        }
    }
}


void freeMap() {
    // Free the allocated memory for the map, objects, and lastChangeTimes arrays
    for (int i = 0; i < rows; i++) {
        free(map[i]);
        free(objects[i]);
        //free(lastChangeTimes[i]);
    }
    free(map);
    free(objects);
    //free(lastChangeTimes);
}
