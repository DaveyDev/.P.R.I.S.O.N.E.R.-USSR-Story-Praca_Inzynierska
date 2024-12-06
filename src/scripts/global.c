//Wiktor
#include "global.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_RESOLUTIONS 3 // Adjust based on the number of resolutions available


// Global variables to store the map, objects, and their dimensions
int **map = NULL;
int **objects = NULL;
int rows = 0;
int cols = 0;
//time_t **lastChangeTimes = NULL;



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
        }
    }

    fclose(file);
}
