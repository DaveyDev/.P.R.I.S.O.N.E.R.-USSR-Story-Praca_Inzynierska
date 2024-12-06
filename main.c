//Wiktor
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION

#include "src/raygui.h"
#include "src/raylib.h"
#include "src/scripts/player/player.h"
#include "src/scenes/scenes.h"
#include "src/scripts/global.h"
#include "src/scripts/styles.h"
#include "src/scripts/textures.h"

bool exitMenu = false;

int main(){



    // Raylib Initialization
    //const int screenWidth = 1024;
    //const int screenHeight = 768;
    loadProperties("game.properties");

    
    InitWindow(resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height, ".P.R.I.S.O.N.E.R.");
    SetTargetFPS(60);
    initGuiStyle();
    
    loadTextures();

    Player player;
    initPlayer(&player, resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height, 200.0f); // Initial position and speed
    initGlobals();
    

    while (!WindowShouldClose()) {

        //BeginDrawing();
        //ClearBackground(RAYWHITE);


        //float deltaTime = GetFrameTime();
        //updatePlayer(&player, deltaTime);
        //drawPlayer(&player);

        //int countFPS = GetFPS();
        //DrawText(TextFormat("FPS: %i", countFPS), screenWidth - 100, 15, 20, BLUE);
        //DrawText(TextFormat("ver. 021024"), screenWidth - 150, screenHeight - 100, 20, BLUE);

        //EndDrawing();


        BeginDrawing();

        
        switch (*currentScene){
            case MENU:
                menuScene();
                break;
            case OPTIONS:
                optionsScene();
                break;
            case GAME:
                prisonScene();
                break;
            default:
                WindowShouldClose();
                break;
        }   
            

        EndDrawing();

    
    }

    unloadTextures();
    CloseWindow(); // Close window and OpenGL context

    return 0;
}