#include "player.h"


void initPlayer(Player *player, int screenWidth, int screenHeight, float speed) {
    player-> position.x = screenWidth/2;
    player-> position.y = screenHeight/2;
    player-> speed = speed;
    player-> collider  = (Rectangle){player-> position.x, player->position.y, 64, 64};

    player-> texture = LoadTexture("data/textures/player.png");
    
}

void updatePlayer(Player *player, float deltaTime) {
    float speedPerSecond = player->speed * deltaTime;
    // Handle player input (e.g., arrow keys or WASD)
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D') && player-> position.x < GetScreenWidth() - 32) {
        player-> position.x += speedPerSecond;
        
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown('A') && player-> position.x > -32) {
        player->position.x -= speedPerSecond;
        
    }

    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S') && player-> position.y < GetScreenHeight() - 32) {
        player->position.y += speedPerSecond;
        
    } else if (IsKeyDown(KEY_UP) || IsKeyDown('W') && player-> position.y > -32) {
        player->position.y -= speedPerSecond;
        
    }
    
    player-> collider.x = player-> position.x;
    player-> collider.y = player-> position.y;
}




void drawPlayer(Player *player) {
    DrawTexture(player->texture, player->position.x, player->position.y, WHITE);
}

void unloadPlayer(Player *player) {
    UnloadTexture(player->texture);
}
