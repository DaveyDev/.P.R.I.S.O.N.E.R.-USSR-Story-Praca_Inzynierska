#include "player.h"
#include "../map/objects.h"
#include "../map/objects.h"
#include "../items/idList.h"
#include <math.h>
#include "../global.h"
#include <stdio.h>
#include <string.h>
#include "../sound/soundManager.h"
#include "../dayCycle.h"
#include "../items/idList.h"
#include "inventory.h"
#include "../NPC/npc.h"
#include "combat.h"
#include "../NPC/startBehavior.h"

Player player;

const float damageCooldown = 0.1f; // 0.5 seconds between damage
const int barbedWireDamage = 1;



void initPlayer(Player *player, int screenWidth, int screenHeight, float speed) {
    player-> position.x = screenWidth/2;
    player-> position.y = screenHeight/2;
    //player-> position = (Vector2) {32 * 10, 32 * 10}; // tile (10, 5) as example
    //player-> position = (Vector2) {screenWidth/2, screenHeight/2};

    player-> speed = speed;
    player-> collider  = (Rectangle){player-> position.x + 2, player->position.y + 50, 16, 25};
    player->footstepTimer = 0.5f;

    // Define the oval collider parameters
    player->colliderCenter = (Vector2){player->position.x + 16, player->position.y + 50};
    player->colliderRadiusX = 7;  // Half the width
    player->colliderRadiusY = 3;   // Half the height
    
    player-> playerTexture = LoadTexture("data/textures/playerSet.png");
    player-> playerAnimation[0] = createSpriteAnimation(player-> playerTexture, 4, (Rectangle[]){
        (Rectangle){0, 0, 32, 64}, 
        (Rectangle){32, 0, 32, 64},
        (Rectangle){64, 0, 32, 64},
        (Rectangle){96, 0, 32, 64},
        
    }, 4);
    player-> playerAnimation[1] = createSpriteAnimation(player-> playerTexture, 4, (Rectangle[]){
        (Rectangle){0, 64, 32, 64}, 
        (Rectangle){32, 64, 32, 64},
        (Rectangle){64, 64, 32, 64},
        (Rectangle){96, 64, 32, 64},
        
    }, 4);
    player-> playerAnimation[2] = createSpriteAnimation(player-> playerTexture, 4, (Rectangle[]){
        (Rectangle){0, 128, 32, 64}, 
        (Rectangle){32, 128, 32, 64},
        (Rectangle){64, 128, 32, 64},
        (Rectangle){96, 128, 32, 64},
        
    }, 4);
    player-> playerAnimation[3] = createSpriteAnimation(player-> playerTexture, 4, (Rectangle[]){
        (Rectangle){0, 192, 32, 64}, 
        (Rectangle){32, 192, 32, 64},
        (Rectangle){64, 192, 32, 64},
        (Rectangle){96, 192, 32, 64},
        
    }, 4);
    player-> playerAnimation[4] = createSpriteAnimation(player-> playerTexture, 4, (Rectangle[]){
        (Rectangle){0, 256, 32, 64}, 
        (Rectangle){32, 256, 32, 64},
        (Rectangle){64, 256, 32, 64},
        (Rectangle){96, 256, 32, 64},
        
    }, 4);

    player-> food = 20.0f;
    player-> extraFood = 0.0f;
    player-> health = 30.0f;
    player-> extraHealth = 0.0f;

    player-> maxFood = 20;
    player-> maxHealth = 30;

    player-> lastDamageTime = 0.0f;

    player-> wasKnockedOutToday = false;
    
}



