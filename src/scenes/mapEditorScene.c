//Hubert
#include "../../lib/raylib.h"
#include "scenes.h"
#include "../scripts/map/map.h"
#include "../scripts/global.h"
#include "../scripts/map/editMap.h"

bool wasMapLoadedE = false;

// Persistent camera state variables
Camera2D cameraE = { 0 };   // Define the camera outside of the function
bool isCameraInitializedE = false;

// Editor variables
int selectedTile = 1;   // Default selected tile
int selectedObject = 0; // Default selected object
Vector2 mouseTilePos;

void mapsEditorScene() {
    ClearBackground(GRAY);

    // Load the map if not already loaded
   // Assuming mapName is a global variable accessible in this file
    if (!wasMapLoadedE) {
        char mapPath[256]; // Buffer to store the complete path
        snprintf(mapPath, sizeof(mapPath), "data/levels/%s.map", mapName);
        loadMap(mapPath); // Use the constructed path
        wasMapLoadedE = true;
}

    // Initialize the camera only once
    if (!isCameraInitializedE) {
        cameraE.target = (Vector2){0, 0};
        cameraE.offset = (Vector2){0, 0};
        cameraE.rotation = 0.0f;
        //camera.zoom = 1.0f;
        cameraE.zoom = (resolutions[currentResolutionIndex].height / 600.0f) * 2.0f;
        isCameraInitializedE = true;  // Mark camera as initialized
    }

    // Set camera movement speed
    float cameraSpeed = 200.0f * GetFrameTime();  // Adjust speed here

    // Move the camera based on arrow keys
    if (IsKeyDown(KEY_RIGHT)) cameraE.target.x += cameraSpeed;
    if (IsKeyDown(KEY_LEFT)) cameraE.target.x -= cameraSpeed;
    if (IsKeyDown(KEY_DOWN)) cameraE.target.y += cameraSpeed;
    if (IsKeyDown(KEY_UP)) cameraE.target.y -= cameraSpeed;

    
    drawMap(cameraE);
    updateMap(cameraE);
    //editMap(cameraE);

    // Mouse position in world coordinates
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cameraE);
    mouseTilePos = (Vector2){(int)(mouseWorldPos.x / TILE_SIZE), (int)(mouseWorldPos.y / TILE_SIZE)};

    // Highlight the tile under the mouse
    DrawRectangleLines(mouseTilePos.x * TILE_SIZE, mouseTilePos.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);

    // Place tile/object on left mouse click
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        setTile(mouseTilePos.x, mouseTilePos.y, selectedTile, selectedObject);
    }

    // Tile/Object selection (Basic example)
    DrawText("Tile/Object Selection:", 10, 10, 20, WHITE);
    DrawText(TextFormat("Tile: %d", selectedTile), 10, 40, 20, WHITE);
    DrawText(TextFormat("Object: %d", selectedObject), 10, 70, 20, WHITE);

    if (IsKeyPressed(KEY_ONE)) selectedTile = 1; // Example: Change tile to 1
    if (IsKeyPressed(KEY_TWO)) selectedTile = 2; // Example: Change tile to 2
    if (IsKeyPressed(KEY_THREE)) selectedObject = 1; // Example: Change object to 1
    if (IsKeyPressed(KEY_FOUR)) selectedObject = 2; // Example: Change object to 2

    // Save/Load functionality
    if (IsKeyPressed(KEY_S)) saveMap("data/levels/test1.map");
    //if (IsKeyPressed(KEY_L)) loadMap("data/levels/test1.map");

    if(IsKeyDown(KEY_A)) {
        *currentScene = MENU;
        isCameraInitializedE = false;  // Mark camera as initialized
    }
}
