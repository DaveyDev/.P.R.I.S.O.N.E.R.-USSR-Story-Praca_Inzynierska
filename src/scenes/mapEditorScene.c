//Hubert
#include "../../lib/raylib.h"
#include "scenes.h"
#include "../scripts/map/map.h"
#include "../scripts/global.h"
#include "../scripts/map/editMap.h"

bool wasMapLoadedE = false;

// Persistent camera state variables
Camera2D cameraE = { 0 };   // Define the camera outside of the function
bool isCameraInitializedE = false;

// Editor variables
int selectedTile = 11;   // Default selected tile
int selectedObject = 0; // Default selected object
int mode = 0;

void mapsEditorScene() {
    ClearBackground(GRAY);

    if (!wasMapLoadedE) {
        char mapPath[256];
        snprintf(mapPath, sizeof(mapPath), "data/levels/%s.map", mapName);
        loadMap(mapPath);
        wasMapLoadedE = true;
    }

    if (!isCameraInitializedE) {
        cameraE.target = (Vector2){0, 0};
        cameraE.offset = (Vector2){0, 0};
        cameraE.rotation = 0.0f;
        cameraE.zoom = (resolutions[currentResolutionIndex].height / 600.0f) * 2.0f;
        isCameraInitializedE = true;
    }

    float cameraSpeed = 200.0f * GetFrameTime();
    if (IsKeyDown(KEY_RIGHT)) cameraE.target.x += cameraSpeed;
    if (IsKeyDown(KEY_LEFT)) cameraE.target.x -= cameraSpeed;
    if (IsKeyDown(KEY_DOWN)) cameraE.target.y += cameraSpeed;
    if (IsKeyDown(KEY_UP)) cameraE.target.y -= cameraSpeed;

    drawMap(cameraE);
    updateMap(cameraE);
    editMap(cameraE);

    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), cameraE);
    Vector2 mouseTilePos = (Vector2){(int)(mouseWorldPos.x / TILE_SIZE), (int)(mouseWorldPos.y / TILE_SIZE)};

    

    if (IsKeyPressed(KEY_SPACE)) {
        mode = (mode + 1) % 3;
    }

    selectedTile -= GetMouseWheelMove();

    // Draw UI Panel
    int panelHeight = 100;
    DrawRectangle(0, GetScreenHeight() - panelHeight, GetScreenWidth(), panelHeight, DARKGRAY);

    // Draw Tile Selection
    int tileSize = 50;
    int tileX = 10;
    int tileY = GetScreenHeight() - panelHeight + 10;
    for (int i = 0; i < 10; i++) { // Example: 10 tiles
        DrawRectangle(tileX + i * (tileSize + 5), tileY, tileSize, tileSize, (selectedTile == i) ? RED : LIGHTGRAY);
        DrawText(TextFormat("%d", i), tileX + i * (tileSize + 5) + 15, tileY + 15, 20, BLACK);

        // Handle Tile Selection
        Rectangle tileRect = {tileX + i * (tileSize + 5), tileY, tileSize, tileSize};
        if (CheckCollisionPointRec(GetMousePosition(), tileRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            selectedTile = i;
        }
    }

    // Save Button
    Rectangle saveButton = {GetScreenWidth() - 150, GetScreenHeight() - panelHeight + 10, 60, 30};
    DrawRectangleRec(saveButton, BLUE);
    DrawText("Save", saveButton.x + 10, saveButton.y + 5, 20, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), saveButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        saveMap("data/levels/test1.map");
        printf("saved map");
    }

    // Exit Button
    Rectangle exitButton = {GetScreenWidth() - 80, GetScreenHeight() - panelHeight + 10, 60, 30};
    DrawRectangleRec(exitButton, RED);
    DrawText("Exit", exitButton.x + 10, exitButton.y + 5, 20, WHITE);
    if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        *currentScene = MENU;
        isCameraInitializedE = false;
    }

    DrawText(TextFormat("Tile: %d", selectedTile), 10, 40, 20, WHITE);
    DrawText(TextFormat("Mode: %d", mode), 10, 70, 20, WHITE);


    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GetMousePosition().y < GetScreenHeight() - panelHeight) {
        if (mode == 0) setTile(mouseTilePos, selectedTile, selectedObject);
        if (mode == 1) setTile(mouseTilePos, 0, selectedObject);
    }
}

