#include "npc.h"
#include "../textures.h"
#include <stdio.h>
#include "math.h"
#include "../map/map.h"
#include "pathfinding.h"
#include "../../../lib/raymath.h"
#include "../global.h"
#include "../items/idList.h"
#include "../sound/soundManager.h"
#include "../player/sleep.h"
#include "../global.h"
#include "../items/items.h"
#include "../items/idList.h"
#include "../../../lib/raygui.h"
#include "trade.h"





NPC inmates[10];
int numInmates = 0;

NPC guards[5];
int numGuards = 0;

NPC *activeTradeNPC = NULL;
Rectangle tradeWindowBounds = { 300, 200, 400, 200 };
Rectangle acceptButton = { 350, 330, 80, 30 };
Rectangle closeButton = { 470, 330, 80, 30 };

int activeTradeNPCIndex;


bool spawnReserved[MAX_ROWS][MAX_COLS] = {0};









NPC initNPC(Texture2D texture, Vector2 position, NPCType type, NPCBehavior behavior) {
    NPC npc;
    npc.texture = texture;
    npc.position = position;
    npc.prevsPosition = position;
    npc.origin = position;
    npc.frame = 0;
    npc.frameCounter = 0;
    npc.direction = 1;
    npc.dir = DOWN;
    npc.type = type;
    npc.behavior = behavior;

    for (int i = 0; i < MAX_NPC_PATH; i++) {
        npc.path[i] = (Vector2){0};
    }

    npc.pathLength = 0;
    npc.pathIndex = 0;

    npc.moveTimer = 0.0f;
    //npc.pathUpdateTimer = 0.0f;
    npc.pathUpdateTimer = GetRandomValue(0, 1000) / 1000.0f;  // random delay 0–1s
    npc.attackCooldown = 0.0f;

    npc.health = 35;

    npc.currentPatrolTarget = (Vector2){0,0};
    npc.hasPatrolTarget = false;
    npc.gotFood = false;
    npc.queueIndex = 0;
    npc.queueTargetBlock = 0;
    npc.resourceTile = (Vector2){0,0};
    npc.animationNumber = 0;

    npc.requestedItemId = 3006;
    npc.rewardItemId = 3007;
    npc.rewardItemName[0] = '\0';
    npc.tradeCompleted = false;
    npc.isTalking = false;
/*
    npc.npcAnimation[0] = createSpriteAnimation(npc.texture, 3, (Rectangle[]){
        (Rectangle){0, 0, 32, 64}, 
        (Rectangle){32, 0, 32, 64},
        (Rectangle){64, 0, 32, 64},
        (Rectangle){96, 0, 32, 64},
        
    }, 4);
    npc.npcAnimation[1] = createSpriteAnimation(npc.texture, 3, (Rectangle[]){
        (Rectangle){0, 64, 32, 64}, 
        (Rectangle){32, 64, 32, 64},
        (Rectangle){64, 64, 32, 64},
        (Rectangle){96, 64, 32, 64},
        
    }, 4);
    npc.npcAnimation[2] = createSpriteAnimation(npc.texture, 3, (Rectangle[]){
        (Rectangle){0, 128, 32, 64}, 
        (Rectangle){32, 128, 32, 64},
        (Rectangle){64, 128, 32, 64},
        (Rectangle){96, 128, 32, 64},
        
    }, 4);
    npc.npcAnimation[3] = createSpriteAnimation(npc.texture, 3, (Rectangle[]){
        (Rectangle){0, 192, 32, 64}, 
        (Rectangle){32, 192, 32, 64},
        (Rectangle){64, 192, 32, 64},
        (Rectangle){96, 192, 32, 64},
        
    }, 4);
    npc.npcAnimation[4] = createSpriteAnimation(npc.texture, 3, (Rectangle[]){
        (Rectangle){0, 256, 32, 64}, 
        (Rectangle){32, 256, 32, 64},
        (Rectangle){64, 256, 32, 64},
        (Rectangle){96, 256, 32, 64},
        
    }, 4);

    */

    return npc;
}

void InitNPCAnimations(NPC *npc, Texture2D texture, int rowOffset) {
    npc->texture = texture;
    for (int anim = 0; anim < ANIMATIONS_PER_NPC; anim++) {
        Rectangle frames[FRAMES_PER_ANIMATION];
        for (int f = 0; f < FRAMES_PER_ANIMATION; f++) {
            frames[f] = (Rectangle){ f * 32, (rowOffset + anim) * 64, 32, 64 };
        }
        npc->npcAnimation[anim] = createSpriteAnimation(texture, 3, frames, FRAMES_PER_ANIMATION);
    }
}

