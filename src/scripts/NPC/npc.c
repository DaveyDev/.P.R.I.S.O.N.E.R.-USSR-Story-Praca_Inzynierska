#include "npc.h"
#include "../textures.h"
#include <stdio.h>

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

    npc.npcAnimation[0] = createSpriteAnimation(npc.texture, 3, (Rectangle[]){
        (Rectangle){0, 0, 32, 64}, 
        (Rectangle){32, 0, 32, 64},
        (Rectangle){64, 0, 32, 64},
        (Rectangle){96, 0, 32, 64},
        
    }, 4);


    return npc;
}

void updateNPC(NPC *npc, float deltaTime) {
    npc->frameCounter++;
    if (npc->frameCounter >= 10) {
        npc->frame = (npc->frame + 1) % 4;
        npc->frameCounter = 0;
    }

    // Behavior logic
    switch (npc->behavior) {
        case BEHAVIOR_PATROL: {
            npc->position.x += npc->direction * 30 * deltaTime;

            float left = npc->origin.x - 32;
            float right = npc->origin.x + 32;

            if (npc->position.x < left) npc->direction = 1;
            else if (npc->position.x > right) npc->direction = -1;

            npc->dir = (npc->direction < 0) ? LEFT : RIGHT;
        } break;

        case BEHAVIOR_IDLE:
        default:
            // Do nothing
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
    Vector2 origin = { 0, 128 }; // origin at feet

    drawSpriteAnimationPro(npc->npcAnimation[0], dest, origin, 0, WHITE);
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
