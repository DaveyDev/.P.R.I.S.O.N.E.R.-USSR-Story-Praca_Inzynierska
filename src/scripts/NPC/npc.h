#ifndef NPC_H
#define NPC_H

#include "../../../lib/raylib.h"
#include "../animation.h"
#include "../player/inventory.h"

#define MAX_NPC_PATH 64
#define MAX_ROWS 100
#define MAX_COLS 100


#define ANIMATIONS_PER_NPC 5
#define FRAMES_PER_ANIMATION 4


typedef enum {
    NPC_INMATE,
    NPC_GUARD
} NPCType;

typedef enum {
    BEHAVIOR_IDLE,  //just standing
    BEHAVIOR_FOLLOW, //Follows you and beats the shit out of you
    BEHAVIOR_PATROL, //going from one patrol point to another
    BEHAVIOR_SLEEP, //sleeping
    BEHAVIOR_LUNCH, //receaving food and eating
    BEHAVIOR_LUNCH_GUARD, //giving food
    BEHAVIOR_FREE_TIME, // mostly standing 
    BEHAVIOR_WORK, //working (cutting down trees and stones)
    BEHAVIOR_TALKING, // talking with you
} NPCBehavior;

typedef enum {
    JOB_NONE,
    JOB_WOOD,
    JOB_ROCK,
} JobType;




typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    Texture2D texture;
    Vector2 position;
    Vector2 prevsPosition;
    Vector2 origin;
    int frame;
    int frameCounter;
    int direction; // -1 or 1 for horizontal patrol
    SpriteAnimation npcAnimation[5];
    int animationNumber;
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
    bool gotFood;
    int queueIndex; // position in queue, 0 = first
    int queueTargetBlock; // index of the food block they're queuing at
    JobType job;
    float workTimer;
    Vector2 resourceTile;  // Stores the tile where the NPC is working
    NPCBehavior lastBehavior;
    int requestedItemId;   // ID of item they want
    int rewardItemId;      // ID of item they give
    char rewardItemName[20];
    bool tradeCompleted;    // optional flag to prevent repeating trade
    bool isTalking;
    int reservedRow;
    int reservedCol;

    



} NPC;


// Declare NPC arrays and counts as extern so they can be accessed globally
extern NPC inmates[10];
extern int numInmates;

extern NPC guards[5];
extern int numGuards;

extern bool spawnReserved[MAX_ROWS][MAX_COLS];

NPC initNPC(Texture2D texture, Vector2 position, NPCType type, NPCBehavior behavior);
//void updateNPC(NPC *npc, float deltaTime, Vector2 playerPos);
void updateNPC(NPC *npc, float deltaTime, Vector2 playerPos, int groupIndex, int groupSize);

void drawNPC(NPC *npc, Camera2D camera);
void saveNPCsToFile(const char *filename, NPC *npcArray, int count);
int loadNPCsFromFile(const char *filename, NPC *npcArray, int maxCount, Texture2D texture);
bool isTileTemporarilyBlocked(int row, int col, NPC *self, NPC *all, int count);
void handleNPCClick(int inmateNo);
void DrawBarterUI();
void assignRandomTradeToNPC(NPC *npc);
void assignTradesToAllNPCs(NPC *npcs, int npcCount);
void InitNPCAnimations(NPC *npc, Texture2D texture, int rowOffset);

#endif
