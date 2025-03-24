#include "../../../lib/raylib.h"
#include "../map/objects.h"
#include <stdio.h>
#include <string.h>
#include "inventory.h"
#include "../items.h"

#define INVENTORY_SIZE 5

InventoryItem inventory[INVENTORY_SIZE]; 


void initInventory() {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        inventory[i].id = -1; // -1 means empty slot
        strcpy(inventory[i].name, "");
        inventory[i].quantity = 0;
    }
}

int addItemToInventory(int id, const char *name) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i].id == -1) { // Find first empty slot
            inventory[i].id = id;
            strncpy(inventory[i].name, name, sizeof(inventory[i].name) - 1);
            inventory[i].quantity = 1;
            return 1; // Successfully added
        }
    }
    return 0; // Inventory full
}

void saveInventory() {
    FILE *file = fopen("data/inventory.dat", "w");
    if (!file) {
        fprintf(stderr, "Error saving inventory\n");
        return;
    }
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i].id != -1) {
            fprintf(file, "%d:%s:%d\n", inventory[i].id, inventory[i].name, inventory[i].quantity);
        }
    }
    fclose(file);
}

void loadInventory() {
    FILE *file = fopen("data/inventory.dat", "r");
    if (!file) return;
    
    initInventory();
    while (!feof(file)) {
        int id, quantity;
        char name[20];
        if (fscanf(file, "%d:%19[^:]:%d\n", &id, name, &quantity) == 3) {
            addItemToInventory(id, name);
        }
    }
    fclose(file);
}

void drawInventory() {
    int slotSize = 60;
    int spacing = 10;
    int startX = (GetScreenWidth() - (INVENTORY_SIZE * (slotSize + spacing))) / 2;
    int startY = GetScreenHeight() - 80;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        Rectangle slot = { startX + i * (slotSize + spacing), startY, slotSize, slotSize };
        DrawRectangleRec(slot, GRAY);
        DrawRectangleLines(slot.x, slot.y, slot.width, slot.height, DARKGRAY);

        if (inventory[i].id != -1) {
            DrawText(inventory[i].name, slot.x + 5, slot.y + 5, 10, BLACK);
        }
    }
}
