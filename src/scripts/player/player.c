#include "player.h"

void initPlayer(Player *player, int screenWidth, int screenHeight, float speed) {
    player-> position.x = screenWidth/2;
    player-> position.y = screenHeight/2;
    player-> speed = speed;
    player-> collider  = (Rectangle){player-> position.x + 16, player->position.y + 50, 32, 80};
    

    

    player-> playerTexture = LoadTexture("data/textures/playerSet.png");
    player-> playerAnimation[0] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 0, 32, 64}, 
        (Rectangle){32, 0, 32, 64},
        (Rectangle){64, 0, 32, 64},
        (Rectangle){96, 0, 32, 64},
        (Rectangle){128, 0, 32, 64},
        (Rectangle){160, 0, 32, 64},
        (Rectangle){192, 0, 32, 64},
        (Rectangle){224, 0, 32, 64}
    }, 8);
    player-> playerAnimation[1] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 64, 32, 64}, 
        (Rectangle){32, 64, 32, 64},
        (Rectangle){64, 64, 32, 64},
        (Rectangle){96, 64, 32, 64},
        (Rectangle){128, 64, 32, 64},
        (Rectangle){160, 64, 32, 64},
        (Rectangle){192, 64, 32, 64},
        (Rectangle){224, 64, 32, 64}
    }, 8);
    player-> playerAnimation[2] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 128, 32, 64}, 
        (Rectangle){32, 128, 32, 64},
        (Rectangle){64, 128, 32, 64},
        (Rectangle){96, 128, 32, 64},
        (Rectangle){128, 128, 32, 64},
        (Rectangle){160, 128, 32, 64},
        (Rectangle){192, 128, 32, 64},
        (Rectangle){224, 128, 32, 64}
    }, 8);
    
}

void updatePlayer(Player *player, float deltaTime) { //for testing purpose only 
    float speedPerSecond = player->speed * deltaTime;
    // Handle player input (e.g., arrow keys or WASD)
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D') && player-> position.x < GetScreenWidth() - 64) {
        player-> position.x += speedPerSecond;
        
        
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown('A') && player-> position.x > 0) {
        player->position.x -= speedPerSecond;
        
    }

    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S') && player-> position.y < GetScreenHeight() - 140) {
        player->position.y += speedPerSecond;
        
    } else if (IsKeyDown(KEY_UP) || IsKeyDown('W') && player-> position.y > - 48 ) {
        player->position.y -= speedPerSecond;
        
    }
    
    player-> collider.x = player-> position.x + 16;
    player-> collider.y = player-> position.y + 50;
}



void drawPlayer(Player *player) {
    Vector2 position = {player->position.x, player->position.y};
    Rectangle dest = {player-> position.x, player->position.y, 64, 128};
    Vector2 origin = {0};
    //DrawRectangle(player->collider.x, player->collider.y, player->collider.width, player->collider.height, RED);

    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S')) {
        drawSpriteAnimationPro(player-> playerAnimation[1], dest, origin, 0, WHITE);
    } else if (IsKeyDown(KEY_UP) || IsKeyDown('W')) {
        drawSpriteAnimationPro(player-> playerAnimation[2], dest, origin, 0, WHITE);
    } else {
        drawSpriteAnimationPro(player-> playerAnimation[0], dest, origin, 0, WHITE);

    }
}


void unloadPlayer(Player *player) {
    UnloadTexture(player->playerTexture);
    disposeSpriteAnimation(player-> playerAnimation[0]);
    disposeSpriteAnimation(player-> playerAnimation[1]);
    disposeSpriteAnimation(player-> playerAnimation[2]);


}
