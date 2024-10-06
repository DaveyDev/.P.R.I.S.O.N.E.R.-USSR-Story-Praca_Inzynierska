#include "global.h"
#include <stdlib.h>


AppScene *currentScene = NULL;

int maxFPS = 60;
int score = 0;
int bestScore = 0;


Vector2 mousePoint = {};
Color buttonColor = (Color){0, 0, 0, 50};
Color noColor = (Color){0, 0, 0, 0};
Color bgColor = (Color){0, 150, 100, 255};


// Initialize resolutions
Resolution resolutions[] = {
    {800, 600},
    {1024, 768},
    {1920, 1080}
};

// Set the initial resolution index (e.g., 0 for 800x600)
int currentResolutionIndex = 0;
const int numResolutions = sizeof(resolutions) / sizeof(resolutions[0]);  // Now numResolutions is defined properly



void initGlobals() {
    currentScene = (AppScene *)malloc(sizeof(AppScene));
    if (currentScene != NULL) {
        *currentScene = MENU; // Initialize the value
    }

}