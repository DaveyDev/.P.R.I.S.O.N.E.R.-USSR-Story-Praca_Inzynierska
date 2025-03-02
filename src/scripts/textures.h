//Wiktor
#ifndef TEXTURES_H
#define TEXTURES_H

#include "../../lib/raylib.h"

// Declare the textures (extern means they are defined elsewhere)
extern Texture2D grassTileset;
extern Texture2D treesTileset;
extern Texture2D placeableTileset;
extern Texture2D wallSet;
extern Texture2D playerSet;
extern Texture2D itemsSet;



// Function declarations
void loadTextures();
void unloadTextures();

#endif // TEXTURES_H
