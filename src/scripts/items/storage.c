#include "storage.h"
#include "../../../lib/raylib.h"
#include <stdio.h>
#include "idList.h"
#include "../global.h"
#include "../player/inventory.h"
#include "../textures.h"


Inventory playerInventory;
Chest chestData[100][100];

int openedChestRow = -1, openedChestCol = -1; // Track opened chest position
bool chestUIOpen = false; // Is the chest window open?

bool isChest(int objectID) {
    return objectID == CABINET;
}

void updateChests(int **map, int mapRows, int mapCols, Camera2D camera) {
    int row = player.position.y / TILE_SIZE;
    int col = player.position.x / TILE_SIZE;

    if (row < 0 || col < 0 || row >= mapRows || col >= mapCols) return;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);

        int clickedCol = (int)(worldMousePos.x / TILE_SIZE);
        int clickedRow = (int)(worldMousePos.y / TILE_SIZE);

        if (clickedRow >= 0 && clickedCol >= 0 && clickedRow < mapRows && clickedCol < mapCols) {
            if (isChest(objects[clickedRow][clickedCol])) {
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




/*
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
*/ 

void drawChestUI() {
    if (!chestUIOpen || openedChestRow == -1 || openedChestCol == -1) return;

    int slotSize = 60;
    int spacing = 10;
    int rows = 2, cols = 2;
    int chestWidth = cols * (slotSize + spacing) - spacing;
    int chestHeight = rows * (slotSize + spacing) - spacing;
    
    int startX = (GetScreenWidth() - chestWidth) / 2;
    int startY = (GetScreenHeight() - chestHeight) / 2;

    Chest *chest = &chestData[openedChestRow][openedChestCol];

    DrawRectangle(startX - 10, startY - 10, chestWidth + 20, chestHeight + 20, DARKGRAY);
    DrawRectangleLines(startX - 10, startY - 10, chestWidth + 20, chestHeight + 20, WHITE);
    DrawText("Chest", startX + chestWidth / 2 - 30, startY - 30, 20, YELLOW);

    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < 4; i++) {
        int row = i / cols;
        int col = i % cols;
        int slotX = startX + col * (slotSize + spacing);
        int slotY = startY + row * (slotSize + spacing);

        Rectangle slot = { slotX, slotY, slotSize, slotSize };
        DrawRectangleRec(slot, GRAY);
        DrawRectangleLines(slot.x, slot.y, slot.width, slot.height, DARKGRAY);

        if (chest->storage.items[i] != -1) {
            int itemID = chest->storage.items[i];
            int itemsWidth = 32, itemsHeight = 32;
            int itemsPerRow = itemsSet.width / itemsWidth;
            int itemIndex = itemID - 3000;

            int itemRow = itemIndex / itemsPerRow;
            int itemCol = itemIndex % itemsPerRow;

            Rectangle itemSource = { itemCol * itemsWidth, itemRow * itemsHeight, itemsWidth, itemsHeight };
            Rectangle itemDest = { slot.x + 5, slot.y + 5, slotSize - 10, slotSize - 10 };

            DrawTexturePro(itemsSet, itemSource, itemDest, (Vector2){0, 0}, 0.0f, WHITE);
        }

        // Clicking to retrieve item
        if (CheckCollisionPointRec(mousePos, slot) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            retrieveItemFromChest(openedChestRow, openedChestCol, &playerInventory);
        }
    }
}

/*
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
*/
bool storeItemInChest(int row, int col, int itemID, Inventory *playerInventory) {
    if (!isChest(objects[row][col])) return false;

    Chest *chest = &chestData[row][col];

    // Find the first empty slot and store the item
    for (int i = 0; i < MAX_ITEMS_IN_CHEST; i++) {
        if (chest->storage.items[i] == -1) {  // Empty slot found
            chest->storage.items[i] = itemID;

            // Remove *one* instance of the item from player inventory
            for (int j = 0; j < playerInventory->itemCount; j++) {
                if (playerInventory->items[j] == itemID) {
                    // Move last item to this slot and reduce count
                    playerInventory->items[j] = playerInventory->items[playerInventory->itemCount - 1];
                    playerInventory->itemCount--;
                    return true;  // Exit after successfully storing 1 item
                }
            }
            return false;  // If item wasn't found in player inventory
        }
    }
    return false;  // No empty slots available
}


bool retrieveItemFromChest(int row, int col, Inventory *playerInventory) {
    if (!isChest(objects[row][col])) return false;

    Chest *chest = &chestData[row][col];

    for (int i = 0; i < 4; i++) {
        if (chest->storage.items[i] != -1) { // Has an item
            int itemID = chest->storage.items[i];
            chest->storage.items[i] = -1; // Remove from chest

            // Add to player inventory if space available
            if (playerInventory->itemCount < MAX_ITEMS_IN_CHEST) {
                playerInventory->items[playerInventory->itemCount++] = itemID;
                return true;
            }
        }
    }
    return false;
}
