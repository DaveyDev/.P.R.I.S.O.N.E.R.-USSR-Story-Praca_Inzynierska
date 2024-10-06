#include "textures.h"

// Define the tileset texture
Texture2D grassTileset;

void loadTextures() {
    // Load the tileset texture
    grassTileset = LoadTexture("data/textures/grass1.png");
}

void unloadTextures() {
    // Unload the tileset texture
    UnloadTexture(grassTileset);
}
