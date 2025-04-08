#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "../../../lib/raylib.h"

void initSoundManager();
void updateSoundManager();
void playOpenDoorSound();
void playCloseDoorSound();
void playBackgroundMusic();
void stopBackgroundMusic();
void unloadSoundManager();
void playPickupItemSound();
void playFootstepSound();
void calculatePlayerSteps(bool isMoving, float deltaTime);



#endif
