#include <stdio.h>
#include <string.h>
#include "../../lib/raylib.h"
#include "../scripts/global.h"
#include "../scripts/map/map.h"
#include "../scripts/map/editMap.h"

#define MAX_NAME_LENGTH 64

//char mapName[MAX_NAME_LENGTH] = "";  // Store entered map name
int mapWidth = 10;  // Default map width
int mapHeight = 10; // Default map height

void createMapScene() {
    ClearBackground(DARKGRAY);

    DrawText("Create New Map", 20, 20, 30, WHITE);

    // Input Fields
    DrawText("Map Name:", 20, 70, 20, WHITE);
    DrawRectangle(150, 65, 200, 30, BLACK);
    DrawText(mapName, 160, 70, 20, WHITE);

    DrawText("Height:", 20, 120, 20, WHITE);
    DrawRectangle(150, 115, 100, 30, BLACK);
    DrawText(TextFormat("%d", mapWidth), 160, 120, 20, WHITE);

    DrawText("Width:", 20, 170, 20, WHITE);
    DrawRectangle(150, 165, 100, 30, BLACK);
    DrawText(TextFormat("%d", mapHeight), 160, 170, 20, WHITE);

    // Handle text input for map name
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32 && key <= 125) && (strlen(mapName) < MAX_NAME_LENGTH - 1)) {
            int len = strlen(mapName);
            mapName[len] = (char)key;
            mapName[len + 1] = '\0';
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && strlen(mapName) > 0) {
        mapName[strlen(mapName) - 1] = '\0';
    }

    // Adjust width and height with arrow keys
    if (IsKeyPressed(KEY_RIGHT)) mapWidth++;
    if (IsKeyPressed(KEY_LEFT) && mapWidth > 1) mapWidth--;
    if (IsKeyPressed(KEY_UP)) mapHeight++;
    if (IsKeyPressed(KEY_DOWN) && mapHeight > 1) mapHeight--;

    // Create Map Button
    Rectangle createButton = {20, 230, 120, 40};
    DrawRectangleRec(createButton, BLUE);
    DrawText("Create Map", createButton.x + 10, createButton.y + 10, 20, WHITE);

    // Back Button
    Rectangle backButton = {160, 230, 80, 40};
    DrawRectangleRec(backButton, RED);
    DrawText("Back", backButton.x + 20, backButton.y + 10, 20, WHITE);

    // Handle button clicks
    if (CheckCollisionPointRec(GetMousePosition(), createButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (strlen(mapName) > 0) {
            
            createEmptyMap(mapWidth, mapHeight);
            *currentScene = EDITOR;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScene = MENU;
    }
}
