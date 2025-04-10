#include "../../lib/raylib.h"
#include "dayCycle.h"
#include "stdio.h"

static float timeOfDay = 0.0f;
static float daySpeed = 0.1f; // Default: 1 full day in 4 minutes

void initDayCycle(void) {
    timeOfDay = 8.0f; // Start at 8 AM
    daySpeed = 0.1f;
}

void setDaySpeed(float speed) {
    daySpeed = speed;
}

float getTimeOfDay(void) {
    return timeOfDay;
}

void updateDayCycle(void) {
    timeOfDay += GetFrameTime() * daySpeed;
    if (timeOfDay >= 24.0f) timeOfDay -= 24.0f;
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

    printf("darkness: %f\n", darkness);

    Color overlay = (Color){ 0, 0, 0, (unsigned char)(darkness * 255) };
    DrawRectangle(0, 0, screenWidth, screenHeight, overlay);
}

float getTime(){
    return timeOfDay;
}