void drawPlayer(Player *player) {
    Vector2 position = {player->position.x, player->position.y};
    Rectangle dest = {player-> position.x, player->position.y, 64, 128};
    Vector2 origin = {0};


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


/*
void updatePlayer(Player *player, float deltaTime, int **objects, int **details, int rows, int cols, Camera2D camera) {
    float speedPerSecond = player->speed * deltaTime;
    Vector2 newColliderCenter = player->colliderCenter;
    bool isMoving = false;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D')) {
        newColliderCenter.x += speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.x += speedPerSecond;
            isMoving = true;
        }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown('A')) {
        newColliderCenter.x -= speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.x -= speedPerSecond;
            isMoving = true;
        }
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S')) {
        newColliderCenter.y += speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.y += speedPerSecond;
            isMoving = true;
        }
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown('W')) {
        newColliderCenter.y -= speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.y -= speedPerSecond;
            isMoving = true;
        }
    }

    calculatePlayerSteps(isMoving, deltaTime);
    handlePickupWithE(); 

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        if (!tryAttackNPCs(worldPos, guards, numGuards)) {
            tryAttackNPCs(worldPos, inmates, numInmates);
        }
    }

    //if(IsKeyPressed(KEY_G)) startRollCallForAllNPCs();
    //if(IsKeyPressed(KEY_H)) assignTradesToAllNPCs(inmates, numInmates);
    //if(IsKeyPressed(KEY_F)) startSleepForAllNPCs();

    //if(IsKeyPressed(KEY_V)) startWorkForAllNPCs();

    

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), camera);
    for (int i = 0; i < numInmates; i++) {
        Rectangle hitbox = { inmates[i].position.x - 16, inmates[i].position.y - 32, 32, 32 };
        if (CheckCollisionPointRec(worldMouse, hitbox)) {
            if (inmates[i].behavior != BEHAVIOR_TALKING) {
                inmates[i].lastBehavior = inmates[i].behavior;
                inmates[i].behavior = BEHAVIOR_TALKING;
                inmates[i].isTalking = true;

                //activeTradeNPC = &inmates[i];
                //activeTradeNPCIndex = i;
                
                handleNPCClick(i);
                // Start dialog UI or job logic
                
                printf("Inmate says: 'Hey comrade... Need something?'\n"); // for testing
                //inmates[i].behavior = inmates[i].lastBehavior;
            }
        }
    }
}
*/
void updatePlayer(Player *player, float deltaTime, int **objects, int **details, int rows, int cols, Camera2D camera) {
    float speedPerSecond = player->speed * deltaTime;
    bool isMoving = false;

    Vector2 newPos = player->position;
    //Vector2 newPos2 = GetScreenToWorld2D(player->position, camera);

    //printf("player pos: %f, %f\n", player->position.x, player->position.y);
    //printf("player pos: %f, %f\n", newPos2.x, newPos2.y);
    // Try horizontal movement
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown('D')) {
        Vector2 newColliderCenter = player->colliderCenter;
        newColliderCenter.x += speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            newPos.x += speedPerSecond;
            isMoving = true;
        }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown('A')) {
        Vector2 newColliderCenter = player->colliderCenter;
        newColliderCenter.x -= speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            newPos.x -= speedPerSecond;
            isMoving = true;
        }
    }

    // Apply horizontal movement first
    player->position.x = newPos.x;
    //camera.target.x = newPos.x;
    

    // Try vertical movement
    if (IsKeyDown(KEY_DOWN) || IsKeyDown('S')) {
        Vector2 newColliderCenter = player->colliderCenter;
        newColliderCenter.y += speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.y += speedPerSecond;
            isMoving = true;
        }
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown('W')) {
        Vector2 newColliderCenter = player->colliderCenter;
        newColliderCenter.y -= speedPerSecond;
        if (!checkCollisionWithObjects(newColliderCenter, player->colliderRadiusX, player->colliderRadiusY, objects, details, rows, cols)) {
            player->position.y -= speedPerSecond;
            
            isMoving = true;
        }
    }

    calculatePlayerSteps(isMoving, deltaTime);
    handlePickupWithE(); 
