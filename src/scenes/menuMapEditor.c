//Hubert
#include "../raylib.h"
#include "../raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"




const char *titleMapEditor = ".P.R.I.S.O.N.E.R.";
const int titleFontSizeMapEditor = 48;
const char *subtitleMapEditor = "Map Editor";
const int subtitleFontSizeMapEditor = 40;


 


void menuMapEditorScene(){
    
    mousePoint = GetMousePosition();
    
     // Dynamically adjust font size based on resolution
    int dynamicTitleFontSize = titleFontSizeMapEditor * (GetScreenWidth() / 800);  // Scale font size relative to base resolution
    int dynamicSubitleFontSize = subtitleFontSizeMapEditor * (GetScreenWidth() / 800);  // Scale font size relative to base resolution

    ClearBackground(bgColor);

    Rectangle playBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2, 200, 50};
    Rectangle optionsBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 80, 200, 50};
    Rectangle exitBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 160, 200, 50};

    DrawText(titleMapEditor, GetScreenWidth() / 2 - MeasureText(titleMapEditor, dynamicTitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4, dynamicTitleFontSize, WHITE);
    DrawText(subtitleMapEditor, GetScreenWidth() / 2 - MeasureText(subtitleMapEditor, dynamicSubitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4 + dynamicTitleFontSize, dynamicSubitleFontSize, WHITE);




    if (GuiButton(playBtn, "EDIT")) {
     *currentScene = EDITOR;
    }
    if (GuiButton(optionsBtn, "OPTIONS")) {
     *currentScene = OPTIONS;
    }
    if (GuiButton(exitBtn, "EXIT")) {
     CloseWindow();
    }

    
    
}