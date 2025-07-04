#include "../../lib/raylib.h"
#include "dayCycle.h"
#include "stdio.h"
#include "map/map.h"
#include <stdlib.h> // for rand()
#include "items/idList.h"
#include "NPC/startBehavior.h"
#include "NPC/npc.h"
#include "NPC/trade.h"
#include "sound/soundManager.h"
#include "global.h"
#include "player/player.h"

static float timeOfDay = 0.0f;
static float daySpeed = 0.02f; // Default: 1 full day in 4 minutes
static int dayCount = 1; // Starts at day 1
static float lastTimeOfDay = 0.0f;

bool timeReached(float hour) {
    return (lastTimeOfDay < hour && timeOfDay >= hour);
}

void initDayCycle() {
    timeOfDay = 6.5f; // Start at 6:30 AM
    daySpeed = 0.03f;
}

void setDaySpeed(float speed) {
    daySpeed = speed;
}

float getTimeOfDay() {
    return timeOfDay;
}

int getDayCount() {
    return dayCount;
}

void setTimeOfDay(float time) {
    timeOfDay = time;
}

void setDayCount(int count) {
    dayCount = count;
}

void checkIfStateWasDone(){
    if(!player.isStateDone){
        guards[1].lastBehavior = inmates[1].behavior;
        guards[1].behavior = BEHAVIOR_DISCIPLINE_HIT;
    }
}


void updateDayCycle() {
    float previousTime = timeOfDay;  // Store before updating
    timeOfDay += GetFrameTime() * daySpeed;

    if (timeOfDay >= 24.0f) {
        timeOfDay -= 24.0f;
        dayCount++;
        newDayNature();
        assignTradesToAllNPCs(inmates, numInmates);
        previousTime = 0.0f;  // Reset for next day triggers
    }

    if (previousTime < 7.0f && timeOfDay >= 7.0f){ //roll call
        printf("roll call time triggered\n");
        startRollCallForAllNPCs();
        playBellSound();
        updateDayState(0);
        wasFoodGiven = false;
        isWorkDone = false;
        workAmount = 0;
        //startFoodForGuardNPC();
        player.isStateDone = false;
    }

    if (previousTime < 7.5f && timeOfDay >= 7.5f){ // food
        printf("Guard food time triggered\n");
        startFoodForGuardNPC();
    }

    if (previousTime < 8.0f && timeOfDay >= 8.0f) { // food
        checkIfStateWasDone();
        printf("Eating time triggered\n");
        startLunchForAllNPCs();
        startPatrolForAllNPCs();
        startFoodForGuardNPC();
        playBellSound();
        updateDayState(1);
        player.isStateDone = false;
    }

    if (previousTime < 9.0f && timeOfDay >= 9.0f) { // free time
        checkIfStateWasDone();
        printf("free time triggered\n");
        startFreeTimeForAllNPCs();
        startPatrolForAllNPCs();
        playBellSound();
        updateDayState(2);
        player.isStateDone = false;
    }

    if (previousTime < 11.0f && timeOfDay >= 11.0f) { //work time 
        checkIfStateWasDone();
        printf("work time triggered\n");
        startWorkForAllNPCs();
        playBellSound();
        updateDayState(3);
        player.isStateDone = false;
    }

    if (previousTime < 16.0f && timeOfDay >= 16.0f) { //free time
        checkIfStateWasDone();
        printf("free time triggered\n");
        startFreeTimeForAllNPCs();
        playBellSound();
        updateDayState(2);
        player.isStateDone = false;
    }

    if (previousTime < 20.0f && timeOfDay >= 20.0f) { //roll call
        checkIfStateWasDone();
        printf("free time triggered\n");
        startRollCallForAllNPCs();
        playBellSound();
        updateDayState(0);
        player.isStateDone = false;
    }

    if (previousTime < 21.0f && timeOfDay >= 21.0f) { //sleep
        checkIfStateWasDone(); 
        printf("sleep time triggered\n");
        startSleepForAllNPCs();
        playBellSound();
        updateDayState(4);
        player.isStateDone = false;
    }


    if (previousTime < 22.0f && timeOfDay >= 22.0f) {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                spawnReserved[row][col] = false;
            }
        }
        startSleepForAllNPCs();
    }

    lastTimeOfDay = timeOfDay;  // Optional if you use it elsewhere
}


void drawDayCycleOverlay(int screenWidth, int screenHeight) {
    float darkness = 0.0f;

    if (timeOfDay >= 20.0f || timeOfDay < 5.0f) {
        darkness = 0.6f; // Night
    }
    else if (timeOfDay >= 17.0f && timeOfDay < 20.0f) {
        darkness = 0.6f * ((timeOfDay - 17.0f) / 3.0f); // Evening
    }
    else if (timeOfDay >= 5.0f && timeOfDay < 8.0f) {
        darkness = 0.6f * (1.0f - ((timeOfDay - 5.0f) / 3.0f)); // Morning
    }

    //printf("darkness: %f\n", darkness);

    Color overlay = (Color){ 0, 0, 0, (unsigned char)(darkness * 255) };
    DrawRectangle(0, 0, screenWidth, screenHeight, overlay);
}

