#include "storage.h"
#include "../../../lib/raylib.h"
#include <stdio.h>
#include "idList.h"
#include "../global.h"



Inventory playerInventory;
Chest chestData[100][100];  

int openedChestRow = -1, openedChestCol = -1; // Track opened chest position
bool chestUIOpen = false; // Is the chest window open?

bool IsChest(int objectID) {
    return objectID == CABINET;
}

void UpdateChests(int **map, int mapRows, int mapCols, Camera2D camera) {
    int row = player.position.y / TILE_SIZE;
    int col = player.position.x / TILE_SIZE;

    if (row < 0 || col < 0 || row >= mapRows || col >= mapCols) return;

    // Check if left mouse button is clicked on a chest
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);
        
        int clickedCol = (int)(worldMousePos.x / 32);
        int clickedRow = (int)(worldMousePos.y / 32);

        printf("row: %i, col: %i\n", clickedRow, clickedCol);
        printf("IsChest: %d\n", IsChest(objects[clickedRow][clickedCol]));
        printf("Object ID at [%i, %i]: %d\n", clickedRow, clickedCol, objects[clickedRow][clickedCol]);


        if (clickedRow >= 0 && clickedCol >= 0 && clickedRow < mapRows && clickedCol < mapCols) {
            if (IsChest(objects[clickedRow][clickedCol])) {
                chestUIOpen = !chestUIOpen;  // Toggle chest UI
                printf("chest opened\n");
                
                if (chestUIOpen) {
                    openedChestRow = clickedRow;
                    openedChestCol = clickedCol;
                } else {
                    openedChestRow = -1;
                    openedChestCol = -1;
                }
            }
        }
    }
}


void DrawChestUI() {
    if (!chestUIOpen || openedChestRow == -1 || openedChestCol == -1) return;

    Chest *chest = &chestData[openedChestRow][openedChestCol];

    int uiX = GetScreenWidth() / 2 - 100;
    int uiY = GetScreenHeight() / 2 - 100;
    DrawRectangle(uiX, uiY, 200, 200, DARKGRAY);
    DrawText("Chest Inventory", uiX + 20, uiY + 10, 20, WHITE);

    for (int i = 0; i < chest->storage.itemCount; i++) {
        DrawText(TextFormat("Item %d: %d", i, chest->storage.items[i]), uiX + 20, uiY + 40 + i * 20, 18, WHITE);
    }

    DrawText("[G] Take Item | [Esc] Close", uiX + 20, uiY + 170, 16, WHITE);

    if (IsKeyPressed(KEY_G)) RetrieveItemFromChest(openedChestRow, openedChestCol, &playerInventory);
    if (IsKeyPressed(KEY_ESCAPE)) chestUIOpen = false;
}

bool StoreItemInChest(int row, int col, int itemID, Inventory *playerInventory) {
    int objectID = objects[row][col];  
    if (!IsChest(objectID)) return false;


    Chest *chest = &chestData[row][col];

    if (chest->storage.itemCount >= MAX_ITEMS_IN_CHEST) return false;

    chest->storage.items[chest->storage.itemCount++] = itemID;

    for (int i = 0; i < playerInventory->itemCount; i++) {
        if (playerInventory->items[i] == itemID) {
            playerInventory->items[i] = playerInventory->items[--playerInventory->itemCount];
            return true;
        }
    }
    return false;
}

bool RetrieveItemFromChest(int row, int col, Inventory *playerInventory) {
    int objectID = objects[row][col];  
    if (!IsChest(objectID)) return false;


    Chest *chest = &chestData[row][col];

    if (chest->storage.itemCount == 0 || playerInventory->itemCount >= MAX_ITEMS_IN_CHEST) return false;

    int itemID = chest->storage.items[--chest->storage.itemCount];
    playerInventory->items[playerInventory->itemCount++] = itemID;

    return true;
}
