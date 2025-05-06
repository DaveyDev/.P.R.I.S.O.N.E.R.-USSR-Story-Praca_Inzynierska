//Wiktor
#include "global.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "translation.h"
#include "sound/soundManager.h"
#include "NPC/npc.h"


#define MAX_RESOLUTIONS 3 // Adjust based on the number of resolutions available


// Global variables to store the map, objects, and their dimensions
int **map = NULL;
int **objects = NULL;
int **details = NULL;
int rows = 0;
int cols = 0;
char mapName[MAX_MAP_NAME_LENGTH] = "\0";
//time_t **lastChangeTimes = NULL;
bool windowShouldClose = false;
Font baseFont;


AppScene *currentScene = NULL;

int maxFPS = 60;
int score = 0;
int bestScore = 0;


Vector2 mousePoint = {};
Color buttonColor = (Color){0, 0, 0, 50};
Color greySeeThroughColor = (Color){0, 0, 0, 128};
Color noColor = (Color){0, 0, 0, 0};
Color bgColor = (Color){100, 150, 100, 255};


// Initialize resolutions
Resolution resolutions[] = {
    {800, 600},
    {1024, 768},
    {1920, 1080}
};

// Set the initial resolution index (e.g., 0 for 800x600)
int currentResolutionIndex = 0;
const int numResolutions = sizeof(resolutions) / sizeof(resolutions[0]);  // Now numResolutions is defined properly

char language[16] = "ENG";       // Default language

//map and items
char mapPath[256] = "\0";
char itemPath[256] = "\0";
bool wasMapLoaded = false;
bool isNewGame = false;
bool playerWon = false;
bool isEditor = false;

//music
Music backgroundMusic;
float musicVolume = 1.0f;
bool musicPlaying = false;
bool musicEnabled = false;

//work
bool isWorkDone = false;


Vector2 patrolPoints[MAX_PATROL_POINTS] = {0};
int patrolPointCount = 0;
int npcPatrolIndex[MAX_NPC_COUNT] = {0};

Vector2 foodTakeBlocks[MAX_FOOD_BLOCKS];
int foodBlockCount = 0;
int foodQueueLengths[MAX_FOOD_BLOCKS]; // how many NPCs waiting at each block
int foodQueueLengths[MAX_FOOD_BLOCKS];
NPC *foodQueues[MAX_FOOD_BLOCKS][MAX_QUEUE_PER_BLOCK]; // pointers to NPCs in queue
Vector2 freeTimeBlocks[MAX_FREE_TILES];
int freeTimeBlockCount = 0;

//attack 
bool attackMode = false;

//work
bool wasFoodGiven = false;

Vector2 treeBlocks[MAX_RESOURCE_BLOCKS];
int treeBlockCount = 0;

Vector2 rockBlocks[MAX_RESOURCE_BLOCKS];
int rockBlockCount = 0;



void initGlobals() {
    currentScene = (AppScene *)malloc(sizeof(AppScene));
    if (currentScene != NULL) {
        *currentScene = MENU; // Initialize the value
    }
    baseFont = LoadFont("data/font/KingthingsTrypewriter2.ttf");
    
}

// Function to map language code to enum
Language getLanguageEnum(const char *langCode) {
    if (strcmp(langCode, "ENG") == 0) {
        return LANG_ENGLISH;
    } else if (strcmp(langCode, "PL") == 0) {
        return LANG_POLISH;
    } 
    return LANG_ENGLISH; // Default to unknown if not recognized
}


// Function to load properties from game.properties
void loadProperties(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open properties file\n");
        return;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        char key[64], value[64];
        sscanf(line, "%63[^=]=%63[^\n]", key, value);

        if (strcmp(key, "resolution") == 0) {
            currentResolutionIndex = atoi(value); // Convert string to int
            if (currentResolutionIndex < 0 || currentResolutionIndex >= MAX_RESOLUTIONS) {
                currentResolutionIndex = 0; // Default to first resolution if out of range
            }
        } else if (strcmp(key, "fullscreen") == 0) {
            //fullscreen = (strcmp(value, "true") == 0);
        } else if (strcmp(key, "language") == 0) {
            strncpy(language, value, sizeof(language) - 1);
            language[sizeof(language) - 1] = '\0'; // Ensure null termination
            currentLanguage = getLanguageEnum(language); // Map to enum
        }  else if (strcmp(key, "music") == 0) {
            musicEnabled = (strcmp(value, "true") == 0);
            if (musicEnabled) {
                playBackgroundMusic();
            }

        } else if (strcmp(key, "music_volume") == 0) {
            musicVolume = strtof(value, NULL); // Convert string to float
        }

        
    }

    fclose(file);
}
