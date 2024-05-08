#include <stdio.h>
#include <stdlib.h>
#include "src/raylib.h"
#include "src/scripts/player.c"
#include "src/scripts/checkCollision.c"
#include "src/scripts/menu.c"
#include "src/scripts/textures.c"
#include "src/scripts/map.c"





int main() {
    // Load the map from the text file
    loadMap("src/map.map");
    //int scene = 0;
    //int numSeeds = 0;
    
    //int coins = 0;
    
    // Raylib Initialization
    //const int screenWidth = 832;
    const int screenWidth = 1088;
    const int screenHeight = 640;
    InitWindow(screenWidth, screenHeight, "Map Editor");
    
    loadTextures();
    
    Camera2D camera = { 0 };
    camera.target = (Vector2){0,0};
    //camera.target.y = player.position.y - 100;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;



    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
         if (IsKeyDown(KEY_A)) {
            camera.target.x -= 10;
        }
        if (IsKeyDown(KEY_D)) {
            camera.target.x += 10;
        }
        
        if (IsKeyDown(KEY_W)) {
            camera.target.y -= 10;
        }
        
        if (IsKeyDown(KEY_S)) {
            camera.target.y += 10;
        }
        
        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove()*0.05f);

        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            //camera.rotation = 0.0f;
        }
            

        //if(scene == 0){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawMap(camera);
        //scene = menuScene();

        
        //}

        EndDrawing();
        
    }
    
 
    unloadTextures();
    CloseWindow();

    return 0;
}