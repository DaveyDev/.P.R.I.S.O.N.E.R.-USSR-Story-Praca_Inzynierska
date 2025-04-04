#ifndef STORAGE_H
#define STORAGE_H

#include "../../../lib/raylib.h"
#include "../map/map.h"  // Include your map system header

#define MAX_ITEMS_IN_CHEST 10

typedef struct {
    int items[MAX_ITEMS_IN_CHEST];
    int itemCount;
} Inventory;

typedef struct {
    Inventory storage;
    bool isOpen;
} Chest;

extern Inventory playerInventory;
extern int openedChestRow, openedChestCol;
extern bool chestUIOpen;  

bool isChest(int objectID);
void updateChests(int **map, int mapRows, int mapCols, Camera2D camera);
void drawChestUI();
bool storeItemInChest(int row, int col, int itemID, Inventory *playerInventory);
bool retrieveItemFromChest(int row, int col, Inventory *playerInventory);

#endif // STORAGE_H
