//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/translation.h"




const char *title = ".P.R.I.S.O.N.E.R.";
const int titleFontSize = 48;
const char *subtitle = "USSR Story";
const int subtitleFontSize = 40;


 


void menuScene(){
    
    mousePoint = GetMousePosition();
    
     // Dynamically adjust font size based on resolution
    int dynamicTitleFontSize = titleFontSize * (GetScreenWidth() / 800);  // Scale font size relative to base resolution
    int dynamicSubitleFontSize = subtitleFontSize * (GetScreenWidth() / 800);  // Scale font size relative to base resolution

    ClearBackground(bgColor);

    Rectangle playBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2, 200, 50};
    Rectangle optionsBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 80, 200, 50};
    Rectangle exitBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 160, 200, 50};

    DrawText(title, GetScreenWidth() / 2 - MeasureText(title, dynamicTitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4, dynamicTitleFontSize, WHITE);
    DrawText(subtitle, GetScreenWidth() / 2 - MeasureText(subtitle, dynamicSubitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4 + dynamicTitleFontSize, dynamicSubitleFontSize, WHITE);

    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);




    if (GuiButton(playBtn, getTranslation("menu_start"))) {
     *currentScene = GAME;
    }
    if (GuiButton(optionsBtn, getTranslation("menu_options"))) {
     *currentScene = OPTIONS;
    }
    if (GuiButton(exitBtn, getTranslation("menu_exit"))) {
     CloseWindow();
    }

    
    
}