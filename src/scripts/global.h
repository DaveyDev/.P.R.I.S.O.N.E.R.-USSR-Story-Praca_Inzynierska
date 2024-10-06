#ifndef GLOBAL_H
#define GLOBAL_H

#include "../scenes/scenes.h"


extern AppScene *currentScene;
extern int maxFPS;

extern Vector2 mousePoint;
extern Color buttonColor;
extern Color noColor;
extern Color bgColor;

// Define a struct for screen resolutions
typedef struct {
    int width;
    int height;
} Resolution;

// Define available resolutions
extern Resolution resolutions[];
extern int currentResolutionIndex;
extern const int numResolutions;  // Declare the number of resolutions





void initGlobals();

#endif