/*
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        if (!tryAttackNPCs(worldPos, guards, numGuards)) {
            tryAttackNPCs(worldPos, inmates, numInmates);
        }
    }

    // Handle NPC clicking
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), camera);
        for (int i = 0; i < numInmates; i++) {
            Rectangle hitbox = { inmates[i].position.x - 16, inmates[i].position.y - 32, 32, 32 };
            if (CheckCollisionPointRec(worldMouse, hitbox)) {
                if (inmates[i].behavior != BEHAVIOR_TALKING) {
                    inmates[i].lastBehavior = inmates[i].behavior;
                    inmates[i].behavior = BEHAVIOR_TALKING;
                    inmates[i].isTalking = true;

                    handleNPCClick(i);

                    printf("Inmate says: 'Hey comrade... Need something?'\n");
                }
            }
        }
    }
*/
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), camera);
    
    if (!tryAttackNPCs(worldMouse, guards, numGuards)) {
        tryAttackNPCs(worldMouse, inmates, numInmates);
    }

    // Example: interaction with NPC hitbox
    for (int i = 0; i < numInmates; i++) {
        Rectangle hitbox = { inmates[i].position.x - 16, inmates[i].position.y - 32, 32, 32 };
        if (CheckCollisionPointRec(worldMouse, hitbox)) {
            if (inmates[i].behavior != BEHAVIOR_TALKING) {
                    inmates[i].lastBehavior = inmates[i].behavior;
                    inmates[i].behavior = BEHAVIOR_TALKING;
                    inmates[i].isTalking = true;

                    handleNPCClick(i);

                    printf("Inmate says: 'Hey comrade... Need something?'\n");
                }
        }
    }
}



    if (IsKeyPressed(KEY_SPACE)) {
        attackMode = !attackMode;
    }

    

    // Update collider position
    Vector2 worldPos = GetScreenToWorld2D((Vector2){player->position.x, player->position.y}, camera);
    player->collider.x = worldPos.x + 4;
    player->collider.y = worldPos.y + 23;

    player->colliderCenter.x = worldPos.x + 13;
    player->colliderCenter.y = worldPos.y + 46;

    if(player->health <= 0){
        *currentScene = END_GAME;
    }

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

            if (objectID == LUMBER_WORK_CHEST) {
                Vector2 circleCenter = {col * 32 + 16, row * 32 + 16};
                float circleRadius = 8.0f;
                if (CheckCollisionEllipseCircle(colliderCenter, radiusX, radiusY, circleCenter, circleRadius)) {
                    return true;
                }
            }

            if (objectID == BARBED_WIRE || objectID == BARBED_WIRE_VERTICAL) {
                if (CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider)) {
                    float currentTime = GetTime();
                    if (currentTime - player.lastDamageTime >= damageCooldown) {
                        player.health -= barbedWireDamage;
                        player.lastDamageTime = currentTime;
                    }
                return true;
                }
            }


            if (objectID >= 2000 && objectID <= 2999 
                && objectID != WOODEN_FLOOR
                && objectID != OAK_WOODEN_FLOOR 
                && objectID != STONE_FLOOR
                && objectID != BARBED_WIRE
                && objectID != BARBED_WIRE_VERTICAL) {
                Vector2 circleCenter = {col * 32 + 16, row * 32 + 16};
                float circleRadius = 12.0f;
                if (CheckCollisionEllipseCircle(colliderCenter, radiusX, radiusY, circleCenter, circleRadius)) {
                    return true;
                }
            }
/*
            if ((detailID == GREY_DOOR || detailID == LIGHTGREY_DOOR) 
            && CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider)) {
                openDoor(row, col, true);
            }

            if (!CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider) 
            && (detailID == OPEN_GREY_DOOR || detailID == OPEN_LIGHTGREY_DOOR)) {
                closeDoor(row, col, true);
            }
*/


            if(detailID >= 2000 && detailID <= 2999
            && detailID != OPEN_GREY_DOOR
            && detailID != OPEN_LIGHTGREY_DOOR
            && detailID != FANCY_BED_UPPER
            && detailID != FANCY_BED_DOWN
            && detailID != USELESS_BED_UPPER
            && detailID != USELESS_BED_BOTTOM
            && detailID != GREEN_BED_UPPER
            && detailID != GREEN_BED_BOTTOM
            && detailID != GREY_DOOR
            && detailID != LIGHTGREY_DOOR
            ){
                if (CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider)) {
                    return true;
                }
            }

            // Winning block
            if (objectID == WINNING_BLOCK) {
                if (CheckCollisionEllipseRec(colliderCenter, radiusX, radiusY, objectCollider)) {
                    playerWon = true;  // You should declare this flag somewhere globally
                    *currentScene = WIN_GAME;
                    return false; // don't block movement
                }
            }
        }
    }
    return false;
}

