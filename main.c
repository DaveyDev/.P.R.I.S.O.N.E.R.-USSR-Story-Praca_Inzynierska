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
    int scene = 0;
    
    int coins = 0;
    
    // Raylib Initialization
    const int screenWidth = 1024;
    const int screenHeight = 768;
    
    InitWindow(screenWidth, screenHeight, ".P.R.I.S.O.N.E.R.");
    //ToggleFullscreen();

    
    loadTextures();
    
    
    Player player;
    initPlayer(&player, screenWidth, screenHeight, 213.7f); // Initial position and speed
    
   
    Camera2D camera = { 0 };
    camera.target = (Vector2){0,0};
    //camera.target.y = player.position.y - 100;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    



    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        

    switch(scene){
        case 0:
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawMap(camera);
        scene = menuScene();
        EndDrawing();

        
        break;
        
        case 1:
        
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          
        clickOnTile(&player);
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the map
        //updateMap();
        drawMap(camera);
       
   
        float deltaTime = GetFrameTime();
        updatePlayer(&player, deltaTime);
        drawPlayer(&player);
        
        int countFPS = GetFPS();
        DrawText(TextFormat("FPS: %i", countFPS), screenWidth - 100, 15, 20, BLUE);
        DrawText(TextFormat("ver. 240126"), screenWidth - 150, 600, 20, BLUE);
        DrawText(TextFormat("coins: %i", coins), 80, 15, 20, BLUE);
  
        
     
        EndDrawing();
        break;
        
    }
}
    unloadPlayer(&player);
    unloadTextures();
    CloseWindow();

    return 0;
}
