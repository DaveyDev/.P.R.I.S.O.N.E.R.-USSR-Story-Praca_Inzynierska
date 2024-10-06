#ifndef TEXTURES_H
#define TEXTURES_H

#include "../raylib.h"

// Declare the textures (extern means they are defined elsewhere)
extern Texture2D grassTileset;

// Function declarations
void loadTextures();
void unloadTextures();

#endif // TEXTURES_H
