#include "player.h"
#include "../map/objects.h"

void initPlayer(Player *player, int screenWidth, int screenHeight, float speed) {
    player-> position.x = screenWidth/2;
    player-> position.y = screenHeight/2;
    player-> speed = speed;
    player-> collider  = (Rectangle){player-> position.x + 32, player->position.y + 50, 16, 2};
    
    player-> playerTexture = LoadTexture("data/textures/playerSet.png");
    player-> playerAnimation[0] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 0, 32, 64}, 
        (Rectangle){32, 0, 32, 64},
        (Rectangle){64, 0, 32, 64},
        (Rectangle){96, 0, 32, 64},
        
    }, 4);
    player-> playerAnimation[1] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 64, 32, 64}, 
        (Rectangle){32, 64, 32, 64},
        (Rectangle){64, 64, 32, 64},
        (Rectangle){96, 64, 32, 64},
        
    }, 4);
    player-> playerAnimation[2] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 128, 32, 64}, 
        (Rectangle){32, 128, 32, 64},
        (Rectangle){64, 128, 32, 64},
        (Rectangle){96, 128, 32, 64},
        
    }, 4);
    player-> playerAnimation[3] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 192, 32, 64}, 
        (Rectangle){32, 192, 32, 64},
        (Rectangle){64, 192, 32, 64},
        (Rectangle){96, 192, 32, 64},
        
    }, 4);
    player-> playerAnimation[4] = createSpriteAnimation(player-> playerTexture, 3, (Rectangle[]){
        (Rectangle){0, 256, 32, 64}, 
        (Rectangle){32, 256, 32, 64},
        (Rectangle){64, 256, 32, 64},
        (Rectangle){96, 256, 32, 64},
        
    }, 4);
    
}

/*void updatePlayer(Player *player, float deltaTime) { //for testing purpose only 
    float speedPerSecond = player->speed * deltaTime;
    // Handle player input (e.g., arrow keys or WASD)
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D') && player-> position.x < GetScreenWidth() - 64) {
        player-> position.x += speedPerSecond;
    } 
    
    if (IsKeyDown(KEY_LEFT) || IsKeyDown('A') && player-> position.x > 0) {
        player->position.x -= speedPerSecond;
    }

    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S') && player-> position.y < GetScreenHeight() - 140) {
        player->position.y += speedPerSecond;
    } 
    
    if (IsKeyDown(KEY_UP) || IsKeyDown('W') && player-> position.y > - 48 ) {
        player->position.y -= speedPerSecond;
    }
    
    player-> collider.x = player-> position.x + 16;
    player-> collider.y = player-> position.y + 50;
}
*/
void updatePlayer(Player *player, float deltaTime, int **objects, int rows, int cols, Camera2D camera) {
    float speedPerSecond = player->speed * deltaTime;
    Rectangle newCollider = player->collider;

    if ((IsKeyDown(KEY_RIGHT) || IsKeyDown('D'))) {
        newCollider.x += speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, rows, cols)) {
            player->position.x += speedPerSecond;
        }
    }
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown('A'))) {
        newCollider.x -= speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, rows, cols)) {
            player->position.x -= speedPerSecond;
        }
    }
    if ((IsKeyDown(KEY_DOWN) || IsKeyDown('S'))) {
        newCollider.y += speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, rows, cols)) {
            player->position.y += speedPerSecond;
        }
    }
    if ((IsKeyDown(KEY_UP) || IsKeyDown('W'))) {
        newCollider.y -= speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, rows, cols)) {
            player->position.y -= speedPerSecond;
        }
    }

    // Update collider position
    Vector2 worldPos = GetScreenToWorld2D((Vector2){player->position.x, player->position.y}, camera);
    player->collider.x = worldPos.x + 4;
    player->collider.y = worldPos.y + 48;
}





void drawPlayer(Player *player) {
    Vector2 position = {player->position.x, player->position.y};
    Rectangle dest = {player-> position.x, player->position.y, 64, 128};
    Vector2 origin = {0};
    //DrawRectangle(player->collider.x, player->collider.y, player->collider.width, player->collider.height, RED);

    if (IsKeyDown(KEY_LEFT) || IsKeyDown('A')) {
        drawSpriteAnimationPro(player-> playerAnimation[4], dest, origin, 0, WHITE);
    } else if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D')) {
        drawSpriteAnimationPro(player-> playerAnimation[3], dest, origin, 0, WHITE);
    } else if (IsKeyDown(KEY_UP) || IsKeyDown('W')) {
        drawSpriteAnimationPro(player-> playerAnimation[2], dest, origin, 0, WHITE);
    } else if (IsKeyDown(KEY_DOWN) || IsKeyDown('S')) {
        drawSpriteAnimationPro(player-> playerAnimation[1], dest, origin, 0, WHITE);
    } else {
        drawSpriteAnimationPro(player-> playerAnimation[0], dest, origin, 0, WHITE);
    }
}


void unloadPlayer(Player *player) {
    UnloadTexture(player->playerTexture);
    disposeSpriteAnimation(player-> playerAnimation[0]);
    disposeSpriteAnimation(player-> playerAnimation[1]);
    disposeSpriteAnimation(player-> playerAnimation[2]);
    disposeSpriteAnimation(player-> playerAnimation[3]);
    disposeSpriteAnimation(player-> playerAnimation[4]);

}


bool checkCollisionWithObjects(Rectangle playerCollider, int **objects, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int objectID = objects[row][col];

            // Skip empty spaces
            if (objectID == 0) continue;

            // If it's a wall or other rectangular object
            if (isWallLike(objectID) || (objectID >= 2000 && objectID <= 2999)) {
                Rectangle objectCollider = { col * 32, row * 32, 32, 32 };
                if (CheckCollisionRecs(playerCollider, objectCollider)) {
                    return true;
                }
            }
            // If it's a tree (circular collision, IDs 1000-1999)
            else if (objectID >= 1000 && objectID <= 1999) {
                Vector2 circleCenter = { col * 32 + 16, row * 32 + 16 }; // Center of the tree
                float circleRadius = 8.0f; // Adjust radius as needed
                if (CheckCollisionCircleRec(circleCenter, circleRadius, playerCollider)) {
                    return true;
                }
            }
        }
    }
    return false; // No collision
}


