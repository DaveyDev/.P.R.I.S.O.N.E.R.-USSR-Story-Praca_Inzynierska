#include "debugCMD.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../../../lib/raylib.h"
#include "../items/items.h"
#include "../player/inventory.h"
#include "../dayCycle.h"

#define MAX_INPUT_LENGTH 128

static bool enteringCommand = false;
static char inputBuffer[MAX_INPUT_LENGTH] = {0};
static int inputIndex = 0;
static bool showColliders = false;
static bool showFPSFlag = false;

void initDebugCMD() {
    enteringCommand = false;
    memset(inputBuffer, 0, MAX_INPUT_LENGTH);
    inputIndex = 0;
}

void processCommand() {
    if (strcmp(inputBuffer, "colliders true") == 0) {
        showColliders = true;
        printf("Colliders enabled.\n");
    } else if (strcmp(inputBuffer, "colliders false") == 0) {
        showColliders = false;
        printf("Colliders disabled.\n");
    } else if (strcmp(inputBuffer, "showFPS true") == 0) {
        showFPSFlag = true;
        printf("FPS counter enabled.\n");
    } else if (strcmp(inputBuffer, "showFPS false") == 0) {
        showFPSFlag = false;
        printf("FPS counter disabled.\n");
    } else if (strncmp(inputBuffer, "give ", 5) == 0) {
        int itemID;
        if (sscanf(inputBuffer + 5, "%d", &itemID) == 1) {
            if (addItemToInventory(itemID, "Unknown Item")) {
                printf("Given item with ID: %d\n", itemID);
            } else {
                printf("Inventory full! Cannot give item.\n");
            }
        } else {
            printf("Invalid format! Use: give <id>\n");
        }
    } else if (strncmp(inputBuffer, "time set ", 9) == 0) {
        int hour;
        if (sscanf(inputBuffer + 9, "%d", &hour) == 1 && hour >= 0 && hour <= 23) {
            setTimeOfDay((float)hour);
            printf("Time set to %02d:00\n", hour);
        } else {
            printf("Invalid format! Use: time set <0-23>\n");
        }
    } else {
        printf("Unknown command: %s\n", inputBuffer);
    }
}


void updateDebugCMD() {
    if (IsKeyPressed(KEY_GRAVE)) {  // ` key
        enteringCommand = !enteringCommand;
        if (enteringCommand) {
            memset(inputBuffer, 0, MAX_INPUT_LENGTH);
            inputIndex = 0;
        }
    }

    if (enteringCommand) {
        // Check for ENTER separately, since GetCharPressed() does not detect it
        if (IsKeyPressed(KEY_ENTER)) {  
            processCommand();  // Execute command
            enteringCommand = false;  // Exit command mode
            memset(inputBuffer, 0, MAX_INPUT_LENGTH);
            inputIndex = 0;
            return;  // Stop further processing
        }

        // Check for BACKSPACE separately
        if (IsKeyPressed(KEY_BACKSPACE) && inputIndex > 0) {
            inputIndex--;
            inputBuffer[inputIndex] = '\0';
        }

        // Process regular character input
        int key = GetCharPressed();
        while (key > 0) {
            if (inputIndex < MAX_INPUT_LENGTH - 1 && key >= 32 && key <= 126) { 
                inputBuffer[inputIndex++] = (char)key;
                inputBuffer[inputIndex] = '\0';
            }
            key = GetCharPressed();
        }
    }
}

void drawDebugCMD() {
    if (enteringCommand) {
        DrawRectangle(10, 10, 400, 40, (Color){0, 0, 0, 150});  // Background
        DrawRectangleLines(10, 10, 400, 40, RAYWHITE);           // Border
        DrawText(inputBuffer, 20, 20, 20, RAYWHITE);            // Text
        DrawText("_", 20 + MeasureText(inputBuffer, 20), 20, 20, RAYWHITE); // Cursor
    }
}

bool isShowCollidersEnabled() {
    return showColliders;
}

void showFPS(){
    if(showFPSFlag){
        DrawText(TextFormat("FPS: %d", GetFPS()), GetScreenWidth() - 130 , 10, 20, RED);
        }
}