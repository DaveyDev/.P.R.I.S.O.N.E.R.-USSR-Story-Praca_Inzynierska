//Hubert
#include <stdio.h>
#include <stdlib.h>

#include "../../../lib/raylib.h"
#include "map.h"
#include "../global.h"
#include "objects.h"
#include "editMap.h"


void editMap(Camera2D cameraE) 
{
    
    
}


void setTile(Vector2 mouseTilePos, int selectedTile, int selectedObject){

    printf("set tile function was called\n");

    int row = mouseTilePos.y;
    int col = mouseTilePos.x;
    objects[row][col] = selectedTile;  // Toggle to tree
        
}


