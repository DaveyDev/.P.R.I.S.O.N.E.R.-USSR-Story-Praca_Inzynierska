#include "player.h"
#include "../map/objects.h"
#include "../map/objects.h"
#include "../idList.h"
#include <math.h>

Player player;

void initPlayer(Player *player, int screenWidth, int screenHeight, float speed) {
    player-> position.x = screenWidth/2;
    player-> position.y = screenHeight/2;
    player-> speed = speed;
    player-> collider  = (Rectangle){player-> position.x + 2, player->position.y + 50, 16, 25};

    // Define the oval collider parameters
    player->colliderCenter = (Vector2){player->position.x + 16, player->position.y + 50};
    player->colliderRadiusX = 7;  // Half the width
    player->colliderRadiusY = 3;   // Half the height
    
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


bool CheckCollisionEllipseRec(Vector2 ellipseCenter, float radiusX, float radiusY, Rectangle rect) {
    float closestX = fmax(rect.x, fmin(ellipseCenter.x, rect.x + rect.width));
    float closestY = fmax(rect.y, fmin(ellipseCenter.y, rect.y + rect.height));

    float dx = (closestX - ellipseCenter.x) / radiusX;
    float dy = (closestY - ellipseCenter.y) / radiusY;

    return (dx * dx + dy * dy) <= 1;
}

bool CheckCollisionEllipseCircle(Vector2 ellipseCenter, float radiusX, float radiusY, Vector2 circleCenter, float circleRadius) {
    // Translate the circle position to the ellipse’s local coordinate system
    float dx = circleCenter.x - ellipseCenter.x;
    float dy = circleCenter.y - ellipseCenter.y;

    // Convert to normalized ellipse space
    float normX = dx / radiusX;
    float normY = dy / radiusY;

    // Compute the squared distance from the ellipse center in normalized space
    float distanceSquared = normX * normX + normY * normY;

    // If the point is inside the ellipse, there's definitely a collision
    if (distanceSquared <= 1.0f) return true;

    // Find the closest point on the ellipse to the circle center
    float angle = atan2(normY, normX); // Angle from ellipse center to circle center
    float closestX = radiusX * cos(angle);
    float closestY = radiusY * sin(angle);

    // Convert back to world space
    closestX += ellipseCenter.x;
    closestY += ellipseCenter.y;

    // Compute the actual distance from the closest point on the ellipse to the circle center
    float distX = circleCenter.x - closestX;
    float distY = circleCenter.y - closestY;
    float distance = sqrtf(distX * distX + distY * distY);

    // Check if the closest point is within the circle radius
    return distance <= circleRadius;
}



void updatePlayer(Player *player, float deltaTime, int **objects, int **details, int rows, int cols, Camera2D camera) {
    float speedPerSecond = player->speed * deltaTime;
    Vector2 newColliderCenter = player->colliderCenter;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D')) {
        newColliderCenter.x += speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.x += speedPerSecond;
        }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown('A')) {
        newColliderCenter.x -= speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.x -= speedPerSecond;
        }
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S')) {
        newColliderCenter.y += speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.y += speedPerSecond;
        }
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown('W')) {
        newColliderCenter.y -= speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.y -= speedPerSecond;
        }
    }

    // Update collider position
    Vector2 worldPos = GetScreenToWorld2D((Vector2){player->position.x, player->position.y}, camera);
    player->collider.x = worldPos.x + 4;
    player->collider.y = worldPos.y + 23;

    player->colliderCenter.x = worldPos.x + 13;
    player->colliderCenter.y = worldPos.y + 46;
}




bool checkCollisionWithObjects(Vector2 colliderCenter, float radiusX, float radiusY, int **objects, int **details, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int objectID = objects[row][col];
            int detailID = details[row][col];

            if (objectID == 0 && detailID == 0) continue;

            Rectangle objectCollider = {col * 32, row * 32, 32, 32};

            if (isWallLike(objectID)) {
                if (CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider)) {
                    return true;
                }
            }

            if (objectID >= 1000 && objectID <= 1999) {
                Vector2 circleCenter = {col * 32 + 16, row * 32 + 16};
                float circleRadius = 8.0f;
                if (CheckCollisionEllipseCircle(colliderCenter, radiusX, radiusY, circleCenter, circleRadius)) {
                    return true;
                }
            }

            if (objectID >= 2000 && objectID <= 2999 && objectID != WOODEN_FLOOR && objectID != STONE_FLOOR) {
                Vector2 circleCenter = {col * 32 + 16, row * 32 + 16};
                float circleRadius = 12.0f;
                if (CheckCollisionEllipseCircle(colliderCenter, radiusX, radiusY, circleCenter, circleRadius)) {
                    return true;
                }
            }

            if ((detailID == GREY_DOOR || detailID == LIGHTGREY_DOOR) && CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider)) {
                openDoor(row, col);
            }

            if (!CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider) && (detailID == OPEN_GREY_DOOR || detailID == OPEN_LIGHTGREY_DOOR)) {
                closeDoor(row, col);
            }
        }
    }
    return false;
}



