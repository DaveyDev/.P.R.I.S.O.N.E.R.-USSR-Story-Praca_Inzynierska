#include "soundManager.h"
#include "../global.h"
#include "../player/player.h"
#include <math.h>

static Sound doorOpenSound;
static Sound doorCloseSound;
static Sound pickupItemSound;
static Sound footstepSound;
static Sound attackSound;
static Sound bellSound;

//static Music backgroundMusic;
//static bool musicPlaying = false;

void initSoundManager() {
    InitAudioDevice();

    doorOpenSound = LoadSound("data/sound/doorOpen.wav");
    pickupItemSound = LoadSound("data/sound/pickup.wav");
    doorCloseSound = LoadSound("data/sound/doorClose.wav");
    backgroundMusic = LoadMusicStream("data/sound/background.mp3");
    footstepSound = LoadSound("data/sound/footstep.wav"); 
    attackSound = LoadSound("data/sound/attack1.wav"); 
    bellSound = LoadSound("data/sound/timeBell.wav");

    SetMusicVolume(backgroundMusic, 0.5f);
    
}

void updateSoundManager() {
    if (musicPlaying) {
        UpdateMusicStream(backgroundMusic);
    }
}

void playOpenDoorSound() {
    PlaySound(doorOpenSound);
}
void playCloseDoorSound(){
    PlaySound(doorCloseSound);
}
void playPickupItemSound(){
    PlaySound(pickupItemSound);
}
void playFootstepSound() {
    PlaySound(footstepSound);
}
void playAttackSound() {
    PlaySound(attackSound);
}
void playBellSound() {
    PlaySound(bellSound);
}

void playBackgroundMusic() {
    if (!musicPlaying) {
        PlayMusicStream(backgroundMusic);
        musicPlaying = true;
    }
}

void stopBackgroundMusic() {
    if (musicPlaying) {
        StopMusicStream(backgroundMusic);
        musicPlaying = false;
    }
}

void calculatePlayerSteps(bool isMoving, float deltaTime){  //calculates if player footstep sound should play 
    if (isMoving) {
        player.footstepTimer += deltaTime;
        if (player.footstepTimer >= 0.5f) { // 0.4 seconds between steps
            playFootstepSound();
            player.footstepTimer = 0.0f;
        }
        } else {
        player.footstepTimer = 0.5f; // Reset so it plays immediately when walking again
        }
}

void unloadSoundManager() {
    UnloadSound(doorOpenSound);
    UnloadSound(doorCloseSound);
    UnloadSound(pickupItemSound);
    UnloadMusicStream(backgroundMusic);
    UnloadSound(footstepSound);
    UnloadSound(attackSound);
    UnloadSound(bellSound);


    CloseAudioDevice();
}