Vector2 avoidOtherNPCs(NPC *self, NPC *all, int count) {
    Vector2 push = {0};

    for (int i = 0; i < count; i++) {
        NPC *other = &all[i];
        if (other == self) continue;

        float dist = Vector2Distance(self->position, other->position);
        float minDist = 20.0f;

        if (dist < minDist && dist > 0.01f) {
            Vector2 away = Vector2Subtract(self->position, other->position);
            away = Vector2Normalize(away);

            float strength = (minDist - dist) / minDist;  // falloff 0.0 to 1.0
            away = Vector2Scale(away, strength * 8.0f);   // max 8px push at closest

            push = Vector2Add(push, away);

        }
    }

    return push;
}

Vector2 getCircleTarget(Vector2 center, int index, int total, float radius) {
    float angle = ((float)index / total) * 2 * PI;
    return (Vector2){
        center.x + cosf(angle) * radius,
        center.y + sinf(angle) * radius
    };
}

Vector2 findNearestPatrolPoint(Vector2 from) {
    Vector2 closest = from;
    float bestDist = 999999.0f;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (objects[row][col] == PATROL_BLOCK) {
                printf("Found patrol at row: %d col: %d\n", row, col);

                Vector2 pos = { col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2 };
                float dist = Vector2Distance(from, pos);
                if (dist < bestDist) {
                    bestDist = dist;
                    closest = pos;
                }
            }
        }
    }

    return closest;
}

Vector2 pickNextPatrolPoint(NPC *npc, int npcIndex) {
    

    if (patrolPointCount == 0) return npc->position; // no patrol points

    int index = npcPatrolIndex[npcIndex];
    index = (index + 1) % patrolPointCount;
    npcPatrolIndex[npcIndex] = index;

    return patrolPoints[index];
}






