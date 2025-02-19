//Wiktor
#include "textures.h"

// Define the tileset texture
Texture2D grassTileset;
Texture2D treesTileset;
Texture2D placeableTileset;
Texture2D wallSet;
Texture2D playerSet;



void loadTextures() {
    // Load the tileset texture
    grassTileset = LoadTexture("data/textures/grassTileset.png");
    treesTileset = LoadTexture("data/textures/treeSet.png");
    wallSet = LoadTexture("data/textures/wallTileset.png");
    placeableTileset = LoadTexture("data/textures/placeableTileset.png");
    playerSet = LoadTexture("data/textures/playerSet.png");

}

void unloadTextures() {
    // Unload the tileset texture
    UnloadTexture(grassTileset);
    UnloadTexture(treesTileset);
    UnloadTexture(wallSet);
    UnloadTexture(placeableTileset);
    UnloadTexture(playerSet);


}
