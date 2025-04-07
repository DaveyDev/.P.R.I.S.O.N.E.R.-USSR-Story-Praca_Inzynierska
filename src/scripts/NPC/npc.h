#ifndef NPC_H
#define NPC_H

#include "../../../lib/raylib.h"
#include "../animation.h"

typedef enum {
    NPC_INMATE,
    NPC_GUARD
} NPCType;

typedef enum {
    BEHAVIOR_IDLE,
    BEHAVIOR_PATROL
    // You can add more like BEHAVIOR_FOLLOW, BEHAVIOR_TALK
} NPCBehavior;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    Texture2D texture;
    Vector2 position;
    Vector2 origin;
    int frame;
    int frameCounter;
    int direction; // -1 or 1 for horizontal patrol
    SpriteAnimation npcAnimation[1];
    Direction dir;
    NPCType type;
    NPCBehavior behavior;
} NPC;


// Declare NPC arrays and counts as extern so they can be accessed globally
extern NPC inmates[10];
extern int numInmates;

extern NPC guards[5];
extern int numGuards;

NPC InitNPC(Texture2D texture, Vector2 position, NPCType type, NPCBehavior behavior);
void UpdateNPC(NPC *npc, float deltaTime);
void DrawNPC(NPC *npc);

#endif
