//Hubert
#include <stdio.h>
#include <stdlib.h>

#include "../../../lib/raylib.h"
#include "map.h"
#include "../global.h"
#include "objects.h"
#include "editMap.h"


#include <sys/types.h> // Required for mkdir()
#include <direct.h>    // _mkdir() for Windows


void editMap(Camera2D cameraE) 
{
    
    
}


void setTile(Vector2 mouseTilePos, int selectedTile, int selectedObject){

    printf("set tile function was called\n");

    int row = mouseTilePos.y;
    int col = mouseTilePos.x;
    objects[row][col] = selectedTile;  // Toggle to tree
        
}


void createEmptyMap(int width, int height) {
    // Create directory path
    char dirPath[256];
    snprintf(dirPath, sizeof(dirPath), "data/levels/%s", mapName);

    // Create directory if it doesn't exist
    _mkdir(dirPath);  // Windows, use mkdir(dirPath, 0777) on Linux

    // Create file path
    char mapPath[256];
    snprintf(mapPath, sizeof(mapPath), "%s/%s.map", dirPath, mapName);

    // Open file for writing
    FILE *file = fopen(mapPath, "w");
    if (!file) {
        printf("Error: Could not create file %s\n", mapPath);
        return;
    }

    // Write width and height
    fprintf(file, "%d %d\n", width, height);

    // Write map data with three layers (tile:object:detail)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fprintf(file, "1:0:0");  // Default ground, empty object, empty detail
            if (x < width - 1) fprintf(file, " "); // Space between tiles
        }
        fprintf(file, "\n"); // New line after each row
    }

    fclose(file);
    printf("Map created successfully: %s\n", mapPath);
}




