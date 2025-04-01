#include "storage.h"
#include "../../../lib/raylib.h"
#include <stdio.h>

Chest chests[MAX_CHESTS];

void InitChests() {
    for (int i = 0; i < MAX_CHESTS; i++) {
        chests[i].position = (Vector2){50 + i * 100, 50}; // Example placement
        chests[i].storage.itemCount = 0;
        chests[i].isOpen = false;
    }
}

void OpenChest(int chestIndex) {
    if (chestIndex < 0 || chestIndex >= MAX_CHESTS) return;
    chests[chestIndex].isOpen = !chests[chestIndex].isOpen;
}

bool StoreItemInChest(int chestIndex, int itemID, Inventory *playerInventory) {
    if (chestIndex < 0 || chestIndex >= MAX_CHESTS) return false;
    Chest *chest = &chests[chestIndex];

    if (chest->storage.itemCount >= MAX_ITEMS) return false; // Chest full

    // Add item to chest
    chest->storage.items[chest->storage.itemCount++] = itemID;

    // Remove item from player inventory
    for (int i = 0; i < playerInventory->itemCount; i++) {
        if (playerInventory->items[i] == itemID) {
            playerInventory->items[i] = playerInventory->items[--playerInventory->itemCount];
            return true;
        }
    }
    return false;
}

bool RetrieveItemFromChest(int chestIndex, Inventory *playerInventory) {
    if (chestIndex < 0 || chestIndex >= MAX_CHESTS) return false;
    Chest *chest = &chests[chestIndex];

    if (chest->storage.itemCount == 0 || playerInventory->itemCount >= MAX_ITEMS) 
        return false; // No items or player's inventory is full

    // Transfer the last item
    int itemID = chest->storage.items[--chest->storage.itemCount];
    playerInventory->items[playerInventory->itemCount++] = itemID;

    return true;
}

void DrawChests() {
    for (int i = 0; i < MAX_CHESTS; i++) {
        Color color = chests[i].isOpen ? DARKGRAY : BROWN;
        DrawRectangleV(chests[i].position, (Vector2){32, 32}, color);
    }
}

void DrawChestContents(int chestIndex) {
    if (!chests[chestIndex].isOpen) return;

    for (int i = 0; i < chests[chestIndex].storage.itemCount; i++) {
        DrawText(TextFormat("Item %d: %d", i, chests[chestIndex].storage.items[i]), 50, 100 + (i * 20), 20, WHITE);
    }
}

void CheckChestInteraction(Vector2 playerPos) {
    for (int i = 0; i < MAX_CHESTS; i++) {
        if (CheckCollisionPointRec(playerPos, (Rectangle){chests[i].position.x, chests[i].position.y, 32, 32})) {
            if (IsKeyPressed(KEY_E)) OpenChest(i);
        }
    }
}
