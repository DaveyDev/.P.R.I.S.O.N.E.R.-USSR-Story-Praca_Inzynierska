//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/map/map.h"
#include "../scripts/translation.h"
#include "../scripts/items/items.h"
#include "../scripts/textures.h"
#include "../scripts/player/inventory.h"
#include "../scripts/cheats/debugCMD.h"

const int subtitleFontSize2 = 24;

void winGameScene(){

    Rectangle exitBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2 + 160, 300, 50};

    ClearBackground(bgColor);

    // ending text
    int dynamicEndingFontSize = subtitleFontSize2 * (GetScreenWidth() / 800);

    Vector2 endingLine1Pos = { 
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, getTranslation("ending_good_escape_line1"), dynamicEndingFontSize, 2).x / 2, 
        GetScreenHeight() / 2 - dynamicEndingFontSize - GetScreenHeight() / 8 // Adjust the Y position for the first line
    };
    Vector2 endingLine2Pos = { 
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, getTranslation("ending_good_escape_line2"), dynamicEndingFontSize, 2).x / 2, 
        GetScreenHeight() / 2 + dynamicEndingFontSize - GetScreenHeight() / 8// Adjust the Y position for the second line
    };
    DrawTextEx(baseFont, getTranslation("ending_good_escape_line1"), endingLine1Pos, dynamicEndingFontSize, 2, WHITE);
    DrawTextEx(baseFont, getTranslation("ending_good_escape_line2"), endingLine2Pos, dynamicEndingFontSize, 2, WHITE);
    


    showFPS();

    if (GuiButton(exitBtn, getTranslation("pause_exit"))) {
        
        
        *currentScene = MENU;
    }

}