/*
void drawPlayerStats(Player *player, int fontSize, Color textColor, Color bgColor) {
    int padding = 10;
    int spacing = 8;

    // Prepare text strings with extra values if present
    char healthText[64];
    if (player->extraHealth > 0) {
        snprintf(healthText, sizeof(healthText), "Health: %d (+%d)", (int)player->health, (int)player->extraHealth);
    } else {
        snprintf(healthText, sizeof(healthText), "Health: %d", (int)player->health);
    }

    char foodText[64];
    if (player->extraFood > 0) {
        snprintf(foodText, sizeof(foodText), "Food: %d (+%d)", (int)player->food, (int)player->extraFood);
    } else {
        snprintf(foodText, sizeof(foodText), "Food: %d", (int)player->food);
    }

    // Measure text width
    int healthWidth = MeasureText(healthText, fontSize);
    int foodWidth = MeasureText(foodText, fontSize);
    int maxWidth = (healthWidth > foodWidth) ? healthWidth : foodWidth;

    int boxWidth = maxWidth + padding * 2;
    int boxHeight = fontSize * 2 + spacing + padding * 2;

    int boxX = 10;
    int boxY = 10;

    // Draw background box
    DrawRectangleRounded((Rectangle){ boxX, boxY, boxWidth, boxHeight }, 0.2f, 8, bgColor);

    // Draw text
    int textX = boxX + padding;
    int textY = boxY + padding;

    DrawText(healthText, textX, textY, fontSize, textColor);
    DrawText(foodText, textX, textY + fontSize + spacing, fontSize, textColor);
}
*/
void drawPlayerStats(Player *player, int fontSize, Color textColor, Color bgColor) {
    int padding = 10;
    int spacing = 8;
    int screenWidth = GetScreenWidth();

    // Prepare text strings
    char healthText[64];
    if (player->extraHealth > 0) {
        snprintf(healthText, sizeof(healthText), "Health: %d (+%d)", (int)player->health, (int)player->extraHealth);
    } else {
        snprintf(healthText, sizeof(healthText), "Health: %d", (int)player->health);
    }

    char foodText[64];
    if (player->extraFood > 0) {
        snprintf(foodText, sizeof(foodText), "Food: %d (+%d)", (int)player->food, (int)player->extraFood);
    } else {
        snprintf(foodText, sizeof(foodText), "Food: %d", (int)player->food);
    }

    // Measure text widths
    int healthWidth = MeasureText(healthText, fontSize);
    int foodWidth = MeasureText(foodText, fontSize);

    int boxHeight = fontSize + padding * 2;
    int boxY = 10;

    // Health box (left)
    int healthBoxWidth = healthWidth + padding * 2;
    int healthBoxX = 10;
    DrawRectangleRounded((Rectangle){ healthBoxX, boxY, healthBoxWidth, boxHeight }, 0.2f, 8, bgColor);
    DrawText(healthText, healthBoxX + padding, boxY + padding, fontSize, textColor);

    // Food box (right)
    int foodBoxWidth = foodWidth + padding * 2;
    int foodBoxX = screenWidth - foodBoxWidth - 10;
    DrawRectangleRounded((Rectangle){ foodBoxX, boxY, foodBoxWidth, boxHeight }, 0.2f, 8, bgColor);
    DrawText(foodText, foodBoxX + padding, boxY + padding, fontSize, textColor);
}



void savePlayerStats(Player *player, Camera2D camera) {
    FILE *file = fopen("data/saves/save1/player.dat", "w");
    if (!file) {
        fprintf(stderr, "Error saving player stats\n");
        return;
    }

    // Save format: health:food:timeOfDay:dayCount:camera_target_x:camera_target_y
    fprintf(file, "%f:%f:%f:%d:%f:%f", player->health, player->food, getTimeOfDay(), getDayCount(),
            camera.target.x, camera.target.y);

    fclose(file);
}


