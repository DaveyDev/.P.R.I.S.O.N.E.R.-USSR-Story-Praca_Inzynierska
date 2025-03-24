#ifndef INVENTORY_H
#define INVENTORY_H

#include "../../../lib/raylib.h"

#define INVENTORY_SIZE 5

typedef struct {
    int id;
    char name[20];
    int quantity;
} InventoryItem;

extern InventoryItem inventory[INVENTORY_SIZE]; // Declare as extern

void initInventory();
int addItemToInventory(int id, const char *name);
void saveInventory();
void loadInventory();
void drawInventory();

#endif
