#include "player.h"
#include "../map/objects.h"
#include "../map/objects.h"
#include "../idList.h"

Player player;

void initPlayer(Player *player, int screenWidth, int screenHeight, float speed) {
    player-> position.x = screenWidth/2;
    player-> position.y = screenHeight/2;
    player-> speed = speed;
    player-> collider  = (Rectangle){player-> position.x + 32, player->position.y + 50, 16, 2};

    // Define the oval collider parameters
    player->colliderCenter = (Vector2){player->position.x + 16, player->position.y + 50};
    player->colliderRadiusX = 10;  // Half the width
    player->colliderRadiusY = 5;   // Half the height
    
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


void updatePlayer(Player *player, float deltaTime, int **objects, int **details, int rows, int cols, Camera2D camera) {
    float speedPerSecond = player->speed * deltaTime;
    Rectangle newCollider = player->collider;

    if ((IsKeyDown(KEY_RIGHT) || IsKeyDown('D'))) {
        newCollider.x += speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, details, rows, cols)) {
            player->position.x += speedPerSecond;
        }
    }
    if ((IsKeyDown(KEY_LEFT) || IsKeyDown('A'))) {
        newCollider.x -= speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, details, rows, cols)) {
            player->position.x -= speedPerSecond;
        }
    }
    if ((IsKeyDown(KEY_DOWN) || IsKeyDown('S'))) {
        newCollider.y += speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, details, rows, cols)) {
            player->position.y += speedPerSecond;
        }
    }
    if ((IsKeyDown(KEY_UP) || IsKeyDown('W'))) {
        newCollider.y -= speedPerSecond;
        if (!checkCollisionWithObjects(newCollider, objects, details, rows, cols)) {
            player->position.y -= speedPerSecond;
        }
    }

    // Update collider position
    Vector2 worldPos = GetScreenToWorld2D((Vector2){player->position.x, player->position.y}, camera);
    player->collider.x = worldPos.x + 4;
    player->collider.y = worldPos.y + 48;

    player->colliderCenter.x = worldPos.x + 4;
    player->colliderCenter.y = worldPos.y + 48;
}





void drawPlayer(Player *player) {
    Vector2 position = {player->position.x, player->position.y};
    Rectangle dest = {player-> position.x, player->position.y, 64, 128};
    Vector2 origin = {0};
    //DrawRectangle(player->collider.x, player->collider.y, player->collider.width, player->collider.height, RED);
    //DrawRectangle(player->position.x, player->position.y, 30,30, GREEN);


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



bool checkCollisionWithObjects(Rectangle playerCollider, int **objects, int **details, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int objectID = objects[row][col];
            int detailID = details[row][col];

            // Skip empty spaces
            if (objectID == 0 && detailID == 0) continue;

            Rectangle objectCollider = { col * 32, row * 32, 32, 32 };

            // If object is solid (e.g., wall)
            if (isWallLike(objectID)) {
                if (CheckCollisionRecs(playerCollider, objectCollider)) {
                    return true;
                }
            }

            // Check trees (circular collision, IDs 1000-1999)
            if (objectID >= 1000 && objectID <= 1999) {
                Vector2 circleCenter = { col * 32 + 16, row * 32 + 16 };
                float circleRadius = 8.0f;
                if (CheckCollisionCircleRec(circleCenter, circleRadius, playerCollider)) {
                    return true;
                }
            }

            
            
           
            // Check placeables (IDs 2000-2999)
            if (objectID >= 2000 && objectID <= 2999 && objectID != WOODEN_FLOOR && objectID != STONE_FLOOR) {
                Vector2 circleCenter = { col * 32 + 16, row * 32 + 16 };
                float circleRadius = 12.0f;
                if (CheckCollisionCircleRec(circleCenter, circleRadius, playerCollider)) {
                    return true;
                }
            }


             // Special case: Floors (IDs 5000-5999 in details layer) -> No collision
            //if (detailID == WOODEN_FLOOR || detailID == STONE_FLOOR) {
                //return false;  // Ignore collision for special floor tiles
            //}

            if(detailID == GREY_DOOR || detailID == LIGHTGREY_DOOR){
                if (CheckCollisionRecs(playerCollider, objectCollider)) {
                    openDoor(row, col);
                    
            }
            }
            if(!CheckCollisionRecs(playerCollider, objectCollider) && detailID == OPEN_GREY_DOOR || 
            !CheckCollisionRecs(playerCollider, objectCollider) && detailID == OPEN_LIGHTGREY_DOOR){
                closeDoor(row, col);
            }

        }
    }
    return false; // No collision detected
}


