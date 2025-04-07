#include "npc.h"
#include "../textures.h"

NPC inmates[10];
int numInmates = 0;

NPC guards[5];
int numGuards = 0;

NPC InitNPC(Texture2D texture, Vector2 position, NPCType type, NPCBehavior behavior) {
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

void UpdateNPC(NPC *npc, float deltaTime) {
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

void DrawNPC(NPC *npc, Camera2D camera) {
    Vector2 screenPos = GetWorldToScreen2D(npc->position, camera);

    Rectangle dest = { screenPos.x, screenPos.y, 64, 128 }; // scaled 2x from 32x64
    Vector2 origin = { 0, 128 }; // origin at feet

    drawSpriteAnimationPro(npc->npcAnimation[0], dest, origin, 0, WHITE);
}


