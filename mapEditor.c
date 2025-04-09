//Hubert
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION

#include "lib/raygui.h"
#include "lib/raylib.h"
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

    
    InitWindow(resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height, ".P.R.I.S.O.N.E.R. - Map Editor");
    SetTargetFPS(60);
    initGuiStyle();
    loadTextures();
    initGlobals();

    SetExitKey(0);
    isEditor = true;
    

    while (!WindowShouldClose()) {


        BeginDrawing();

        
        switch (*currentScene){
            case MENU:
                menuMapEditorScene();
                break;
            case OPTIONS:
                optionsScene();
                break;
            case EDITOR:
                mapsEditorScene();
                break;
            case CREATE_MAP:
                createMapScene();
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