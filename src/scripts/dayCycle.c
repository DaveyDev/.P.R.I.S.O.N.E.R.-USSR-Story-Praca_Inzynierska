#include "../../lib/raylib.h"
#include "dayCycle.h"
#include "stdio.h"

static float timeOfDay = 0.0f;
static float daySpeed = 0.02f; // Default: 1 full day in 4 minutes
static int dayCount = 1; // Starts at day 1


void initDayCycle() {
    timeOfDay = 8.0f; // Start at 8 AM
    daySpeed = 0.02f;
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


void updateDayCycle() {
    timeOfDay += GetFrameTime() * daySpeed;
    if (timeOfDay >= 24.0f) {
        timeOfDay -= 24.0f;
        dayCount++;
    }
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
    snprintf(timeText, sizeof(timeText), "%02d:%02d", hour, minute);  // "08:11"

    int padding = 10;
    float charBoxWidth = fontSize * 0.6f;  // fixed slot width for each character

    int totalTextWidth = charBoxWidth * 5; // 5 characters: 2 digits + colon + 2 digits
    int boxWidth = totalTextWidth + padding * 2;
    int boxHeight = fontSize + padding * 2;

    int boxX = screenWidth - boxWidth - 10;
    int boxY = 10;
    int textY = boxY + padding;

    DrawRectangleRounded((Rectangle){ boxX, boxY, boxWidth, boxHeight }, 0.2f, 8, bgColor);

    // Adjust for the colon to be centered
    for (int i = 0; i < 5; i++) {
        const char *chr = TextSubtext(timeText, i, 1);
        int charActualWidth = MeasureText(chr, fontSize);

        // Position digits normally but ensure colon is centered
        float charX = boxX + padding + i * charBoxWidth;

        // For the colon, manually center it between the digits
        if (i == 2) {
            // Manually adjust for the colon's centered position
            float offsetX = boxX + padding + (charBoxWidth * 2) + (charBoxWidth / 2 - charActualWidth / 2);
            DrawText(chr, (int)offsetX, textY, fontSize, textColor);
        } else {
            // For all other characters, just right-align them within their box
            float offsetX = charX + (charBoxWidth - charActualWidth);
            DrawText(chr, (int)offsetX, textY, fontSize, textColor);
        }
    }
    
    char dayText[16];
    snprintf(dayText, sizeof(dayText), "Day %d", dayCount);
    DrawText(dayText, boxX, boxY + boxHeight + 4, fontSize - 4, textColor); // Slightly below the clock

}

void newDay(){ //temporary
    
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

