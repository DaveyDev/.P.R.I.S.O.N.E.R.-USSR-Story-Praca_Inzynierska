#include "soundManager.h"
#include "../global.h"

static Sound doorSound;
//static Music backgroundMusic;
//static bool musicPlaying = false;

void initSoundManager() {
    InitAudioDevice();

    doorSound = LoadSound("data/sound/door.wav");
    backgroundMusic = LoadMusicStream("data/sound/background.mp3");
    SetMusicVolume(backgroundMusic, 0.5f);
}

void updateSoundManager() {
    if (musicPlaying) {
        UpdateMusicStream(backgroundMusic);
    }
}

void playDoorSound() {
    PlaySound(doorSound);
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

void unloadSoundManager() {
    UnloadSound(doorSound);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
}
