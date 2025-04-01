#ifndef STORAGE_H
#define STORAGE_H

#include "../../../lib/raylib.h"

#define MAX_ITEMS 10
#define MAX_CHESTS 5

typedef struct {
    int items[MAX_ITEMS]; // Item IDs
    int itemCount;
} Inventory;

typedef struct {
    Vector2 position;
    Inventory storage;
    bool isOpen;
} Chest;

extern Chest chests[MAX_CHESTS];

void InitChests();
void OpenChest(int chestIndex);
bool StoreItemInChest(int chestIndex, int itemID, Inventory *playerInventory);
bool RetrieveItemFromChest(int chestIndex, Inventory *playerInventory);
void DrawChests();
void DrawChestContents(int chestIndex);
void CheckChestInteraction(Vector2 playerPos);

#endif // STORAGE_H
