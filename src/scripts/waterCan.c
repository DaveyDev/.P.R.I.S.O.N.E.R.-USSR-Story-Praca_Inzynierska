
typedef struct {
    Vector2 position;
    Texture2D texture;
    bool isActive;
    Rectangle collider;
    bool isEquipped;
} WaterCan;

void spawnWaterCan(WaterCan *waterCan, Vector2 position) {
    //waterCan-> texture = texture;
    waterCan-> position = position;
    waterCan-> isActive = true;
    waterCan-> isEquipped = false;
    waterCan-> collider  = (Rectangle){position.x, position.y, 64, 64};
    
    waterCan-> texture = LoadTexture("src/textures/waterCan.png");
}

void drawWaterCan(WaterCan *waterCan) {
    if (waterCan-> isActive) {
        
        DrawTexture(waterCan->texture, waterCan->position.x, waterCan->position.y, WHITE);
    }
    else if(waterCan-> isEquipped){
        
        //DrawRectangle(0, 0, 64, 64, LIGHTGRAY);
        DrawTexture(waterCan->texture, 0, 0, WHITE);
        
    }
}
void updateWaterCan(WaterCan *waterCan){
    


}
void unloadWaterCan(WaterCan *waterCan){
    
    UnloadTexture(waterCan-> texture);    
}