void updateNPC(NPC *npc, float deltaTime, Vector2 playerPos, int groupIndex, int groupSize) {
    npc->frameCounter++;
    if (npc->frameCounter >= 10) {
        npc->frame = (npc->frame + 1) % 4;
        npc->frameCounter = 0;
    }

    NPC *group = (npc->type == NPC_GUARD) ? guards : inmates;
    int groupCount = (npc->type == NPC_GUARD) ? numGuards : numInmates;

    switch (npc->behavior) {
        

    

case BEHAVIOR_FOLLOW: {

    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;

    float attackRange = 20.0f;
    float attackRate = 1.0f;  // seconds between attacks

    // Check distance to player
    float toPlayer = Vector2Distance(npc->position, playerPos);

    if (toPlayer < attackRange) {

    // If player is already KO'd or sleeping, stop attacking
    if (player.wasKnockedOutToday || isPlayerSleeping()) {
        npc->behavior = BEHAVIOR_PATROL;
        npc->pathLength = 0;
        npc->pathIndex = 0;
        return;
    }

    npc->attackCooldown -= deltaTime;
    if (npc->attackCooldown <= 0.0f) {
        npc->attackCooldown = attackRate;
        playAttackSound();

        if (player.health <= 5) {
            player.health = 1;
            player.wasKnockedOutToday = true;
            triggerForcedSleep();
            return;
        }

        useHealth(5.0f);
        printf("Guard attacked! Player HP: %f\n", player.health);
    }

    return;
}


   if (rows > 0 && cols > 0 && npc->pathUpdateTimer >= 1.0f) {
    npc->pathUpdateTimer = 0.0f;
    
    // Pick the right group (guards or inmates)
    NPC *group = (npc->type == NPC_GUARD) ? guards : inmates;
    int groupCount = (npc->type == NPC_GUARD) ? numGuards : numInmates;

    // Restore original logic: go straight to player
    npc->pathLength = findPath(npc->position, playerPos, npc->path, MAX_NPC_PATH, npc, group, groupCount);
    npc->pathIndex = 0;
}


// Lookahead target: peek 2–3 steps ahead, not just the next one
int lookahead = npc->pathIndex + 2;
if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;




if (npc->pathIndex < npc->pathLength) {
    Vector2 target = npc->path[lookahead];
    Vector2 delta = Vector2Subtract(target, npc->position);
    float distance = Vector2Length(delta);

    Vector2 direction = Vector2Normalize(delta);
    float speed = 60.0f;
    float step = speed * deltaTime;

    if (distance <= step) {
        npc->position = target;
        npc->pathIndex = lookahead + 1; // jump ahead!
    } else {
        npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
    }
    
    

    Vector2 avoid = avoidOtherNPCs(npc, group, groupCount);
    npc->position = Vector2Add(npc->position, Vector2Scale(avoid, 0.5f)); // or 0.3f if idle


    // Direction for animation
    if (fabs(direction.x) > fabs(direction.y)) {
        npc->dir = (direction.x < 0) ? LEFT : RIGHT;
    } else {
        npc->dir = (direction.y < 0) ? UP : DOWN;
    }
} else {
    // Path is finished — still push away from others to avoid standing on top
    

    Vector2 avoid = avoidOtherNPCs(npc, group, groupCount);
    npc->position = Vector2Add(npc->position, Vector2Scale(avoid, 0.3f));

}



} break;


case BEHAVIOR_PATROL: {
    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;

    if (rows > 0 && cols > 0 && npc->pathUpdateTimer >= 1.0f) {
        npc->pathUpdateTimer = 0.0f;

        // If no target yet or reached current one, pick new one
        if (!npc->hasPatrolTarget || Vector2Distance(npc->position, npc->currentPatrolTarget) < 4.0f) {
            npc->currentPatrolTarget = pickNextPatrolPoint(npc, groupIndex);
            npc->hasPatrolTarget = true;
        }

        npc->pathLength = findPath(npc->position, npc->currentPatrolTarget, npc->path, MAX_NPC_PATH, npc, group, groupCount);
        npc->pathIndex = 0;
    }

    int lookahead = npc->pathIndex + 2;
    if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;

    if (npc->pathIndex < npc->pathLength) {
        Vector2 target = npc->path[lookahead];
        Vector2 delta = Vector2Subtract(target, npc->position);
        float distance = Vector2Length(delta);

        Vector2 direction = Vector2Normalize(delta);
        float speed = 40.0f;
        float step = speed * deltaTime;

        if (distance <= step) {
            npc->position = target;
            npc->pathIndex = lookahead + 1;
        } else {
            npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
        }

        Vector2 avoid = avoidOtherNPCs(npc, group, groupCount);
        npc->position = Vector2Add(npc->position, Vector2Scale(avoid, 0.5f));

        npc->dir = (fabs(direction.x) > fabs(direction.y)) ? (direction.x < 0 ? LEFT : RIGHT) :
                                                            (direction.y < 0 ? UP : DOWN);
    }
} break;

case BEHAVIOR_SLEEP: {

    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;

    
   if (!npc->hasPatrolTarget) {
    int bestRow = -1, bestCol = -1;
    float bestDist = 999999.0f;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int obj = objects[row][col];
            int det = details[row][col];

            bool correctType = (npc->type == NPC_GUARD)
                ? (obj == GUARD_SPAWN || det == GUARD_SPAWN)
                : (obj == INMATE_SPAWN || det == INMATE_SPAWN);
            if (!correctType) continue;

            if (spawnReserved[row][col]) continue; // Already taken

            // Optional: distance-based occupancy check (if needed)
            bool occupied = false;
            for (int i = 0; i < groupCount; i++) {
                if (&group[i] == npc) continue;
                float dist = Vector2Distance(group[i].position, (Vector2){col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2});
                if (dist < 20.0f) {
                    occupied = true;
                    break;
                }
            }
            if (occupied) continue;

            Vector2 tileCenter = {col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2};
            float dist = Vector2Distance(npc->position, tileCenter);

            if (dist < bestDist) {
                bestDist = dist;
                bestRow = row;
                bestCol = col;
            }
        }
    }

    if (bestRow != -1 && bestCol != -1) {
        spawnReserved[bestRow][bestCol] = true;  // Reserve it now

        npc->currentPatrolTarget = (Vector2){bestCol * TILE_SIZE + TILE_SIZE / 2, bestRow * TILE_SIZE + TILE_SIZE / 2};
        npc->hasPatrolTarget = true;

        npc->pathLength = findPath(npc->position, npc->currentPatrolTarget, npc->path, MAX_NPC_PATH, npc, group, groupCount);
        npc->pathIndex = 0;
    }
}


    if (npc->hasPatrolTarget) {
        int lookahead = npc->pathIndex + 2;
        if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;

        if (npc->pathIndex < npc->pathLength) {
            Vector2 target = npc->path[lookahead];
            Vector2 delta = Vector2Subtract(target, npc->position);
            float distance = Vector2Length(delta);

            Vector2 direction = Vector2Normalize(delta);
            float speed = 30.0f;
            float step = speed * deltaTime;

            if (distance <= step) {
                npc->position = target;
                npc->pathIndex = lookahead + 1;
            } else {
                npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
            }

            npc->dir = (fabs(direction.x) > fabs(direction.y)) ? (direction.x < 0 ? LEFT : RIGHT) :
                                                                (direction.y < 0 ? UP : DOWN);

            

        } else {
            // Once reached, stay there
            // (do nothing, no reset of hasPatrolTarget)
        }

        
    }
    
    //printf("NPC %d going to sleep\n", groupIndex);
    //printf("Target: (%.1f, %.1f)\n", npc->currentPatrolTarget.x, npc->currentPatrolTarget.y);
    //printf("Path steps: %d\n", npc->pathLength);

} break;


