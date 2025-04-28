#ifndef NPC_H
#define NPC_H

#include "../../../lib/raylib.h"
#include "../animation.h"

#define MAX_NPC_PATH 64


typedef enum {
    NPC_INMATE,
    NPC_GUARD
} NPCType;

typedef enum {
    BEHAVIOR_IDLE,
    BEHAVIOR_FOLLOW,
    BEHAVIOR_PATROL,
    BEHAVIOR_SLEEP,
    BEHAVIOR_LUNCH,
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
    Vector2 path[MAX_NPC_PATH];
    int pathLength;
    int pathIndex;
    float moveTimer;
    float pathUpdateTimer;
    float attackCooldown;
    int health;
    Vector2 currentPatrolTarget;
    bool hasPatrolTarget;
    



} NPC;


// Declare NPC arrays and counts as extern so they can be accessed globally
extern NPC inmates[10];
extern int numInmates;

extern NPC guards[5];
extern int numGuards;

NPC initNPC(Texture2D texture, Vector2 position, NPCType type, NPCBehavior behavior);
//void updateNPC(NPC *npc, float deltaTime, Vector2 playerPos);
void updateNPC(NPC *npc, float deltaTime, Vector2 playerPos, int groupIndex, int groupSize);

void drawNPC(NPC *npc, Camera2D camera);
void saveNPCsToFile(const char *filename, NPC *npcArray, int count);
int loadNPCsFromFile(const char *filename, NPC *npcArray, int maxCount, Texture2D texture);
bool isTileTemporarilyBlocked(int row, int col, NPC *self, NPC *all, int count);

#endif
