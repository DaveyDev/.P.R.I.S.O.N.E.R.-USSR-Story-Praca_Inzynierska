//Wiktor
#include "textures.h"

// Define the tileset texture
Texture2D grassTileset;
Texture2D treesTileset;
Texture2D stonesTileset;
Texture2D wallSet;



void loadTextures() {
    // Load the tileset texture
    grassTileset = LoadTexture("data/textures/grassTileset.png");
    treesTileset = LoadTexture("data/textures/treeSet.png");
    wallSet = LoadTexture("data/textures/wallTileset.png");
    stonesTileset = LoadTexture("data/textures/stonesTileset.png");

}

void unloadTextures() {
    // Unload the tileset texture
    UnloadTexture(grassTileset);
    UnloadTexture(treesTileset);
    UnloadTexture(wallSet);
    UnloadTexture(stonesTileset);


}
