#include "../global.h"
#include "npc.h"
#include "../../../lib/raylib.h"



void startLunchForAllNPCs() {
    for (int i = 0; i < numInmates; i++) {
        inmates[i].behavior = BEHAVIOR_LUNCH;
        inmates[i].hasPatrolTarget = false;
        inmates[i].gotFood = false;
    }

    // Reset queues
    for (int i = 0; i < MAX_FOOD_BLOCKS; i++) {
        foodQueueLengths[i] = 0;
    }
}

void startPatrolForAllNPCs() {
    for (int i = 0; i < numInmates; i++) {
        inmates[i].behavior = BEHAVIOR_PATROL;
        //inmates[i].hasPatrolTarget = false;
        //inmates[i].gotFood = false;
    }
}

void startFoodForGuardNPC() {
        guards[0].behavior = BEHAVIOR_LUNCH_GUARD;
        //guard->behavior = BEHAVIOR_LUNCH_GUARD;
        guards[0].hasPatrolTarget = false;
        guards[0].pathIndex = 0;
        guards[0].pathLength = 0;
        
}

void startFreeTimeForAllNPCs() {
    for (int i = 0; i < numInmates; i++) {
        inmates[i].behavior = BEHAVIOR_FREE_TIME;
        inmates[i].hasPatrolTarget = false;
        inmates[i].pathIndex = 0;
        inmates[i].pathLength = 0;
    }
}
