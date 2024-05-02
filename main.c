#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "src/scripts/player.c"
#include "src/scripts/checkCollision.c"
#include "src/scripts/menu.c"
#include "src/scripts/textures.c"
#include "src/scripts/map.c"





int main() {
    // Load the map from the text file
    loadMap("src/map.txt");
    int scene = 0;
    //int numSeeds = 0;
    
    int coins = 0;
    
    // Raylib Initialization
    const int screenWidth = 1024;
    const int screenHeight = 768;
    
    InitWindow(screenWidth, screenHeight, ".P.R.I.S.O.N.E.R.");
    //ToggleFullscreen();

    
    loadTextures();
    
    
    Player player;
    initPlayer(&player, screenWidth, screenHeight, 213.7f); // Initial position and speed
    
   
    
    



    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        

    switch(scene){
        case 0:
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawMap();
        scene = menuScene();
        EndDrawing();

        
        break;
        
        case 1:
        
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          
        clickOnTile(&player);
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the map
        //updateMap();
        drawMap();
       
   
        DrawTexture(emptyHand, 0, 0, WHITE);
        
        //Draw hoe  
        //updateHoe(&hoe);
        //drawHoe(&hoe);
        //drawWaterCan(&waterCan);
        
        
        //drawShop(&shop);
        
        //for (int i = 0; i < coffeeSeedManager.numSeeds; i++) {
        //    CoffeeSeed *coffeeSeed = &coffeeSeedManager.coffeeSeed[i];
        //if(checkCollision(shop.collider, coffeeSeed->collider)){
        //    coffeeSeed->isActive = false;
        //    coffeeSeed->collider.x = 1000;
        //    coffeeSeed->collider.y = 1000;
        //    coins++;
        //    
        //}}
        
        //draw plants
        
        
        
        
        
        //interactWithCoffeePlants(&player, &coffeePlantManager, map, lastChangeTimes);
        //drawCoffeePlant(&coffeePlantManager);
        
        //drawCoffeeSeed(&coffeeSeedManager);
        //interactWithCoffeeSeeds(&player, &coffeeSeedManager);
        
        
        
        float deltaTime = GetFrameTime();
        updatePlayer(&player, deltaTime);
        drawPlayer(&player);
        
        int countFPS = GetFPS();
        DrawText(TextFormat("FPS: %i", countFPS), screenWidth - 100, 15, 20, BLUE);
        DrawText(TextFormat("ver. 240126"), screenWidth - 150, 600, 20, BLUE);
        DrawText(TextFormat("coins: %i", coins), 80, 15, 20, BLUE);
  
        
     
        EndDrawing();
        break;
        
    }
}
    unloadPlayer(&player);
    //unloadHoe(&hoe);
    //unloadWaterCan(&waterCan);
    //unloadCoffeeSeeds(&coffeeSeed);
    //unloadShop(&shop);
    unloadTextures();
    CloseWindow();

    return 0;
}
