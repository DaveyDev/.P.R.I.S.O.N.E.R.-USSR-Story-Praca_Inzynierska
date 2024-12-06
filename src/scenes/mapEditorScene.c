//Hubert
#include "../raylib.h"
#include "scenes.h"
#include "../scripts/map.h"
#include "../scripts/global.h"
#include "../scripts/editMap.h"

bool wasMapLoadedE = false;

// Persistent camera state variables
Camera2D cameraE = { 0 };   // Define the camera outside of the function
bool isCameraInitializedE = false;

void mapsEditorScene() {
    ClearBackground(GRAY);

    // Load the map if not already loaded
    if (!wasMapLoadedE) {
        loadMap("data/levels/test1.map");
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
    editMap(cameraE);

    if(IsKeyDown(KEY_A)) {
        *currentScene = MENU;
        isCameraInitializedE = false;  // Mark camera as initialized
    }
}
