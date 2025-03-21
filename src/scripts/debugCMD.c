#include "debugCMD.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../../lib/raylib.h"

#define MAX_INPUT_LENGTH 128

static bool enteringCommand = false;
static char inputBuffer[MAX_INPUT_LENGTH] = {0};
static int inputIndex = 0;
static bool showColliders = false;

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
