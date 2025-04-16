#include "npc.h"
#include "../textures.h"
#include <stdio.h>
#include "math.h"
#include "../map/map.h"
#include "pathfinding.h"
#include "../../../lib/raymath.h"
#include "../global.h"




NPC inmates[10];
int numInmates = 0;

NPC guards[5];
int numGuards = 0;



NPC initNPC(Texture2D texture, Vector2 position, NPCType type, NPCBehavior behavior) {
    NPC npc;
    npc.texture = texture;
    npc.position = position;
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



    npc.npcAnimation[0] = createSpriteAnimation(npc.texture, 3, (Rectangle[]){
        (Rectangle){0, 0, 32, 64}, 
        (Rectangle){32, 0, 32, 64},
        (Rectangle){64, 0, 32, 64},
        (Rectangle){96, 0, 32, 64},
        
    }, 4);


    return npc;
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



void updateNPC(NPC *npc, float deltaTime, Vector2 playerPos, int groupIndex, int groupSize) {
    npc->frameCounter++;
    if (npc->frameCounter >= 10) {
        npc->frame = (npc->frame + 1) % 4;
        npc->frameCounter = 0;
    }

    switch (npc->behavior) {
        case BEHAVIOR_PATROL: {
            npc->position.x += npc->direction * 30 * deltaTime;

            float left = npc->origin.x - 32;
            float right = npc->origin.x + 32;

            if (npc->position.x < left) npc->direction = 1;
            else if (npc->position.x > right) npc->direction = -1;

            npc->dir = (npc->direction < 0) ? LEFT : RIGHT;
        } break;
/*
        case BEHAVIOR_FOLLOW: {
    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;

    // Recalculate path every 1 second
    if (npc->pathUpdateTimer >= 1.0f) {
        npc->pathUpdateTimer = 0.0f;
        npc->pathLength = findPath(npc->position, playerPos, npc->path, MAX_NPC_PATH);
        npc->pathIndex = 0;
    }

    if (npc->pathIndex < npc->pathLength) {
        Vector2 nextStep = npc->path[npc->pathIndex];
        Vector2 toStep = { nextStep.x - npc->position.x, nextStep.y - npc->position.y };
        float dist = sqrtf(toStep.x * toStep.x + toStep.y * toStep.y);

        if (dist < 2.0f) {
            npc->pathIndex++;
        } else {
            toStep.x /= dist;
            toStep.y /= dist;
            npc->position.x += toStep.x * 40 * deltaTime;
            npc->position.y += toStep.y * 40 * deltaTime;
        }
    }
} break;
*/
case BEHAVIOR_FOLLOW: {

    //float stopRadius = 16.0f;
    //if (Vector2Distance(npc->position, playerPos) < stopRadius) {
        // Already close enough to the player — no need to repath
    //    return;
    //}

    npc->moveTimer += deltaTime;
    npc->pathUpdateTimer += deltaTime;
/*
    if (rows > 0 && cols > 0 && npc->pathUpdateTimer >= 1.0f) {
        npc->pathUpdateTimer = 0.0f;
        //npc->pathLength = findPath(npc->position, playerPos, npc->path, MAX_NPC_PATH, npc, inmates, numInmates);
        // Assign a personal circle target based on NPC index
        //Vector2 circleTarget = getCircleTarget(playerPos, npc - inmates, numInmates, 16.0f);
        Vector2 circleTarget = getCircleTarget(playerPos, groupIndex, groupSize, 16.0f);

        //npc->pathLength = findPath(npc->position, circleTarget, npc->path, MAX_NPC_PATH, npc, inmates, numInmates);
        NPC *group = (npc->type == NPC_GUARD) ? guards : inmates;
        int groupCount = (npc->type == NPC_GUARD) ? numGuards : numInmates;

        // You can use circleTarget OR playerPos here
        npc->pathLength = findPath(npc->position, playerPos, npc->path, MAX_NPC_PATH, npc, group, groupCount);




        npc->pathIndex = 0;
    }
    */
   if (rows > 0 && cols > 0 && npc->pathUpdateTimer >= 1.0f) {
    npc->pathUpdateTimer = 0.0f;
    
    // Pick the right group (guards or inmates)
    NPC *group = (npc->type == NPC_GUARD) ? guards : inmates;
    int groupCount = (npc->type == NPC_GUARD) ? numGuards : numInmates;

    // Restore original logic: go straight to player
    npc->pathLength = findPath(npc->position, playerPos, npc->path, MAX_NPC_PATH, npc, group, groupCount);
    npc->pathIndex = 0;
}

/*
    if (npc->pathIndex < npc->pathLength) {
    Vector2 target = npc->path[npc->pathIndex];
    Vector2 delta = Vector2Subtract(target, npc->position);
    float distance = Vector2Length(delta);

    Vector2 direction = Vector2Normalize(delta);
    float speed = 60.0f;
    float step = speed * deltaTime;

    if (distance <= step) {
        // Close enough: snap and advance
        npc->position = target;
        npc->pathIndex++;
    } else {
        // Move smoothly toward target
        npc->position = Vector2Add(npc->position, Vector2Scale(direction, step));
    }

    // Update animation direction
    if (fabs(direction.x) > fabs(direction.y)) {
        npc->dir = (direction.x < 0) ? LEFT : RIGHT;
    } else {
        npc->dir = (direction.y < 0) ? UP : DOWN;
    }
}
*/
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
    
    // 🟨 Apply avoidance
    //Vector2 avoid = avoidOtherNPCs(npc, inmates, numInmates);
    //npc->position = Vector2Add(npc->position, Vector2Scale(avoid, 0.5f));
    NPC *group = (npc->type == NPC_GUARD) ? guards : inmates;
    int groupCount = (npc->type == NPC_GUARD) ? numGuards : numInmates;

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
    //Vector2 avoid = avoidOtherNPCs(npc, inmates, numInmates);
    //npc->position = Vector2Add(npc->position, Vector2Scale(avoid, 0.3f)); // use softer force
    NPC *group = (npc->type == NPC_GUARD) ? guards : inmates;
    int groupCount = (npc->type == NPC_GUARD) ? numGuards : numInmates;

    Vector2 avoid = avoidOtherNPCs(npc, group, groupCount);
    npc->position = Vector2Add(npc->position, Vector2Scale(avoid, 0.3f));

}



} break;



        case BEHAVIOR_IDLE:
        default:
            break;
    }
}

