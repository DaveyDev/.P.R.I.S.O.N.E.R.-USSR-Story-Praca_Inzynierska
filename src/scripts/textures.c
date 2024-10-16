#include "textures.h"

// Define the tileset texture
Texture2D grassTileset;
Texture2D treesTileset;


void loadTextures() {
    // Load the tileset texture
    grassTileset = LoadTexture("data/textures/grass1.png");
    treesTileset = LoadTexture("data/textures/tree.png");
}

void unloadTextures() {
    // Unload the tileset texture
    UnloadTexture(grassTileset);
    UnloadTexture(treesTileset);

}
