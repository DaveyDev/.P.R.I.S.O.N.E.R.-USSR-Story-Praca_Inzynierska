#include "storage.h"
#include "../../../lib/raylib.h"
#include <stdio.h>
#include "idList.h"
#include "../global.h"
#include "../player/inventory.h"
#include "../textures.h"
#include "items.h"


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

        
        if (CheckCollisionPointRec(mousePos, slot) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            retrieveItemFromChest(openedChestRow, openedChestCol, i, &playerInventory);
        }

    }
}

bool storeItemInChest(int row, int col, int itemID, Inventory *playerInventory) {
    if (!isChest(objects[row][col])) return false;

    Chest *chest = &chestData[row][col];

    for (int i = 0; i < MAX_ITEMS_IN_CHEST; i++) {
        if (chest->storage.items[i] == -1) {
            chest->storage.items[i] = itemID;
            chest->storage.itemCount++;
            return true;
        }
    }

    // No free slot
    printf("Chest is full!\n");
    return false;
}
bool retrieveItemFromChest(int row, int col, int slotIndex, Inventory *playerInventory) {
    if (!isChest(objects[row][col])) return false;

    Chest *chest = &chestData[row][col];

    if (slotIndex < 0 || slotIndex >= MAX_ITEMS_IN_CHEST) return false;

    int itemID = chest->storage.items[slotIndex];
    if (itemID == -1) return false;

    if (addItemToInventory(itemID, items[itemID].itemName)) {
        chest->storage.items[slotIndex] = -1;
        chest->storage.itemCount--;
        return true;
    } else {
        printf("Inventory full!\n");
        return false;
    }
}


void loadChests() {
    FILE *file = fopen("data/saves/save1/chests.dat", "r");
    if (!file) {
        fprintf(stderr, "Error loading chests\n");
        return;
    }

    int row, col, slotCount;
    while (fscanf(file, "%d:%d:%d:", &row, &col, &slotCount) == 3) {
        if (isChest(objects[row][col])) {
            Chest *chest = &chestData[row][col];
            chest->storage.itemCount = 0;

            // Clear all slots
            for (int i = 0; i < MAX_ITEMS_IN_CHEST; i++) {
                chest->storage.items[i] = -1;
            }

            // Read all slots
            for (int i = 0; i < slotCount; i++) {
                int itemID;
                fscanf(file, "%d,", &itemID);
                if (i < MAX_ITEMS_IN_CHEST) {
                    chest->storage.items[i] = itemID;
                    if (itemID != -1) chest->storage.itemCount++;
                }
            }
        }
    }

    fclose(file);
}

void saveChests(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error saving chests\n");
        return;
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (isChest(objects[row][col])) {
                Chest *chest = &chestData[row][col];

                // Recalculate itemCount based on valid items
                chest->storage.itemCount = 0;
                for (int i = 0; i < MAX_ITEMS_IN_CHEST; i++) {
                    if (chest->storage.items[i] != -1) {
                        chest->storage.itemCount++;
                    }
                }

                fprintf(file, "%d:%d:%d:", row, col, MAX_ITEMS_IN_CHEST);
                for (int i = 0; i < MAX_ITEMS_IN_CHEST; i++) {
                    fprintf(file, "%d,", chest->storage.items[i]);
                }
                fprintf(file, "\n");
            }
        }
    }

    fclose(file);
}

