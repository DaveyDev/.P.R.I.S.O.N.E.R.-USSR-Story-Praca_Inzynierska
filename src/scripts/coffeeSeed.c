#define MAX_COFFEE_SEEDS 2048



typedef struct {
    Vector2 position;
    Texture2D texture;
    bool isActive;
    Rectangle collider;
    bool isEquipped;
} CoffeeSeed;

typedef struct {
    CoffeeSeed coffeeSeed[MAX_COFFEE_SEEDS];  // an array to store all the enemies
    int numSeeds;  // the current number of enemies
} CoffeeSeedManager;

void despawnCoffeeSeed(CoffeeSeed *coffeeSeed) {
    // Mark the seed as inactive
    coffeeSeed->isActive = false;
    // Additionally, you can reset its properties or free any resources if needed
}


/*
void spawnCoffeeSeed(CoffeeSeed *coffeeSeed, Vector2 position) {
    //coffeeSeed-> texture = texture;
    coffeeSeed-> position = position;
    coffeeSeed-> isActive = true;
    coffeeSeed-> isEquipped = false;
    coffeeSeed-> collider  = (Rectangle){position.x, position.y, 64, 64};
    
    coffeeSeed-> texture = LoadTexture("src/textures/coffeeSeed.png");
}
*/
void spawnCoffeeSeed(CoffeeSeed *coffeeSeed, Vector2 position) {
    
        coffeeSeed->position = position;
        coffeeSeed->isActive = true;
        coffeeSeed->isEquipped = false;
        coffeeSeed->collider = (Rectangle){position.x, position.y, 64, 64};
        coffeeSeed->texture = LoadTexture("src/textures/coffeeSeed.png");
        
    
}

void drawCoffeeSeed(CoffeeSeedManager *coffeeSeedManager) {
    for (int i = 0; i < coffeeSeedManager-> numSeeds; i++) {
        CoffeeSeed *coffeeSeed = &coffeeSeedManager-> coffeeSeed[i];
        if (coffeeSeed->isActive) {
            DrawTexture(coffeeSeed->texture, coffeeSeed->position.x, coffeeSeed->position.y, WHITE);
        } else if (coffeeSeed->isEquipped) {
            DrawTexture(coffeeSeed->texture, 0, 0, WHITE); // Example: Draw equipped coffeeSeed at a fixed position
        }
    }
}
/*
void drawCoffeeSeed(CoffeeSeed *coffeeSeed) {
    if (coffeeSeed-> isActive) {
        
        DrawTexture(coffeeSeed->texture, coffeeSeed->position.x, coffeeSeed->position.y, WHITE);
    }
    else if(coffeeSeed-> isEquipped){
        
        //DrawRectangle(0, 0, 64, 64, LIGHTGRAY);
        DrawTexture(coffeeSeed->texture, 0, 0, WHITE);
        
    }
}*/
void updateCoffeeSeed(CoffeeSeed *coffeeSeed){
    


}
void interactWithCoffeeSeeds(Player *player, CoffeeSeedManager *coffeeSeedManager) {
    for (int i = 0; i < coffeeSeedManager->numSeeds; i++) {
        CoffeeSeed *coffeeSeed = &coffeeSeedManager->coffeeSeed[i];
        
        if (checkCollision(player->collider, coffeeSeed->collider) && IsKeyDown(KEY_E) && player->isHandEmpty) {
            coffeeSeed->isActive = false;
            coffeeSeed->isEquipped = true;
            player->isHandEmpty = false;
            // Perform actions associated with picking up the coffee seed
        } 
        
        if (coffeeSeed->isEquipped && IsKeyDown(KEY_Q)) {
            coffeeSeed->isActive = true;
            coffeeSeed->isEquipped = false;
            coffeeSeed->position = player->position;
            coffeeSeed->collider.x = player->position.x;
            coffeeSeed->collider.y = player->position.y;
            player->isHandEmpty = true;
        }
        
        
        
        
        
    }
}

void unloadCoffeeSeeds(CoffeeSeed *coffeeSeed){
    
    for (int i = 0; i < 2048; i++) {
        UnloadTexture(coffeeSeed[i].texture);
    }
    //UnloadTexture(coffeeSeed->texture);    
}