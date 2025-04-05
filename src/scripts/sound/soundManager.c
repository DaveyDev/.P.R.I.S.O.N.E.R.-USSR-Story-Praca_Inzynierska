#include "soundManager.h"

static Sound doorSound;
static Music backgroundMusic;
static bool musicPlaying = false;

void InitSoundManager(void) {
    InitAudioDevice();

    doorSound = LoadSound("data/sound/door.wav");
    backgroundMusic = LoadMusicStream("data/sound/background.mp3");
    SetMusicVolume(backgroundMusic, 0.5f);
}

void UpdateSoundManager(void) {
    if (musicPlaying) {
        UpdateMusicStream(backgroundMusic);
    }
}

void PlayDoorSound(void) {
    PlaySound(doorSound);
}

void PlayBackgroundMusic(void) {
    if (!musicPlaying) {
        PlayMusicStream(backgroundMusic);
        musicPlaying = true;
    }
}

void StopBackgroundMusic(void) {
    if (musicPlaying) {
        StopMusicStream(backgroundMusic);
        musicPlaying = false;
    }
}

void UnloadSoundManager(void) {
    UnloadSound(doorSound);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
}
