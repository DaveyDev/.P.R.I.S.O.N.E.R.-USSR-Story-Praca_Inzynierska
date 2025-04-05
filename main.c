//Wiktor
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION

#include "lib/raygui.h"
#include "lib/raylib.h"
#include "src/scripts/player/player.h"
#include "src/scenes/scenes.h"
#include "src/scripts/global.h"
#include "src/scripts/styles.h"
#include "src/scripts/textures.h"
#include "src/scripts/translation.h"
#include "src/scripts/cheats/debugCMD.h"

bool exitMenu = false;

int main(){



    // Raylib Initialization
    loadProperties("game.properties");

    InitWindow(resolutions[currentResolutionIndex].width, resolutions[currentResolutionIndex].height, ".P.R.I.S.O.N.E.R.");
    SetTargetFPS(600);

    
    initGlobals();
    initGuiStyle();
    initDebugCMD();
    loadTextures();
    loadLanguage();
    SetExitKey(0);


    while (!WindowShouldClose() && !windowShouldClose) {

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
            case PAUSE:
                pauseScene();
                break;
            case STARTGAME:
                startGameScene();
                break;
            case END_GAME:
                endGameScene();
                break;
            default:
                WindowShouldClose();
                break;
        }   


        updateDebugCMD();
        drawDebugCMD(); // Draws the debug command input field


        EndDrawing();

    }

    freeTranslations();
    unloadTextures();
    CloseWindow(); // Close window and OpenGL context

    return 0;
}