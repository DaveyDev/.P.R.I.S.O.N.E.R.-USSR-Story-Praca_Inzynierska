//Wiktor
#ifndef GLOBAL_H
#define GLOBAL_H

#include "../scenes/scenes.h"

#define MAX_MAP_NAME_LENGTH 128 //mapName
#define TILE_SIZE 32 //TILE_SIZE

extern AppScene *currentScene;
extern int maxFPS;

// Global variables
extern int **map;
extern int **objects;
extern int rows;
extern int cols;
extern char mapName[MAX_MAP_NAME_LENGTH];
extern bool windowShouldClose;


extern Vector2 mousePoint;
extern Color buttonColor;
extern Color greySeeThroughColor;
extern Color noColor;
extern Color bgColor;

extern Font baseFont;

// Define a struct for screen resolutions
typedef struct {
    int width;
    int height;
} Resolution;

// Define available resolutions
extern Resolution resolutions[];
extern int currentResolutionIndex;
extern const int numResolutions;  // Declare the number of resolutions

extern char mapPath[256];
extern char itemPath[256];

extern bool wasMapLoaded;
extern bool isNewGame;


void initGlobals();
void loadProperties(const char *filename);


#endif