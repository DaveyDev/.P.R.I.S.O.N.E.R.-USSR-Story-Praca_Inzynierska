//Hubert
#include "../../lib/raylib.h"
#include "scenes.h"
#include "../scripts/map/map.h"
#include "../scripts/global.h"
#include "../scripts/map/editMap.h"
#include "../scripts/items/items.h"
#include "../scripts/player/inventory.h"
#include "../scripts/map/objects.h"

#include "../scripts/player/player.h"
#include "../scripts/animation.h"
#include "../scripts/textures.h"
#include "../scripts/cheats/debugCMD.h"



bool wasMapLoadedE = false;

// Persistent camera state variables
Camera2D cameraE = { 0 };   // Define the camera outside of the function
bool isCameraInitializedE = false;

// Editor variables
int selectedTile = 11;   // Default selected tile
int selectedObject = 0; // Default selected object
int mode = 0;

// Define mode names
const char* modeNames[] = {
    "Object Placement",
    "Object Removal",
    "Item Placement",
    "Detail Placement",
    "Detail Removal"
};

void mapsEditorScene() {
    ClearBackground(GRAY);

    if (!wasMapLoadedE) {
        char mapPath[256];
        char itemPath[256];
        snprintf(mapPath, sizeof(mapPath), "data/levels/%s/%s.map", mapName, mapName);
        snprintf(itemPath, sizeof(itemPath), "data/levels/%s/items.dat", mapName);
        loadMap(mapPath);
        
        initPlayer(&player, resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height, 200.0f); // Initial position and speed
        player.position.x = -100;
        player.position.y = -100;

        loadItems(itemPath);
        wasMapLoadedE = true;

        //initInventory();
        //loadInventory();

       
        
    }

    if (!isCameraInitializedE) {
        cameraE.target = (Vector2){0, 0};
        cameraE.offset = (Vector2){0, 0};
        cameraE.rotation = 0.0f;
        cameraE.zoom = (resolutions[currentResolutionIndex].height / 600.0f) * 2.0f;
        isCameraInitializedE = true;
    }

    float cameraSpeed;
    if(IsKeyDown(KEY_LEFT_SHIFT)) cameraSpeed = 400.0f * GetFrameTime();
    else cameraSpeed = 200.0f * GetFrameTime();

    if (IsKeyDown(KEY_D)) cameraE.target.x += cameraSpeed;
    if (IsKeyDown(KEY_A)) cameraE.target.x -= cameraSpeed;
    if (IsKeyDown(KEY_S)) cameraE.target.y += cameraSpeed;
    if (IsKeyDown(KEY_W)) cameraE.target.y -= cameraSpeed;

    if (IsKeyDown(KEY_Z)) cameraE.zoom -= 0.02f;
    if (IsKeyDown(KEY_X)) cameraE.zoom += 0.02f;

    Player player;

    drawMap(cameraE);
    updateMap(cameraE);
    editMap(cameraE);
    updateItems(cameraE);
    drawItems(cameraE);


    

    

    

    


    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cameraE);
    Vector2 mouseTilePos = (Vector2){(int)(mouseWorldPos.x / TILE_SIZE), (int)(mouseWorldPos.y / TILE_SIZE)};

    if(IsKeyPressed(KEY_Q)) {
        selectedTile -= 1000;
        if (selectedTile < 0) selectedTile = 3000;
    }

    if (IsKeyPressed(KEY_E)) {
        selectedTile += 1000;
        if (selectedTile > 3999) selectedTile = 0;
    }



    if (IsKeyPressed(KEY_SPACE)) {
        mode = (mode + 1) % 5;
    }

    selectedTile -= GetMouseWheelMove();

    // Draw UI Panel
    int panelHeight = 100;
    //DrawRectangle(0, GetScreenHeight() - panelHeight, GetScreenWidth(), panelHeight, DARKGRAY);

    // Draw Tile Selection
    int tileSize = 50;
    int tileX = 10;
    int tileY = GetScreenHeight() - panelHeight + 10;
    for (int i = 0; i < 10; i++) { // Example: 10 tiles
        //DrawRectangle(tileX + i * (tileSize + 5), tileY, tileSize, tileSize, (selectedTile == i) ? RED : LIGHTGRAY);
        //DrawText(TextFormat("%d", i), tileX + i * (tileSize + 5) + 15, tileY + 15, 20, BLACK);

        // Handle Tile Selection
        //Rectangle tileRect = {tileX + i * (tileSize + 5), tileY, tileSize, tileSize};
        //if (CheckCollisionPointRec(GetMousePosition(), tileRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //selectedTile = i;
        //}
    }

    // Draw Top Panel
int topPanelHeight = 50;
DrawRectangle(0, 0, GetScreenWidth(), topPanelHeight, DARKGRAY);


    // Save Button
    Rectangle saveButton = {GetScreenWidth() - 150, 10, 60, 30};
    DrawRectangleRec(saveButton, BLUE);
    DrawText("Save", saveButton.x + 10, saveButton.y + 5, 20, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), saveButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        char mapPath[256];
        snprintf(mapPath, sizeof(mapPath), "data/levels/%s/%s.map", mapName, mapName);
        char itemPath[256];
        snprintf(itemPath, sizeof(itemPath), "data/levels/%s/items.dat", mapName);
        saveMap(mapPath);
        printf("saved map");
        saveItemsE(itemPath);
        printf("saved items");
    }

    // Exit Button
    Rectangle exitButton = {GetScreenWidth() - 80, 10, 60, 30};
    DrawRectangleRec(exitButton, RED);
    DrawText("Exit", exitButton.x + 10, exitButton.y + 5, 20, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScene = MENU;
        isCameraInitializedE = false;
        wasMapLoadedE = false;
    }

    DrawText(TextFormat("Tile: %d", selectedTile), 10, 15, 20, WHITE);
    DrawText(TextFormat("Mode: %d", mode), 130, 15, 20, WHITE);
    // Draw mode name
    DrawText(TextFormat("Mode: %s", modeNames[mode]), 260, 15, 20, WHITE);
    
    Rectangle block = { 150, 50, TILE_SIZE, TILE_SIZE }; // Position next to text
    
    // Determine the correct tile to draw
    int displayTile = selectedTile;
    if (selectedTile >= 2000 && selectedTile <= 2999) {
        displayTile -= 2000; // Adjust for placeables
        drawPlaceable(displayTile, block);
    } else if (selectedTile >= 1000 && selectedTile <= MAX_TREE_ID) {
        block = (Rectangle){ 150, 150, TILE_SIZE, TILE_SIZE };
        displayTile -= 1000; // Adjust for trees
        drawTree(displayTile, block);
        block = (Rectangle){ 150, 30, TILE_SIZE, TILE_SIZE };
    }
    else if (selectedTile >= 3000 && selectedTile <= 3999) {
        displayTile -= 3000; // Adjust for trees
        drawItemEditor(displayTile, block);
    }

// Draw the selected tile next to the text
    
    


    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMousePosition().y > topPanelHeight) {
        Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), cameraE);

        if (mode == 0) setTile(mouseTilePos, selectedTile, selectedObject);
        if (mode == 1) setTile(mouseTilePos, 0, selectedObject);
        if (mode == 2) addItem(worldMousePos, selectedTile, 1, "Custom Item"); // Adjust name as needed
        if (mode == 3) setTile2(mouseTilePos, selectedTile, selectedObject);
        if (mode == 4) setTile2(mouseTilePos, 0, selectedObject);
    }


}