int loadPlayerStats(Player *player, Camera2D camera) {
    FILE *file = fopen("data/saves/save1/player.dat", "r");
    if (!file) {
        fprintf(stderr, "Error loading player stats\n");
        return 0;
    }

    float loadedTimeOfDay;
    int loadedDayCount;
    
    

    // Read format must match saved format
    if (fscanf(file, "%f:%f:%f:%d:%f:%f", &player->health, &player->food, &loadedTimeOfDay, &loadedDayCount,
               &cameraTargetX, &cameraTargetY) != 6) {
        fprintf(stderr, "Error reading player stats\n");
        fclose(file);
        return 0;
    }

    fclose(file);

    // Apply loaded values to the day cycle
    //setDaySpeed(0.05f);  // Ensure a default speed or use your current one
    extern void setTimeOfDay(float);  // You’ll need to define this setter
    extern void setDayCount(int);     // And this one too

    setTimeOfDay(loadedTimeOfDay);
    setDayCount(loadedDayCount);

    
    return 1;
}



void useFood(float foodAmount) {
    if (foodAmount < 0) { // Eating (healing food)
        float totalFood = player.food - foodAmount; // foodAmount is negative

        if (totalFood > player.maxFood) {
            player.extraFood += totalFood - player.maxFood;
            player.food = player.maxFood;
        } else {
            player.food = totalFood;
        }
    } else { // Losing food (damage)
        float leftover = foodAmount;

        // Drain extraFood first
        if (player.extraFood > 0) {
            float usedExtra = fminf(player.extraFood, leftover);
            player.extraFood -= usedExtra;
            leftover -= usedExtra;
        }

        // Then subtract from regular food
        if (leftover > 0) {
            if (player.food >= leftover) {
                player.food -= leftover;
            } else {
                leftover -= player.food;
                player.food = 0;

                // If food is exhausted, reduce health
                player.health -= leftover;
                if (player.health < 0) player.health = 0;
            }
        }
    }
}

void useHealth(float healthAmount) {
    if (healthAmount < 0) { // Healing
        float totalHealth = player.health - healthAmount; // healthAmount is negative

        if (totalHealth > player.maxHealth) {
            player.extraHealth += totalHealth - player.maxHealth;
            player.health = player.maxHealth;
        } else {
            player.health = totalHealth;
        }
    } else { // Taking damage
        float leftover = healthAmount;

        // Drain extraHealth first
        if (player.extraHealth > 0) {
            float usedExtra = fminf(player.extraHealth, leftover);
            player.extraHealth -= usedExtra;
            leftover -= usedExtra;
        }

        // Then subtract from regular health
        if (leftover > 0) {
            player.health -= leftover;
            if (player.health < 0) player.health = 0;
        }
    }
}


void drainExtraStats(float deltaTime) {
    const float decayRate = 1.0f; // per second

    float drainAmount = decayRate * deltaTime;

    // --- Extra food logic ---
    if (player.extraFood > 0) {
        // Prioritize healing if health is not full
        if (player.health < player.maxHealth) {
            useHealth(-drainAmount); // heals, may go into extraHealth if over
            player.extraFood -= drainAmount;
            if (player.extraFood < 0) player.extraFood = 0;
        }
        // Then refill food if not full
        else if (player.food < player.maxFood) {
            useFood(-drainAmount);
            player.extraFood -= drainAmount;
            if (player.extraFood < 0) player.extraFood = 0;
        }
        // Still decay if everything is full
        else {
            player.extraFood -= drainAmount;
            if (player.extraFood < 0) player.extraFood = 0;
        }
    }

    // --- Extra health logic ---
    if (player.extraHealth > 0) {
        // Only heal if not full
        if (player.health < player.maxHealth) {
            useHealth(-drainAmount);
            player.extraHealth -= drainAmount;
            if (player.extraHealth < 0) player.extraHealth = 0;
        } else {
            // Still decay over time if not healing
            player.extraHealth -= drainAmount;
            if (player.extraHealth < 0) player.extraHealth = 0;
        }
    }
}






