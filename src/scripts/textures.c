//Wiktor
#include "textures.h"

// Define the tileset texture
Texture2D grassTileset;
Texture2D treesTileset;
Texture2D placeableTileset;
Texture2D wallSet;
Texture2D playerSet;
Texture2D itemsSet;
Texture2D mainMenuBackground;
Texture2D pauseMenuBackground;
Image mainIco;
Texture2D inmateTexture;
Texture2D guardTexture;

void loadTextures() {
    // Load the tileset texture
    grassTileset = LoadTexture("data/textures/grassTileset.png");
    treesTileset = LoadTexture("data/textures/treeSet.png");
    wallSet = LoadTexture("data/textures/wallTileset.png");
    placeableTileset = LoadTexture("data/textures/placeableTileset.png");
    playerSet = LoadTexture("data/textures/playerSet.png");
    itemsSet = LoadTexture("data/textures/itemsSet.png");
    mainMenuBackground = LoadTexture("data/textures/mainMenuBackground.png");
    pauseMenuBackground = LoadTexture("data/textures/pauseMenuBackground.png");
    mainIco = LoadImage("data/textures/ico.png");
    inmateTexture = LoadTexture("data/textures/inmateSet.png");
    guardTexture = LoadTexture("data/textures/guardSet.png");
}

void unloadTextures() {
    // Unload the tileset texture
    UnloadTexture(grassTileset);
    UnloadTexture(treesTileset);
    UnloadTexture(wallSet);
    UnloadTexture(placeableTileset);
    UnloadTexture(playerSet);
    UnloadTexture(itemsSet);
    UnloadTexture(mainMenuBackground);
    UnloadTexture(pauseMenuBackground);

}
