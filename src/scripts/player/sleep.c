#include "sleep.h"
#include "../../../lib/raylib.h"
#include "../../../lib/raymath.h"
#include "../dayCycle.h"  // adjust path if different
#include "../map/map.h"
#include "../items/idList.h"  // where BED_UPPER and BED_BOTTOM are defined
#include "../player/player.h" // for accessing or modifying player state
#include "../NPC/startBehavior.h"

static bool isSleeping = false;
static float sleepTimer = 0.0f;
static float sleepDuration = 3.0f;
float sleepCounter = 0.0f;  // This will hold the current state of the dot animation (floating point for time-based control)
float sleepMaxTime = 0.5f;  // This will control the duration of one full "dot cycle" (in seconds)


void initSleep() {
    isSleeping = false;
    sleepTimer = 0.0f;
}

void startSleep() {
    if (canSleep()) {
        isSleeping = true;
        sleepTimer = 0.0f;
    }
}

bool canSleep() {
    float time = getTimeOfDay();
    // Allow sleep at night (8PM to 6AM)
    return time >= 20.0f || time < 6.0f;
}

bool isPlayerSleeping() {
    return isSleeping;
}

void updateSleep() {
    if (!isSleeping) return;

    sleepTimer += GetFrameTime();

    if (sleepTimer >= sleepDuration) {
        float time = getTimeOfDay();
        if (time >= 20.0f || time < 6.0f) {
            setTimeOfDay(6.5f);
            setDayCount(getDayCount() + 1);
        } else {
            //setTimeOfDay(time + 6.0f);
            //if (getTimeOfDay() >= 24.0f) {
                //setTimeOfDay(getTimeOfDay() - 24.0f);
                //setDayCount(getDayCount() + 1);
            //}
        }
        newDayNature();
        player.wasKnockedOutToday = false;
        isSleeping = false;
    }
}


void drawSleep(int screenWidth, int screenHeight) {
    if (!isSleeping) return;

    // Get the time elapsed since the last frame (delta time)
    float deltaTime = GetFrameTime();

    // Update the sleepCounter based on delta time
    sleepCounter += deltaTime;

    // Calculate the number of dots to display based on sleepCounter and sleepMaxTime
    int dots = (int)(sleepCounter / sleepMaxTime) % 4; // 4 possible states: 0, 1, 2, 3 dots

    // Create the string for "sleeping" with the appropriate number of dots
    char sleepText[20];
    snprintf(sleepText, sizeof(sleepText), "Sleeping%s", &("...."[dots]));

    // Draw the overlay and text
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 1.0f));
    DrawText(sleepText, screenWidth / 2 - 60, screenHeight / 2, 20, WHITE);
    
    // Reset the counter if it exceeds the duration of one full cycle
    if (sleepCounter >= sleepMaxTime * 4) {
        sleepCounter = 0.0f;  // Reset after completing one cycle of "sleeping...."
    }
}


bool isBedTile(int id) {
    return id == GREEN_BED_UPPER || id == GREEN_BED_BOTTOM;
}



void tryToSleep(Camera2D camera) {
    Vector2 worldMouse = GetScreenToWorld2D(GetMousePosition(), camera);
    int tileX = (int)(worldMouse.x / 32);
    int tileY = (int)(worldMouse.y / 32);

    if (tileX >= 0 && tileX < cols && tileY >= 0 && tileY < rows) {
        if (isBedTile(objects[tileY][tileX]) || isBedTile(details[tileY][tileX])) {
            // center of bed tile
            Vector2 bedWorldPos = { tileX * 32 + 16, tileY * 32 + 16 };

            // use colliderCenter if available, or manually offset player pos
            Vector2 playerWorld = GetScreenToWorld2D(player.position, camera);
            Vector2 playerCenter = { playerWorld.x + 16, playerWorld.y + 32 }; // depends on sprite size

            float dist = Vector2Distance(playerCenter, bedWorldPos);

            if (dist <= 32.0f) {
                if (canSleep()) {
                    startSleep(); // << real sleep logic
                    
                    useHealth(-5.0f); // restore health if you want
                    //player.food = player.food + 5.0f;
                    float targetMax = player.maxFood - getDayCount();
                    if (targetMax < 1.0f) targetMax = 1.0f;

                    if (player.food + player.extraFood < targetMax) {
                        player.food = targetMax;
                    }

                    // optionally: reset hunger, thirst, etc.
                    printf("Player has started sleeping...\n");
                } else {
                    printf("You can only sleep at night.\n");
                }
            } else {
                printf("Too far from the bed (%.2fpx).\n", dist);
            }
        }
    }
}

void triggerForcedSleep() {
    isSleeping = true;
    sleepTimer = 0.0f;

    // Force time skip and start a new day
    


    setTimeOfDay(6.5f);
    setDayCount(getDayCount() + 1);
    newDayNature();
    updateDayState(4); // sleep
    startSleepForAllNPCs();
    startPatrolForAllNPCs();

    // Wake up nearly dead
    player.health = 1.0f;
    //player.food = 3.0f;  // optional
    

    printf("Player was knocked out and forced to sleep...\n");

    // You can even trigger a black screen animation or play a sound here
}
