#include "../../../lib/raylib.h"
#include "player.h"
#include "../../../lib/raymath.h"
#include "../NPC/npc.h"
#include "stdio.h"
#include "../global.h"
#include "../sound/soundManager.h"

double lastAttackTime = 0.0;
double attackCooldown = 0.5; // half a second cooldown

/*
bool tryAttackNPCs(Vector2 clickPos, NPC *npcs, int npcCount) {

    if (!attackMode) return false; // Only allow attacking in attack mode

    float attackRadius = 20.0f;
    int damage = 1;

    for (int i = 0; i < npcCount; i++) {
        if (npcs[i].health <= 0) continue;

        float dist = Vector2Distance(clickPos, npcs[i].position);
        if (dist < attackRadius) {
            npcs[i].health -= damage;
            printf("Clicked and hit NPC %d! New health: %d\n", i, npcs[i].health);
            playAttackSound();

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
*/

bool tryAttackNPCs(Vector2 clickPos, NPC *npcs, int npcCount) {
    if (!attackMode) return false;

    double currentTime = GetTime();
    if (currentTime - lastAttackTime < attackCooldown) {
        return false; // Still cooling down
    }

    float attackRadius = 20.0f;
    float maxDistanceFromPlayer = 32.0f;
    int damage = 3;

    for (int i = 0; i < npcCount; i++) {
        if (npcs[i].health <= 0) continue;

        float distToClick = Vector2Distance(clickPos, npcs[i].position);
        float distToPlayer = Vector2Distance(player.colliderCenter, npcs[i].position);

        if (distToClick < attackRadius && distToPlayer <= maxDistanceFromPlayer) {
            npcs[i].health -= damage;
            printf("Clicked and hit NPC %d! New health: %d\n", i, npcs[i].health);
            printf("Attack distance: %f\n", distToPlayer);
            playAttackSound();

            if (npcs[i].health <= 0) {
                printf("NPC %d has been knocked out!\n", i);
                npcs[i].behavior = BEHAVIOR_IDLE;
            } else {
                npcs[i].behavior = BEHAVIOR_FOLLOW;
            }

            lastAttackTime = currentTime; // Reset cooldown
            return true;
        }
    }

    return false;
}
