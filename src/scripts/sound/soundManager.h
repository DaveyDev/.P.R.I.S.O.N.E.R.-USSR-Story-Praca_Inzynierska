#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "../../../lib/raylib.h"

void initSoundManager();
void updateSoundManager();
void playDoorSound();
void playBackgroundMusic();
void stopBackgroundMusic();
void unloadSoundManager();

#endif
