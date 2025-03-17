//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/translation.h"
#include "../scripts/textures.h"




const int titleFontSize = 48;
const int subtitleFontSize = 40;


 


void menuScene(){
    
    mousePoint = GetMousePosition();
    
     // Dynamically adjust font size based on resolution
    int dynamicTitleFontSize = titleFontSize * (GetScreenWidth() / 800);  // Scale font size relative to base resolution
    int dynamicSubitleFontSize = subtitleFontSize * (GetScreenWidth() / 800);  // Scale font size relative to base resolution

    ClearBackground(bgColor);
    DrawTexture(mainMenuBackground, 0, 0, WHITE);
    // Draw the texture with scaling
    Vector2 position = { 0, 0 };  // Position where the texture will be drawn
    float scaleMenu = (float)GetScreenHeight() / 800.0f;
    DrawTextureEx(mainMenuBackground, position, 0.0f, scaleMenu, WHITE);

    Rectangle playBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2, 200, 50};
    Rectangle optionsBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 80, 200, 50};
    Rectangle exitBtn = {GetScreenWidth()/2 - 100, GetScreenHeight() / 2 + 160, 200, 50};

    //DrawText(title, GetScreenWidth() / 2 - MeasureText(title, dynamicTitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4, dynamicTitleFontSize, WHITE);
    //DrawText(subtitle, GetScreenWidth() / 2 - MeasureText(subtitle, dynamicSubitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4 + dynamicTitleFontSize, dynamicSubitleFontSize, WHITE);

    //DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);


    // Draw title using the custom font
    Vector2 titlePos = {
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, getTranslation("game_title"), dynamicTitleFontSize, 2).x / 2,
        GetScreenHeight() / 2 - GetScreenHeight() / 4
    };
    DrawTextEx(baseFont, getTranslation("game_title"), titlePos, dynamicTitleFontSize, 2, WHITE);

    // Draw subtitle using the custom font
    Vector2 subtitlePos = {
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, getTranslation("game_subtitle"), dynamicSubitleFontSize, 2).x / 2,
        GetScreenHeight() / 2 - GetScreenHeight() / 4 + dynamicTitleFontSize
    };
    DrawTextEx(baseFont, getTranslation("game_subtitle"), subtitlePos, dynamicSubitleFontSize, 2, WHITE);

    // Draw FPS counter
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, GREEN);


    if (GuiButton(playBtn, getTranslation("menu_start"))) {
     *currentScene = STARTGAME;
    }
    if (GuiButton(optionsBtn, getTranslation("menu_options"))) {
     *currentScene = OPTIONS;
    }
    if (GuiButton(exitBtn, getTranslation("menu_exit"))) {
     windowShouldClose = true;
    }

    
    
}