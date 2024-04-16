
typedef struct {
    Vector2 position;
    Texture2D texture;
    bool isActive;
    Rectangle collider;
    bool isEquipped;
} Hoe;

void spawnHoe(Hoe *hoe, Vector2 position) {
    //hoe-> texture = texture;
    hoe-> position = position;
    hoe-> isActive = true;
    hoe-> isEquipped = false;
    hoe-> collider  = (Rectangle){position.x, position.y, 64, 64};
    
    hoe-> texture = LoadTexture("src/textures/hoe.png");
}

void drawHoe(Hoe *hoe) {
    if (hoe-> isActive) {
        
        DrawTexture(hoe->texture, hoe->position.x, hoe->position.y, WHITE);
    }
    else if(hoe-> isEquipped){
        
        //DrawRectangle(0, 0, 64, 64, LIGHTGRAY);
        DrawTexture(hoe->texture, 0, 0, WHITE);
        
    }
}
void updateHoe(Hoe *hoe){
    


}
void unloadHoe(Hoe *hoe){
    
    UnloadTexture(hoe->texture);    
}