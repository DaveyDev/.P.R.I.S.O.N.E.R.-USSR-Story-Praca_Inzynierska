#include "../../../lib/raylib.h"
#include "player.h"
#include "../../../lib/raymath.h"
#include "../NPC/npc.h"
#include "stdio.h"



bool tryAttackNPCs(Vector2 clickPos, NPC *npcs, int npcCount) {
    float attackRadius = 20.0f;
    int damage = 1;

    for (int i = 0; i < npcCount; i++) {
        if (npcs[i].health <= 0) continue;

        float dist = Vector2Distance(clickPos, npcs[i].position);
        if (dist < attackRadius) {
            npcs[i].health -= damage;
            printf("Clicked and hit NPC %d! New health: %d\n", i, npcs[i].health);

            if (npcs[i].health <= 0) {
                printf("NPC %d has been knocked out!\n", i);
                npcs[i].behavior = BEHAVIOR_IDLE;
            } else {
                npcs[i].behavior = BEHAVIOR_FOLLOW;
            }

            return true; // Hit one — stop
        }
    }

    return false; // No NPC hit
}