case BEHAVIOR_LUNCH: {
    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;

    // STEP 1: Assign queue target if not already
    if (!npc->hasPatrolTarget && !npc->gotFood) {
        int bestIndex = -1;
        float bestDist = 999999.0f;

        for (int i = 0; i < foodBlockCount; i++) {
            float dist = Vector2Distance(npc->position, foodTakeBlocks[i]);
            if (dist < bestDist) {
                bestDist = dist;
                bestIndex = i;
            }
        }

        if (bestIndex != -1) {
            //int queuePos = foodQueueLengths[bestIndex];
            int queuePos = foodQueueLengths[bestIndex];
            npc->queueIndex = queuePos;
            npc->queueTargetBlock = bestIndex;


            Vector2 dir = (Vector2){0, 1}; // queue vertically down
            Vector2 offset = Vector2Scale(dir, queuePos * TILE_SIZE);

            npc->currentPatrolTarget = Vector2Add(foodTakeBlocks[bestIndex], offset);
            npc->hasPatrolTarget = true;

            npc->pathLength = findPath(npc->position, npc->currentPatrolTarget, npc->path, MAX_NPC_PATH, npc, group, groupCount);
            npc->pathIndex = 0;

            //foodQueueLengths[bestIndex]++;
            npc->queueIndex = foodQueueLengths[bestIndex];
            npc->queueTargetBlock = bestIndex;

            foodQueues[bestIndex][npc->queueIndex] = npc;
            foodQueueLengths[bestIndex]++;


            printf("NPC %d assigned to food block %d at queue position %d\n", groupIndex, bestIndex, queuePos);
        }
    }

    // STEP 2: Move along path (if assigned)
    if (npc->pathIndex < npc->pathLength) {
        int lookahead = npc->pathIndex + 2;
        if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;

        Vector2 target = npc->path[lookahead];
        Vector2 move = Vector2Subtract(target, npc->position);
        float moveDist = Vector2Length(move);
        Vector2 direction = Vector2Normalize(move);

        float speed = 40.0f;
        float step = speed * deltaTime;

        if (moveDist <= step) {
            npc->position = target;
            npc->pathIndex = lookahead + 1;
        } else {
            npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
        }

        npc->dir = (fabs(direction.x) > fabs(direction.y)) ? (direction.x < 0 ? LEFT : RIGHT) :
                                                            (direction.y < 0 ? UP : DOWN);
    }

    // STEP 3: If close enough to queue position, get food and walk away
    float distToTarget = Vector2Distance(npc->position, npc->currentPatrolTarget);
    if (!npc->gotFood && npc->queueIndex == 0 && distToTarget < 12.0f) {
        printf("NPC %d got food at distance %.2f\n", groupIndex, distToTarget);
        npc->gotFood = true;
        npc->hasPatrolTarget = false;

        // Shift queue forward
int block = npc->queueTargetBlock;
for (int i = 1; i < foodQueueLengths[block]; i++) {
    foodQueues[block][i - 1] = foodQueues[block][i];
    foodQueues[block][i - 1]->queueIndex--;

    // Reassign their target (move one tile forward)
    Vector2 dir = (Vector2){0, 1};
    Vector2 offset = Vector2Scale(dir, foodQueues[block][i - 1]->queueIndex * TILE_SIZE);
    Vector2 newTarget = Vector2Add(foodTakeBlocks[block], offset);

    foodQueues[block][i - 1]->currentPatrolTarget = newTarget;
    foodQueues[block][i - 1]->hasPatrolTarget = true;
    foodQueues[block][i - 1]->pathLength = findPath(
        foodQueues[block][i - 1]->position,
        newTarget,
        foodQueues[block][i - 1]->path,
        MAX_NPC_PATH,
        foodQueues[block][i - 1],
        group,
        groupCount
    );
    foodQueues[block][i - 1]->pathIndex = 0;
}

// Clear last slot
foodQueues[block][foodQueueLengths[block] - 1] = NULL;
foodQueueLengths[block]--;


        float randomX = npc->position.x + GetRandomValue(-100, 100);
        float randomY = npc->position.y + GetRandomValue(50, 150); // walk away down
        npc->currentPatrolTarget = (Vector2){randomX, randomY};

        npc->pathLength = findPath(npc->position, npc->currentPatrolTarget, npc->path, MAX_NPC_PATH, npc, group, groupCount);
        npc->pathIndex = 0;
    }
} break;

