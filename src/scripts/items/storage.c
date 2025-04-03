#include "storage.h"
#include "../../../lib/raylib.h"
#include <stdio.h>
#include "idList.h"
#include "../global.h"
#include "../player/inventory.h"


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

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);

        int clickedCol = (int)(worldMousePos.x / TILE_SIZE);
        int clickedRow = (int)(worldMousePos.y / TILE_SIZE);

        if (clickedRow >= 0 && clickedCol >= 0 && clickedRow < mapRows && clickedCol < mapCols) {
            if (IsChest(objects[clickedRow][clickedCol])) {
                chestUIOpen = !chestUIOpen;
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

    int uiX = GetScreenWidth() / 2 - 150;
    int uiY = GetScreenHeight() / 2 - 120;

    DrawRectangle(uiX, uiY, 300, 240, DARKGRAY);
    DrawRectangleLines(uiX, uiY, 300, 240, WHITE);
    DrawText("Chest Inventory", uiX + 50, uiY + 10, 24, YELLOW);

    Vector2 mousePos = GetMousePosition();
    
    // Draw and detect clicks on chest items
    for (int i = 0; i < chest->storage.itemCount; i++) {
        int itemX = uiX + 20;
        int itemY = uiY + 50 + i * 20;
        DrawText(TextFormat("Item %d: %d", i, chest->storage.items[i]), itemX, itemY, 18, WHITE);

        // Check if mouse clicked an item
        if (CheckCollisionPointRec(mousePos, (Rectangle){itemX, itemY, 100, 20}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            RetrieveItemFromChest(openedChestRow, openedChestCol, &playerInventory);
        }
    }

    // Draw and detect clicks on inventory items
    for (int i = 0; i < playerInventory.itemCount; i++) {
        int itemX = uiX + 160;
        int itemY = uiY + 50 + i * 20;
        DrawText(TextFormat("Inv %d: %d", i, playerInventory.items[i]), itemX, itemY, 18, WHITE);

        if (CheckCollisionPointRec(mousePos, (Rectangle){itemX, itemY, 100, 20}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            StoreItemInChest(openedChestRow, openedChestCol, playerInventory.items[i], &playerInventory);
        }
    }
}


bool StoreItemInChest(int row, int col, int itemID, Inventory *playerInventory) {
    if (!IsChest(objects[row][col])) return false;

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
    if (!IsChest(objects[row][col])) return false;

    Chest *chest = &chestData[row][col];

    if (chest->storage.itemCount == 0 || playerInventory->itemCount >= MAX_ITEMS_IN_CHEST) return false;

    int itemID = chest->storage.items[--chest->storage.itemCount];
    playerInventory->items[playerInventory->itemCount++] = itemID;

    return true;
}
