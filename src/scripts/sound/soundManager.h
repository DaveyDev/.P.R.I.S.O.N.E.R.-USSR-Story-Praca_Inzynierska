#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "../../lib/raylib.h"

void InitSoundManager(void);
void UpdateSoundManager(void);
void PlayDoorSound(void);
void PlayBackgroundMusic(void);
void StopBackgroundMusic(void);
void UnloadSoundManager(void);

#endif
