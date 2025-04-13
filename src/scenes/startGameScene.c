//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/translation.h"
#include "string.h"
#include "../scripts/newGame.h"
#include <unistd.h>  // For access()
#include "../scripts/player/inventory.h"
#include "../scripts/cheats/debugCMD.h"
#include "../scripts/NPC/npc.h"
#include "../scripts/textures.h"
#include "../scripts/dayCycle.h"



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


    showFPS();




    if (GuiButton(playBtn, getTranslation("startgame_newgame"))) { // starts new game
        
        strcpy(mapName, "test");
        snprintf(mapPath, sizeof(mapPath), "data/levels/%s/%s.map", mapName, mapName);
        snprintf(itemPath, sizeof(itemPath), "data/levels/%s/items.dat", mapName);
        //snprintf(chestPath, sizeof(chestPath), "data/levels/%s/chests.dat", mapName);
        makeGameSave(mapName); // creates save by copying files from level to save folder
        initInventory();
        initDayCycle();

        numInmates = loadNPCsFromFile("data/saves/save1/inmates.dat", inmates, 10, inmateTexture);
        numGuards = loadNPCsFromFile("data/saves/save1/guards.dat", guards, 5, guardTexture);
        //loadChests();
        isNewGame = true;
        *currentScene = GAME;
    }
    

    
    bool saveExists = (access("data/saves/save1/test.map", F_OK) == 0); // Check if the file exists
    GuiSetState(saveExists ? STATE_NORMAL : STATE_DISABLED); // Set button state

    if (GuiButton(optionsBtn, getTranslation("startgame_loadsave"))) { // starts game from a save
        strcpy(mapName, "test");
        snprintf(mapPath, sizeof(mapPath), "data/saves/save1/%s.map", mapName);
        snprintf(itemPath, sizeof(itemPath), "data/saves/save1/items.dat");
        //if(access(mapPath, F_OK) == 0) *currentScene = GAME;
        initInventory();
        loadInventory();
        numInmates = loadNPCsFromFile("data/saves/save1/inmates.dat", inmates, 10, inmateTexture);
        numGuards = loadNPCsFromFile("data/saves/save1/guards.dat", guards, 5, guardTexture);
        isNewGame = false;
        *currentScene = GAME;  
        
    }
    GuiSetState(STATE_NORMAL); // Reset GUI state

    if (GuiButton(exitBtn, getTranslation("general_back"))) {
        *currentScene = MENU;
    }


    
    
}

