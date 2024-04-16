
typedef struct {
    Vector2 position;
    Texture2D texture;
    Rectangle collider; // Collider representing the shop area
} Shop;

 void spawnShop(Shop *shop, Vector2 position) {
    //waterCan-> texture = texture;
    shop-> position = position;
    shop-> collider  = (Rectangle){position.x, position.y, 128, 64};
    shop-> texture = LoadTexture("src/textures/shop.png");
}

void drawShop(Shop *shop) {
  
        DrawTexture(shop->texture, shop->position.x, shop->position.y, WHITE);
    
}

void unloadShop(Shop *shop){
    
    UnloadTexture(shop->texture);    
}