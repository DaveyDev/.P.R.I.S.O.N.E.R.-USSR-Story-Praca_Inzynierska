#ifndef SLEEP_H
#define SLEEP_H

#include "stdbool.h"
#include "../../../lib/raylib.h"

void initSleep();
void startSleep();
void updateSleep();
void drawSleep(int screenWidth, int screenHeight);

bool isPlayerSleeping();
bool canSleep();


bool isNearBed(Vector2 playerPos);
void tryToSleep(Camera2D camera);

#endif