case BEHAVIOR_LUNCH_GUARD: {
    npc->moveTimer += deltaTime;

    // Step 1: Walk to the nearest FOOD_WORK_BLOCK once
    if (!npc->hasPatrolTarget) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (objects[row][col] == FOOD_WORK_BLOCK || details[row][col] == FOOD_WORK_BLOCK) {
                    npc->currentPatrolTarget = (Vector2){col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2};
                    npc->hasPatrolTarget = true;
                    npc->pathLength = findPath(npc->position, npc->currentPatrolTarget, npc->path, MAX_NPC_PATH, npc, guards, numGuards);
                    npc->pathIndex = 0;
                    break;
                }
            }
            if (npc->hasPatrolTarget) break;
        }
    }

    // Step 2: Move to the work block
    if (npc->hasPatrolTarget && npc->pathIndex < npc->pathLength) {
        int lookahead = npc->pathIndex + 2;
        if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;

        Vector2 target = npc->path[lookahead];
        Vector2 move = Vector2Subtract(target, npc->position);
        float moveDist = Vector2Length(move);
        Vector2 direction = Vector2Normalize(move);

        float speed = 40.0f;
        float step = speed * deltaTime;

        if (moveDist <= step) {
            npc->position = target;
            npc->pathIndex = lookahead + 1;
        } else {
            npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
        }

        npc->dir = (fabs(direction.x) > fabs(direction.y)) ? (direction.x < 0 ? LEFT : RIGHT) :
                                                            (direction.y < 0 ? UP : DOWN);
        return;
    }

    // Step 3: Once standing there, spawn item every 5 seconds
    static float spawnTimer = 0.0f;
    spawnTimer += deltaTime;

    if (spawnTimer >= 5.0f && !wasFoodGiven) {
        spawnTimer = 0.0f;
        wasFoodGiven = true;

        // Step 4: Find a table nearby
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                int tileObj = objects[row][col];
                int tileDet = details[row][col];
                if (
                    tileObj == SMALL_TABLE || tileObj == LEFT_TABLE || tileObj == MIDDLE_TABLE || tileObj == RIGHT_TABLE ||
                    tileDet == SMALL_TABLE || tileDet == LEFT_TABLE || tileDet == MIDDLE_TABLE || tileDet == RIGHT_TABLE
                ) {
                    Vector2 tableCenter = {col * TILE_SIZE + TILE_SIZE / 4, row * TILE_SIZE + TILE_SIZE / 4};
                    float dist = Vector2Distance(npc->position, tableCenter);
                    if (dist < 120.0f) { // within ~4 tiles
                        //int itemID = GetRandomValue(3000, 3004); // Random food
                        int itemID = FOOD_ITEM_IDS[GetRandomValue(0, NUM_FOOD_ITEMS - 1)];



                        addItem(tableCenter, itemID, 1, "Food");
                        printf("Guard placed item %d on table at %.1f,%.1f\n", itemID, tableCenter.x, tableCenter.y);
                        return;
                    }
                }
            }
        }
       
    }
} break;

case BEHAVIOR_FREE_TIME: {
    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;

    // Pick a random free-time block if idle
    if (!npc->hasPatrolTarget && freeTimeBlockCount > 0) {
        int targetIndex = GetRandomValue(0, freeTimeBlockCount - 1);
        npc->currentPatrolTarget = freeTimeBlocks[targetIndex];
        npc->hasPatrolTarget = true;

        npc->pathLength = findPath(
            npc->position,
            npc->currentPatrolTarget,
            npc->path,
            MAX_NPC_PATH,
            npc,
            group,
            groupCount
        );
        npc->pathIndex = 0;
    }

    // Movement toward the free-time tile
    if (npc->pathIndex < npc->pathLength) {
        int lookahead = npc->pathIndex + 2;
        if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;

        Vector2 target = npc->path[lookahead];
        Vector2 delta = Vector2Subtract(target, npc->position);
        float dist = Vector2Length(delta);
        Vector2 dir = Vector2Normalize(delta);

        float speed = 40.0f;
        float step = speed * deltaTime;

        if (dist <= step) {
            npc->position = target;
            npc->pathIndex = lookahead + 1;
        } else {
            npc->position = Vector2Add(npc->position, Vector2Scale(dir, step));
        }

        npc->dir = (fabs(dir.x) > fabs(dir.y)) ? (dir.x < 0 ? LEFT : RIGHT) :
                                                  (dir.y < 0 ? UP : DOWN);
    }

    // Reached destination — idle or wander again after delay
    if (Vector2Distance(npc->position, npc->currentPatrolTarget) < 8.0f && npc->hasPatrolTarget) {
        static float idleTime = 0.0f;
        idleTime += deltaTime;

        if (idleTime > 4.0f) {
            npc->hasPatrolTarget = false;
            idleTime = 0.0f;
        }
    }
} break;

