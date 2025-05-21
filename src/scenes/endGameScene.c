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



const int subtitleFontSize3 = 24;

void endGameScene() {
    static bool messageSelected = false;
    static const char* deathMessage = NULL;

    static int randomIndex = 0;

    if (!messageSelected) {
        const int numDeathMessages = 5;
        randomIndex = GetRandomValue(1, numDeathMessages);

        char keyBuffer[64];
        snprintf(keyBuffer, sizeof(keyBuffer), "ending_bad_death_%d", randomIndex);
        deathMessage = getTranslation(keyBuffer);

        messageSelected = true;
    }

    Rectangle exitBtn = { GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + GetScreenHeight() / 4, 300, 50 };

    ClearBackground(bgColor);

    int dynamicFontSize = subtitleFontSize3 * (GetScreenWidth() / 800);
    Vector2 deathMsgPos = {
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, deathMessage, dynamicFontSize, 2).x / 2,
        GetScreenHeight() / 3 - dynamicFontSize / 2
    };

    DrawTextEx(baseFont, deathMessage, deathMsgPos, dynamicFontSize, 2, WHITE);

    showFPS();

    if (GuiButton(exitBtn, getTranslation("pause_exit"))) {
        messageSelected = false; // Reset for next time the scene is entered
        *currentScene = MENU;
    }
}

