//Wiktor
#ifndef GLOBAL_H
#define GLOBAL_H

#include "../scenes/scenes.h"

#define MAX_MAP_NAME_LENGTH 128 //mapName
#define TILE_SIZE 32 //TILE_SIZE

#define MAX_TREE_ID 1003
#define MAX_OBJECT_ID 2032

#define MAX_PATROL_POINTS 64
#define MAX_NPC_COUNT 32

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

//map and items
extern char mapPath[256];
extern char itemPath[256];
extern bool wasMapLoaded;
extern bool isNewGame;

//music
extern Music backgroundMusic;
extern float musicVolume;
extern bool musicPlaying;
extern bool musicEnabled;
extern bool playerWon;
extern bool isEditor;


extern Vector2 patrolPoints[MAX_PATROL_POINTS];
extern int patrolPointCount;
extern int npcPatrolIndex[MAX_NPC_COUNT];


//work
extern bool isWorkDone;



void initGlobals();
void loadProperties(const char *filename);


#endif