case BEHAVIOR_WORK: {
    npc->moveTimer += deltaTime;

    // Step 1: Assign target if needed
    if (!npc->hasPatrolTarget) {
        Vector2 *targets = NULL;
        int count = 0;

        if (npc->job == JOB_WOOD && treeBlockCount > 0) {
            targets = treeBlocks;
            count = treeBlockCount;
        } else if (npc->job == JOB_ROCK && rockBlockCount > 0) {
            targets = rockBlocks;
            count = rockBlockCount;
        } else {
            // No resources available — skip behavior
            return;
        }

        int bestIndex = -1;
        float bestDist = 999999.0f;

        for (int i = 0; i < count; i++) {
            float dist = Vector2Distance(npc->position, targets[i]);
            if (dist < bestDist) {
                bestDist = dist;
                bestIndex = i;
            }
        }

        if (count > 0) {
    int tries = 0;
    const int maxTries = count;

    while (tries < maxTries) {
        int index = GetRandomValue(0, count - 1);
        Vector2 resource = targets[index];

        // Optional: Offset one tile down to avoid blocked center
        Vector2 walkTarget = Vector2Add(resource, (Vector2){0, TILE_SIZE});

        npc->resourceTile = resource; // remember what we’re going to modify later



        int pathLen = findPath(npc->position, walkTarget, npc->path, MAX_NPC_PATH, npc, group, groupCount);

        if (pathLen > 0) {
            npc->currentPatrolTarget = walkTarget;
            npc->hasPatrolTarget = true;
            npc->pathLength = pathLen;
            npc->pathIndex = 0;

            printf("NPC %d assigned to %s job, path length: %d → target (%.1f, %.1f)\n",
                   groupIndex,
                   npc->job == JOB_WOOD ? "wood" : "rock",
                   pathLen,
                   walkTarget.x,
                   walkTarget.y);
            break;
        }

        tries++;
    }

    if (tries == maxTries) {
        printf("NPC %d could not find a reachable %s target.\n",
               groupIndex,
               npc->job == JOB_WOOD ? "tree" : "rock");
    }
}

    }

    // Step 2: Move to target
    if (npc->pathIndex < npc->pathLength) {
        int lookahead = npc->pathIndex + 2;
        if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;

        Vector2 target = npc->path[lookahead];
        Vector2 move = Vector2Subtract(target, npc->position);
        float moveDist = Vector2Length(move);
        Vector2 direction = Vector2Normalize(move);

        float speed = 40.0f;
        float step = speed * deltaTime;

        if (moveDist <= step) {
            npc->position = target;
            npc->pathIndex = lookahead + 1;
        } else {
            npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
        }

        npc->dir = (fabs(direction.x) > fabs(direction.y)) ? (direction.x < 0 ? LEFT : RIGHT) :
                                                            (direction.y < 0 ? UP : DOWN);
    }

    // Step 3: Work (stand still) when reached
    if (npc->hasPatrolTarget && Vector2Distance(npc->position, npc->currentPatrolTarget) < 8.0f) {
        npc->workTimer += deltaTime;
        /*if (npc->workTimer > 3.0f) {
            printf("NPC %d finished work at (%.1f, %.1f)\n", groupIndex, npc->position.x, npc->position.y);
            npc->hasPatrolTarget = false;
            npc->workTimer = 0.0f;
        }*/
       if (npc->workTimer > 3.0f) {
        printf("NPC %d finished work at (%.1f, %.1f)\n", groupIndex, npc->position.x, npc->position.y);
    // Determine tile coordinates
    int col = npc->resourceTile.x / TILE_SIZE;
    int row = npc->resourceTile.y / TILE_SIZE;

    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        if (npc->job == JOB_WOOD) {
            objects[row][col] = TREE_STUMP;
            //addItem(npc->position, WOOD_LOG, 1, "Wood Log");
            printf("NPC %d chopped tree at (%d, %d)\n", groupIndex, col, row);
        } else if (npc->job == JOB_ROCK) {
            objects[row][col] = STONE;
            //addItem(npc->position, STONE_ITEM, 1, "Stone");
            printf("NPC %d mined rock at (%d, %d)\n", groupIndex, col, row);
        }
    }

    npc->hasPatrolTarget = false;
    npc->workTimer = 0.0f;
}

    }
} break;

case BEHAVIOR_ROLLCALL: {
    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;

    if (!npc->hasPatrolTarget) {
        int bestRow = -1, bestCol = -1;
        float bestDist = 999999.0f;

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                int obj = objects[row][col];
                int det = details[row][col];

                bool correctType = (npc->type == NPC_GUARD)
                    ? (obj == ROLLCALL_GUARD_BLOCK || det == ROLLCALL_GUARD_BLOCK)
                    : (obj == ROLLCALL_BLOCK || det == ROLLCALL_BLOCK);
                if (!correctType) continue;

                if (spawnReserved[row][col]) continue;

                bool occupied = false;
                for (int i = 0; i < groupCount; i++) {
                    if (&group[i] == npc) continue;
                    float dist = Vector2Distance(group[i].position, (Vector2){col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2});
                    if (dist < 20.0f) {
                        occupied = true;
                        break;
                    }
                }
                if (occupied) continue;

                Vector2 tileCenter = {col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2};
                float dist = Vector2Distance(npc->position, tileCenter);

                if (dist < bestDist) {
                    bestDist = dist;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }

        if (bestRow != -1 && bestCol != -1) {
            spawnReserved[bestRow][bestCol] = true;

            npc->currentPatrolTarget = (Vector2){bestCol * TILE_SIZE + TILE_SIZE / 2, bestRow * TILE_SIZE + TILE_SIZE / 2};
            npc->hasPatrolTarget = true;

            npc->pathLength = findPath(npc->position, npc->currentPatrolTarget, npc->path, MAX_NPC_PATH, npc, group, groupCount);
            npc->pathIndex = 0;
        }
    }

    if (npc->hasPatrolTarget) {
        int lookahead = npc->pathIndex + 2;
        if (lookahead >= npc->pathLength) lookahead = npc->pathLength - 1;

        if (npc->pathIndex < npc->pathLength) {
            Vector2 target = npc->path[lookahead];
            Vector2 delta = Vector2Subtract(target, npc->position);
            float distance = Vector2Length(delta);

            Vector2 direction = Vector2Normalize(delta);
            float speed = 30.0f;
            float step = speed * deltaTime;

            if (distance <= step) {
                npc->position = target;
                npc->pathIndex = lookahead + 1;
            } else {
                npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
            }

            npc->dir = (fabs(direction.x) > fabs(direction.y)) ? (direction.x < 0 ? LEFT : RIGHT) :
                                                                    (direction.y < 0 ? UP : DOWN);
        }
        // Once reached, stand in place
    }
} break;



