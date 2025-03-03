//Wiktor
#include "../../lib/raylib.h"
#include "scenes.h"
#include "../scripts/map/map.h"
#include "../scripts/global.h"
#include "../scripts/player/player.h"
#include "../scripts/animation.h"
#include "../scripts/items.h"



bool wasMapLoaded = false;

// Persistent camera state variables
Camera2D camera = { 0 };   // Define the camera outside of the function
bool isCameraInitialized = false;
Player player;


void prisonScene() {
    ClearBackground(RED);


    // Load the map if not already loaded
    if (!wasMapLoaded) {
        loadMap("data/levels/test1.map");
        loadItems();
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
    
    
    
    // Set camera movement speed
    float cameraSpeed = 200.0f * GetFrameTime();  // Adjust speed here

    //camera follows player magic
    if(player.position.x < GetScreenWidth()/2 - 64  && player.position.x > 0 && camera.target.x > 16 ){
        camera.target.x = camera.target.x - player.speed * GetFrameTime();
        player.position.x = player.position.x + player.speed * GetFrameTime();
    }
    if(player.position.x >  GetScreenWidth()/2 && player.position.x < GetScreenHeight()){
        camera.target.x = camera.target.x + player.speed * GetFrameTime();
        player.position.x = player.position.x - player.speed * GetFrameTime();
    }
    if(player.position.y < GetScreenHeight()/2 - 64  && player.position.y > 0 && camera.target.y > 16){
        camera.target.y = camera.target.y - player.speed * GetFrameTime();
        player.position.y = player.position.y + player.speed * GetFrameTime();
    }
    if(player.position.y >  GetScreenHeight()/2 && player.position.y < GetScreenHeight() && camera.target.y < cols * 32){
        camera.target.y = camera.target.y + player.speed * GetFrameTime();
        player.position.y = player.position.y - player.speed * GetFrameTime();
    }
    //camera.target.y = player.position.y;

    
    drawMap(camera);
    updateMap(camera);

    drawItems(camera);
    updateItems(camera);

    drawPlayer(&player);
    updatePlayer(&player, GetFrameTime());
    

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
