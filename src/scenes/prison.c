//Wiktor
#include "../../lib/raylib.h"
#include "scenes.h"
#include "../scripts/map/map.h"
#include "../scripts/global.h"
#include "../scripts/player/player.h"
#include "../scripts/animation.h"
#include "../scripts/items.h"
#include "../scripts/map/objects.h"



bool wasMapLoaded = false;

// Persistent camera state variables
Camera2D camera = { 0 };   // Define the camera outside of the function
bool isCameraInitialized = false;
Player player;


void prisonScene() {
    ClearBackground(RED);


    // Load the map if not already loaded
    if (!wasMapLoaded) {
        loadMap("data/levels/test/test.map");
        loadItems("data/levels/test/items.dat");
        wasMapLoaded = true;
        
        initPlayer(&player, resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height, 200.0f); // Initial position and speed
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
    
    
    
float cameraSpeed = 150.0f * GetFrameTime(); // Adjust speed here

if (player.position.x < GetScreenWidth() / 2 - 64 && player.position.x > 0 && camera.target.x > 16) {
    camera.target.x -= cameraSpeed;
    player.position.x += player.speed * GetFrameTime();
}
if (player.position.x > GetScreenWidth() / 2 && player.position.x < GetScreenWidth()) {
    camera.target.x += cameraSpeed;
    player.position.x -= player.speed * GetFrameTime();
}
if (player.position.y < GetScreenHeight() / 2 - 64 && player.position.y > 0 && camera.target.y > 16) {
    camera.target.y -= cameraSpeed;
    player.position.y += player.speed * GetFrameTime();
}
if (player.position.y > GetScreenHeight() / 2 && player.position.y < GetScreenHeight() && camera.target.y < cols * 32) {
    camera.target.y += cameraSpeed;
    player.position.y -= player.speed * GetFrameTime();
}


    
    drawMap(camera);
    updateMap(camera);

    drawItems(camera);
    updateItems(camera);

    drawPlayer(&player);
    updatePlayer(&player, GetFrameTime(), objects, details, rows, cols, camera);



    // Draw player collider in green
    DrawRectangleRec(player.collider, GREEN);

    // Draw all solid objects
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int objectID = objects[row][col];

            if (isWallLike(objectID)) {
                Rectangle objectCollider = { col * 32, row * 32, 32, 32 };
                //DrawRectangleRec(objectCollider, RED);
            } else if (objectID >= 1000 && objectID <= 1999) { // Trees (circular)
                Vector2 circleCenter = { col * 32 + 16, row * 32 + 16 };
                float circleRadius = 8.0f;
                //DrawCircleV(circleCenter, circleRadius, BLUE);
            } else if (objectID >= 2000 && objectID <= 2999) { // Trees (circular)
                Vector2 circleCenter = { col * 32 + 16, row * 32 + 16 };
                float circleRadius = 12.0f;
                //DrawCircleV(circleCenter, circleRadius, BLUE);
            }
        }
    }
    

    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);

    if(IsKeyPressed(KEY_ESCAPE)) {
        *currentScene = PAUSE;
        //isCameraInitialized = false;  // Mark camera as initialized
        //unloadPlayer(&player);
       
    } 
    if(IsKeyPressed(KEY_E)) {
        *currentScene = INVENTORY;
        //isCameraInitialized = false;  // Mark camera as initialized
        //unloadPlayer(&player);
       
    } 
}