case BEHAVIOR_TALKING:
    // Freeze NPC
    npc->moveTimer = 0;
    npc->pathUpdateTimer = 0;
    // Maybe play idle anim or turn toward player
    

    break;








        case BEHAVIOR_IDLE:
        default:
            break;
    }

Vector2 delta = {
    npc->position.x - npc->prevsPosition.x,
    npc->position.y - npc->prevsPosition.y,
};

if (delta.x == 0 && delta.y == 0) {
    npc->animationNumber = 0; // standing
} else {
    if (delta.x < 0) {
        npc->animationNumber = 4; // walking left
    } else if (delta.x > 0) {
        npc->animationNumber = 3; // walking right
    } else if (delta.y < 0) {
        npc->animationNumber = 2; // walking up
    } else if (delta.y > 0) {
        npc->animationNumber = 1; // walking down
    }
}

npc->prevsPosition = npc->position;



}



void drawNPC(NPC *npc, Camera2D camera) {
    Vector2 screenPos = GetWorldToScreen2D(npc->position, camera);

    Rectangle dest = { screenPos.x, screenPos.y, 64, 128 }; // scaled 2x from 32x64
    Vector2 origin = { 32 , 128 }; // origin at feet
 
    drawSpriteAnimationPro(npc->npcAnimation[npc->animationNumber], dest, origin, 0, WHITE);

    BeginMode2D(camera);

    // Draw red dots for all path steps
    for (int i = 0; i < npc->pathLength; i++) {
        //DrawCircleV(npc->path[i], 2, RED);
    }

    // Draw red lines between path steps
    for (int i = 1; i < npc->pathLength; i++) {
        //DrawLineV(npc->path[i - 1], npc->path[i], (Color){255, 0, 0, 128}); // semi-transparent red
    }

    // Draw current position and current target
    //DrawCircleV(npc->position, 1, YELLOW);
    if (npc->pathIndex < npc->pathLength) {
       // DrawCircleV(npc->path[npc->pathIndex], 2, RED);
    }

    //DrawCircleLines(npc->position.x, npc->position.y, 20, (Color){255, 100, 100, 60});


    EndMode2D();


}



void saveNPCsToFile(const char *filename, NPC *npcArray, int count) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error saving NPCs to %s\n", filename);
        return;
    }

    for (int i = 0; i < count; i++) {
        NPC *npc = &npcArray[i];
        fprintf(file, "%.2f,%.2f:%.2f,%.2f:%d:%d:%d:%d\n",
            npc->position.x, npc->position.y,
            npc->origin.x, npc->origin.y,
            npc->type,
            npc->behavior,
            npc->direction,
            npc->dir
        );
    }

    fclose(file);
}

int loadNPCsFromFile(const char *filename, NPC *npcArray, int maxCount, Texture2D texture) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error loading NPCs from %s\n", filename);
        return 0;
    }

    int count = 0;
    while (!feof(file) && count < maxCount) {
        float posX, posY, originX, originY;
        int type, behavior, direction, dir;

        int matched = fscanf(file, "%f,%f:%f,%f:%d:%d:%d:%d\n",
                             &posX, &posY,
                             &originX, &originY,
                             &type, &behavior, &direction, &dir);

        if (matched == 8) {
            NPC npc = initNPC(texture, (Vector2){posX, posY}, (NPCType)type, (NPCBehavior)behavior);
            npc.origin = (Vector2){originX, originY};
            npc.direction = direction;
            npc.dir = (Direction)dir;
            npcArray[count++] = npc;
        }
    }

    fclose(file);
    return count;
}

