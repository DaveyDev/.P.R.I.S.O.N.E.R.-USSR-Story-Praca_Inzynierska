//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/translation.h"




const char *subtitle1 = "SAVES";
const int subtitleFontSize1 = 40;


 


void startGameScene(){
    
    mousePoint = GetMousePosition();
    
     // Dynamically adjust font size based on resolution
    int dynamicSubitleFontSize = subtitleFontSize1 * (GetScreenWidth() / 800);  // Scale font size relative to base resolution

    ClearBackground(bgColor);

    Rectangle playBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2, 200, 50};
    Rectangle optionsBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 80, 200, 50};
    Rectangle exitBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 160, 200, 50};


    
    Vector2 subtitle1Pos = { 
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, getTranslation("general_saves"), dynamicSubitleFontSize, 2).x / 2, 
        GetScreenHeight() / 2 - GetScreenHeight() / 4 
    };

    DrawTextEx(baseFont, getTranslation("general_saves"), subtitle1Pos, dynamicSubitleFontSize, 2, WHITE);


    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);




    if (GuiButton(playBtn, getTranslation("startgame_newgame"))) {
     *currentScene = GAME;
    }
    if (GuiButton(optionsBtn, getTranslation("startgame_loadsave"))) {
     *currentScene = MENU;
    }
    if (GuiButton(exitBtn, getTranslation("general_back"))) {
     *currentScene = MENU;
    }

    
    
}