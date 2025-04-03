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



void endGameScene(){

    Rectangle exitBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2 + 160, 300, 50};

    ClearBackground(bgColor);

    DrawText("you died", GetScreenWidth() / 2 , GetScreenHeight() / 2 , 20, WHITE);

    showFPS();

    if (GuiButton(exitBtn, getTranslation("pause_exit"))) {
        
        
        *currentScene = MENU;
    }

}