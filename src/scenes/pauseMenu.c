//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/map/map.h"
#include "../scripts/translation.h"




const char *pause = "PAUSE";
const int pauseFontSize = 48;
//const char *subtitle = "USSR Story";
//const int subtitleFontSize = 40;


 


void pauseScene(){
    
    
    mousePoint = GetMousePosition();
    
     // Dynamically adjust font size based on resolution
    int dynamicTitleFontSize = pauseFontSize * (GetScreenWidth() / 800);  // Scale font size relative to base resolution
    //int dynamicSubitleFontSize = subtitleFontSize * (GetScreenWidth() / 800);  // Scale font size relative to base resolution

    ClearBackground(bgColor);

    //drawMap(camera);

    Rectangle playBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2, 300, 50};
    Rectangle optionsBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2 + 80, 300, 50};
    Rectangle exitBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2 + 160, 300, 50};

    DrawText(pause, GetScreenWidth() / 2 - MeasureText(pause, dynamicTitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4, dynamicTitleFontSize, WHITE);
   // DrawText(subtitle, GetScreenWidth() / 2 - MeasureText(subtitle, dynamicSubitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4 + dynamicTitleFontSize, dynamicSubitleFontSize, WHITE);

    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);




    
    if (GuiButton(optionsBtn, getTranslation("pause_resume"))) {
     *currentScene = GAME;
    }
    if (GuiButton(exitBtn, getTranslation("pause_exit"))) {
     *currentScene = MENU;
    }


    if(IsKeyPressed(KEY_ESCAPE)) {
        *currentScene = GAME;
        //isCameraInitialized = false;  // Mark camera as initialized
        //unloadPlayer(&player);
       
    } 
    
    
    
}