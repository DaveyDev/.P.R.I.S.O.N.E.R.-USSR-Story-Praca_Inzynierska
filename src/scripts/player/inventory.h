#ifndef INVENTORY_H
#define INVENTORY_H

#include "../../../lib/raylib.h"

#define INVENTORY_SIZE 6

typedef struct {
    int id;
    char name[20];
    int quantity;
} InventoryItem;

extern InventoryItem inventory[INVENTORY_SIZE]; // Declare as extern
extern InventoryItem selectedItem;

void initInventory();
int addItemToInventory(int id, const char *name);
void saveInventory();
void loadInventory();
void drawInventory();
void handleInventoryClick(); // Handles clicking on inventory slots
void placeSelectedItem(Camera2D camera); // Places the selected item on the ground
void drawSelectedItem();


#endif
