//Hubert
#ifndef EDITMAP_H
#define EDITMAP_H

#include "../../../lib/raylib.h"


// Function declaration for the map editing functionality
void editMap(Camera2D cameraE);
extern void setTile(Vector2 mouseTilePos, int selectedTile, int selectedObject);
void setTile2(Vector2 mouseTilePos, int selectedTile, int selectedObject);
extern void createEmptyMap(int width, int height);

#endif