/*
void DrawNPC(NPC *npc) {
    Rectangle frameRec = { npc->frame * 32, 0, 32, 32 };
    //DrawTextureRec(npc->texture, frameRec, npc->position, WHITE);
    Rectangle dest = {npc-> position.x, npc->position.y, 64, 128};
    Vector2 origin = {0};
    drawSpriteAnimationPro(npc-> npcAnimation[0], dest, origin, 0, WHITE);
}
*/

void drawNPC(NPC *npc, Camera2D camera) {
    Vector2 screenPos = GetWorldToScreen2D(npc->position, camera);

    Rectangle dest = { screenPos.x, screenPos.y, 64, 128 }; // scaled 2x from 32x64
    Vector2 origin = { 32 , 128 }; // origin at feet

    drawSpriteAnimationPro(npc->npcAnimation[0], dest, origin, 0, WHITE);

    BeginMode2D(camera);

    // Draw red dots for all path steps
    for (int i = 0; i < npc->pathLength; i++) {
        //DrawCircleV(npc->path[i], 2, RED);
    }

    // Draw red lines between path steps
    for (int i = 1; i < npc->pathLength; i++) {
        DrawLineV(npc->path[i - 1], npc->path[i], (Color){255, 0, 0, 128}); // semi-transparent red
    }

    // Draw current position and current target
    DrawCircleV(npc->position, 1, YELLOW);
    if (npc->pathIndex < npc->pathLength) {
        DrawCircleV(npc->path[npc->pathIndex], 2, RED);
    }

    DrawCircleLines(npc->position.x, npc->position.y, 20, (Color){255, 100, 100, 60});


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

