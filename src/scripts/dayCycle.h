
#ifndef DAYCYCLE_H
#define DAYCYCLE_H

#include "../../lib/raylib.h"


// Initializes the day cycle (optional if needed)
void initDayCycle(void);

// Updates the in-game time
void updateDayCycle(void);

// Draws the day-night overlay
void drawDayCycleOverlay(int screenWidth, int screenHeight);

// Returns the current time of day (0.0 - 24.0)
float getTimeOfDay(void);

// Sets the day speed (in-game hours per second)
void setDaySpeed(float speed);

float getTime();

#endif
