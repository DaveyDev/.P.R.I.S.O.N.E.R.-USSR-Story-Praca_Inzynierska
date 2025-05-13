
#ifndef DAYCYCLE_H
#define DAYCYCLE_H

#include "../../lib/raylib.h"


// Initializes the day cycle (optional if needed)
void initDayCycle();

// Updates the in-game time
void updateDayCycle();

// Draws the day-night overlay
void drawDayCycleOverlay(int screenWidth, int screenHeight);

// Returns the current time of day (0.0 - 24.0)
float getTimeOfDay();
int getDayCount();
// Sets the day speed (in-game hours per second)
void setDaySpeed(float speed);

float getTime();

void DrawClock(int screenWidth, int screenHeight, int fontSize, Color textColor, Color bgColor);

void setTimeOfDay(float time);
void setDayCount(int count);
void newDayNature();

void updateDayState(int stateNo);



#endif
