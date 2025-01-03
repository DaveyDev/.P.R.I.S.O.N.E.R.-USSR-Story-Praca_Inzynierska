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
     Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, cameraE);

    int col = (int)(worldMousePos.x / 32); 
    int row = (int)(worldMousePos.y / 32);

    // Early exit if the mouse is out of bounds
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        return;
    }

    // Right click to toggle between objects (objects array)
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        if (objects[row][col] == 0) {
            objects[row][col] = 1;  // Toggle to tree
        } else if (objects[row][col] == 1) {
            objects[row][col] = 0;   
        } 
    }
}
