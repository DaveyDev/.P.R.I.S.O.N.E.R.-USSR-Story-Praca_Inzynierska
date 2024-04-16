#define MAX_COFFEE_PLANTS 500



typedef struct {
    Vector2 position;
    Texture2D texture;
    bool isActive;
    Rectangle collider;
    int stageOfGrowness;
} CoffeePlant;

typedef struct {
    CoffeePlant coffeePlant[MAX_COFFEE_PLANTS];  // an array to store all the enemies
    int numPlants;  // the current number of enemies
} CoffeePlantManager;


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
void spawnCoffeePlant(CoffeePlant *coffeePlant, Vector2 position) {
    
        coffeePlant-> position = position;
        coffeePlant-> isActive = true;  
        coffeePlant-> collider = (Rectangle){position.x, position.y, 64, 64};
        coffeePlant-> stageOfGrowness = 0;
        coffeePlant-> texture = LoadTexture("src/textures/coffeePlant.png");
        
    
}

void drawCoffeePlant(CoffeePlantManager *coffeePlantManager) {
    for (int i = 0; i < coffeePlantManager-> numPlants; i++) {
        CoffeePlant *coffeePlant = &coffeePlantManager-> coffeePlant[i];
        if (coffeePlant->isActive) {
            DrawTexture(coffeePlant->texture, coffeePlant->position.x, coffeePlant->position.y, WHITE);
        } 
    }
}


void interactWithCoffeePlants(Player *player, CoffeePlantManager *coffeePlantManager, int map[MAX_ROWS][MAX_COLS], time_t lastChangeTimes[MAX_ROWS][MAX_COLS]) {
    //spawnCoffeePlant(&coffeePlantManager->coffeePlant[coffeePlantManager->numPlants], (Vector2){player->position.x, player->position.y});
      
    time_t currentTime = time(NULL);
    
    
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (map[row][col] == 41 && currentTime - lastChangeTimes[row][col] >= 30) {
                map[row][col] = 311;  // Change tile value from 41 to 311
                lastChangeTimes[row][col] = currentTime;
                spawnCoffeePlant(&coffeePlantManager->coffeePlant[coffeePlantManager->numPlants], (Vector2){col*64, row*64});
                coffeePlantManager->numPlants++;// Update the last change time
                
            }
        }
    }
}