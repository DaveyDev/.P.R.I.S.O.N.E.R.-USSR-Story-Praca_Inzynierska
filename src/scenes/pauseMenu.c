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
#include "../scripts/NPC/npc.h"




const int pauseFontSize = 48;



 


void pauseScene(){
    
    
    mousePoint = GetMousePosition();
    
     // Dynamically adjust font size based on resolution
    int dynamicTitleFontSize = pauseFontSize * (GetScreenWidth() / 800);  // Scale font size relative to base resolution

    ClearBackground(bgColor);

    Vector2 position = { -100, 0 };  // Position where the texture will be drawn
    float scaleMenu = (float)GetScreenHeight() / 500.0f;
    DrawTextureEx(pauseMenuBackground, position, 0.0f, scaleMenu, WHITE);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();


        // Calculate 1/16 smaller than window size
    int rectWidth = screenWidth * 12 / 16;
    int rectHeight = screenHeight * 12 / 16;

    // Center position (rectangle's center)
    Vector2 rectPosition = { screenWidth / 2, screenHeight / 2 };
    // Origin at the center
    Vector2 origin = { rectWidth / 2, rectHeight / 2 };

    // Draw the rectangle centered on screen
    DrawRectanglePro((Rectangle){ rectPosition.x, rectPosition.y, rectWidth, rectHeight }, 
                 origin, 0, greySeeThroughColor);


    //drawMap(camera);

    Rectangle playBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2, 300, 50};
    Rectangle optionsBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2 + 80, 300, 50};
    Rectangle exitBtn = {GetScreenWidth()/2 - 150, GetScreenHeight() / 2 + 160, 300, 50};

    //DrawText(pause, GetScreenWidth() / 2 - MeasureText(pause, dynamicTitleFontSize) / 2, GetScreenHeight() / 2 - GetScreenHeight() / 4, dynamicTitleFontSize, WHITE);
    Vector2 pausePos = { 
        GetScreenWidth() / 2 - MeasureTextEx(baseFont, getTranslation("pause_menu_name"), dynamicTitleFontSize, 2).x / 2, 
        GetScreenHeight() / 2 - GetScreenHeight() / 4 
    };
    DrawTextEx(baseFont, getTranslation("pause_menu_name"), pausePos, dynamicTitleFontSize, 2, WHITE);


    showFPS();




    
    if (GuiButton(optionsBtn, getTranslation("pause_resume"))) {
        *currentScene = GAME;
    }
    if (GuiButton(exitBtn, getTranslation("pause_exit"))) {
        saveItems();
        saveInventory();
        savePlayerStats(&player, camera);
        char chestPath[256];
        
        snprintf(chestPath, sizeof(chestPath), "data/saves/save1/chests.dat");
        saveChests(chestPath);

        char mapPathSave[256] = "\0";

        snprintf(mapPathSave, sizeof(mapPathSave), "data/saves/save1/%s.map", mapName);
        saveMap(mapPathSave);

        // Saving
        saveNPCsToFile("data/saves/save1/inmates.dat", inmates, numInmates);
        saveNPCsToFile("data/saves/save1/guards.dat", guards, numGuards);

        freeProcessedDoors(rows);

        *currentScene = MENU;
    }


    if(IsKeyPressed(KEY_ESCAPE)) {
        *currentScene = GAME;
        
       
    } 

    
    
    
    
}