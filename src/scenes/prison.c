//Wiktor
#include "../../lib/raylib.h"
#include "scenes.h"
#include "../scripts/map/map.h"
#include "../scripts/global.h"
#include "../scripts/player/player.h"

bool wasMapLoaded = false;

// Persistent camera state variables
Camera2D camera = { 0 };   // Define the camera outside of the function
bool isCameraInitialized = false;

void prisonScene() {
    ClearBackground(RED);

    // Load the map if not already loaded
    if (!wasMapLoaded) {
        loadMap("data/levels/test1.map");
        wasMapLoaded = true;
    }

    // Initialize the camera only once
    if (!isCameraInitialized) {
        camera.target = (Vector2){0, 0};
        camera.offset = (Vector2){0, 0};
        camera.rotation = 0.0f;
        //camera.zoom = 1.0f;
        camera.zoom = (resolutions[currentResolutionIndex].height / 600.0f) * 2.0f;
        isCameraInitialized = true;  // Mark camera as initialized
    }

    // Initialize the player
    Player player;
    initPlayer(&player, GetScreenWidth(), GetScreenHeight(), 200.0f); // Call the function


    // Set camera movement speed
    float cameraSpeed = 200.0f * GetFrameTime();  // Adjust speed here

    // Move the camera based on arrow keys
    if (IsKeyDown(KEY_RIGHT)) camera.target.x += cameraSpeed;
    if (IsKeyDown(KEY_LEFT)) camera.target.x -= cameraSpeed;
    if (IsKeyDown(KEY_DOWN)) camera.target.y += cameraSpeed;
    if (IsKeyDown(KEY_UP)) camera.target.y -= cameraSpeed;

    
    drawMap(camera);
    updateMap(camera);
    //editMap(camera);
    drawPlayer(&player);

    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);

    if(IsKeyDown(KEY_Q)) {
        *currentScene = MENU;
        isCameraInitialized = false;  // Mark camera as initialized
        //unloadPlayer(&player);
    }
}