bool isTileTemporarilyBlocked(int row, int col, NPC *self, NPC *all, int count) {
    Vector2 tileCenter = { col * TILE_SIZE + TILE_SIZE / 2, row * TILE_SIZE + TILE_SIZE / 2 };

    for (int i = 0; i < count; i++) {
        if (&all[i] == self) continue;

        Vector2 otherPos = all[i].position;
        float dist = Vector2Distance(tileCenter, otherPos);

        if (dist < 20) return true; // too close to other NPC
    }

    return false;
}

bool TryBarterWithNPC(NPC *npc) {
    if (npc->tradeCompleted) return false;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i].id == npc->requestedItemId && inventory[i].quantity > 0) {
            // Remove 1 of the requested item
            inventory[i].quantity--;

            // Add the reward item
            addItemToInventory(npc->rewardItemId, npc->rewardItemName);

            npc->tradeCompleted = 1;
            return true;
        }
    }

    return false;
}

void handleNPCClick(int i) {
    
    Rectangle npcRect = { inmates[i].position.x, inmates[i].position.y, TILE_SIZE, TILE_SIZE };

        
    activeTradeNPC = &inmates[i];
    activeTradeNPCIndex = i;
    printf("trading with NPC no: %i \n", i);
            
        
    
}

void drawItemFromId(int id, Vector2 pos) {
    if (id == 0) return;

    int itemsWidth = 32;
    int itemsHeight = 32;
    int itemsPerRow = itemsSet.width / itemsWidth;
    int itemIndex = id - 3000;

    int row = itemIndex / itemsPerRow;
    int col = itemIndex % itemsPerRow;

    Rectangle itemSource = { col * itemsWidth, row * itemsHeight, itemsWidth, itemsHeight };
    Rectangle itemDest = { pos.x, pos.y, itemsWidth, itemsHeight };

    DrawTexturePro(itemsSet, itemSource, itemDest, (Vector2){0, 0}, 0.0f, WHITE);
}

void DrawBarterUI() {
    if (!activeTradeNPC) return;

    DrawRectangleRec(tradeWindowBounds, DARKGRAY);

    // Centered title
    const char *title = "Trade Offer";
    int titleWidth = MeasureText(title, 20);
    DrawText(title, tradeWindowBounds.x + (tradeWindowBounds.width - titleWidth) / 2, tradeWindowBounds.y + 10, 20, WHITE);

    // Wants section
    DrawText("Wants:", tradeWindowBounds.x + 20, tradeWindowBounds.y + 50, 18, RAYWHITE);
    Vector2 wantsIconPos = { tradeWindowBounds.x + 100, tradeWindowBounds.y + 50 };
    drawItemFromId(inmates[activeTradeNPCIndex].requestedItemId, wantsIconPos);

    // Gives section
    DrawText("Gives:", tradeWindowBounds.x + 20, tradeWindowBounds.y + 90, 18, RAYWHITE);
    Vector2 givesIconPos = { tradeWindowBounds.x + 100, tradeWindowBounds.y + 90 };
    drawItemFromId(inmates[activeTradeNPCIndex].rewardItemId, givesIconPos);

    // Buttons
    if (GuiButton(acceptButton, "Accept")) {
    int wanted = inmates[activeTradeNPCIndex].requestedItemId;
    int reward = inmates[activeTradeNPCIndex].rewardItemId;

    if (hasItemInInventory(wanted)) {
    removeItemFromInventory(wanted);
    addItemToInventory(reward, "unknown"); // Assuming you have this
}

}


    if (GuiButton(closeButton, "Close")) {

        if(inmates[activeTradeNPCIndex].isTalking){
            inmates[activeTradeNPCIndex].behavior = inmates[activeTradeNPCIndex].lastBehavior;
            inmates[activeTradeNPCIndex].isTalking = false;
            printf("Ended trading with NPC no: %i\n", activeTradeNPCIndex);
        }
        activeTradeNPC = NULL;
        activeTradeNPCIndex = -1;
        
        
        
    }
}


void HandleBarterUIClick(Vector2 mousePos) {
    if (!activeTradeNPC) return;

    if (CheckCollisionPointRec(mousePos, acceptButton)) {
        if (TryBarterWithNPC(activeTradeNPC)) {
            printf("Trade successful!\n");
        } else {
            printf("Trade failed.\n");
        }
        activeTradeNPC = NULL;
    }

    if (CheckCollisionPointRec(mousePos, closeButton)) {
        activeTradeNPC = NULL;
    }
}

void assignRandomTradeToNPC(NPC *npc) {
    if (tradeListSize == 0) return;
    int randomIndex = GetRandomValue(0, tradeListSize - 1);
    npc->requestedItemId = tradeList[randomIndex].wantedItemID;
    npc->rewardItemId = tradeList[randomIndex].rewardItemID;
}

void assignTradesToAllNPCs(NPC *npcs, int npcCount) {
    for (int i = 0; i < npcCount; i++) {
        assignRandomTradeToNPC(&npcs[i]);
    }
}