float getTime(){
    return timeOfDay;
}

void DrawClock(int screenWidth, int screenHeight, int fontSize, Color textColor, Color bgColor) {
    int hour = (int)timeOfDay;
    int minute = (int)((timeOfDay - hour) * 60.0f);

    char timeText[6];
    snprintf(timeText, sizeof(timeText), "%02d:%02d", hour, minute);

    char dayText[16];
    snprintf(dayText, sizeof(dayText), "Day %d", dayCount);

    const char* stateText = "";
    if (currentDayState >= 0 && currentDayState < NUM_DAY_STATES) {
        stateText = dayStates[currentDayState];
    }

    int padding = 10;
    float charBoxWidth = fontSize * 0.6f;
    int timeTextWidth = charBoxWidth * 5;
    int dayTextWidth = MeasureText(dayText, fontSize - 4);
    int stateTextWidth = MeasureText(stateText, fontSize - 4);

    // Define top bar dimensions
    int barWidth = screenWidth / 2;
    int barHeight = fontSize + padding * 2;
    int barX = (screenWidth - barWidth) / 2;
    int barY = 10;
    int textY = barY + padding;

    DrawRectangleRounded((Rectangle){ barX, barY, barWidth, barHeight }, 0.2f, 8, bgColor);

    // Draw clock on the right side of the bar
    for (int i = 0; i < 5; i++) {
        const char *chr = TextSubtext(timeText, i, 1);
        int charActualWidth = MeasureText(chr, fontSize);
        float charX = barX + barWidth - padding - timeTextWidth + i * charBoxWidth;

        if (i == 2) {
            float offsetX = charX + (charBoxWidth / 2 - charActualWidth / 2);
            DrawText(chr, (int)offsetX, textY, fontSize, textColor);
        } else {
            float offsetX = charX + (charBoxWidth - charActualWidth);
            DrawText(chr, (int)offsetX, textY, fontSize, textColor);
        }
    }

    // Draw day text near the left side
    int dayTextX = barX + padding;
    DrawText(dayText, dayTextX, textY, fontSize - 4, textColor);


    Color stateTextColor;
    if(player.isStateDone) {
        stateTextColor = GREEN;
        } else {
            stateTextColor = textColor;
        }



    // Draw state text centered
    int stateTextX = barX + (barWidth - stateTextWidth) / 2;
    DrawText(stateText, stateTextX, textY, fontSize - 4, stateTextColor);
}


void newDayNature() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int obj = objects[row][col];

            // TREE STUMP to SPRUCE
            if (obj == TREE_STUMP) {
                if (rand() % 3 == 0) {
                    int regrowRow = row + (rand() % 3 - 1);
                    int regrowCol = col + (rand() % 3 - 1);

                    if (regrowRow >= 0 && regrowRow < rows &&
                        regrowCol >= 0 && regrowCol < cols &&
                        objects[regrowRow][regrowCol] == 0) {

                        objects[row][col] = 0;
                        objects[regrowRow][regrowCol] = SPRUCE;
                    }
                }
            }

            // STONE to SIMPLE_STONE
            else if (obj == STONE) {
                if (rand() % 3 == 0) {
                    int regrowRow = row + (rand() % 3 - 1);
                    int regrowCol = col + (rand() % 3 - 1);

                    if (regrowRow >= 0 && regrowRow < rows &&
                        regrowCol >= 0 && regrowCol < cols &&
                        objects[regrowRow][regrowCol] == 0) {

                        objects[row][col] = 0;
                        objects[regrowRow][regrowCol] = SIMPLE_STONE;
                    }
                }
            }

            // SIMPLE_STONE to random ore
            else if (obj == SIMPLE_STONE) {
                if (rand() % 3 == 0) {
                    
                        int ores[] = {COPPER_ORE, COAL_ORE, IRON_ORE, URANIUM_ORE, GOLD_ORE};
                        int oreIndex = rand() % 5;
                        objects[row][col] = ores[oreIndex];
                    
                }
            }
        }
    }
}








void SaveDayCycle(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file) {
        fwrite(&timeOfDay, sizeof(float), 1, file);
        fwrite(&dayCount, sizeof(int), 1, file);  // Save day count too
        fclose(file);
    }
}


void LoadDayCycle(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        fread(&timeOfDay, sizeof(float), 1, file);
        fread(&dayCount, sizeof(int), 1, file);  // Load day count
        fclose(file);
    }
}

void updateDayState(int stateNo) {
    if (stateNo >= 0 && stateNo < NUM_DAY_STATES) {
        currentDayState = stateNo;
    }
}

