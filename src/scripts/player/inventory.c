#include "../../../lib/raylib.h"
#include "../../../lib/raymath.h"
#include "../map/objects.h"
#include <stdio.h>
#include <string.h>
#include "inventory.h"
#include "../items.h"
#include "../textures.h"

#define INVENTORY_SIZE 5

InventoryItem inventory[INVENTORY_SIZE]; 
InventoryItem selectedItem = { .id = -1, .quantity = 0 };



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
            printf("Inventory slot %d filled with item %d (%s)\n", i, id, name);
            return 1; // Successfully added
        }
    }
    printf("Failed to add item to inventory: No space\n");
    return 0; // Inventory full
}

void saveInventory() {
    FILE *file = fopen("data/saves/inventory.dat", "w");
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
    FILE *file = fopen("data/saves/inventory.dat", "r");
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
            int itemsWidth = 32;
            int itemsHeight = 32;
            int itemsPerRow = itemsSet.width / itemsWidth;
            int itemIndex = inventory[i].id - 3000; // Assuming IDs start at 3000

            int row = itemIndex / itemsPerRow;
            int col = itemIndex % itemsPerRow;

            Rectangle itemSource = { col * itemsWidth, row * itemsHeight, itemsWidth, itemsHeight };
            Rectangle itemDest = { slot.x + 5, slot.y + 5, slotSize - 10, slotSize - 10 };

            DrawTexturePro(itemsSet, itemSource, itemDest, (Vector2){0, 0}, 0.0f, WHITE);

            // Display quantity in the bottom-right corner of the slot
            if (inventory[i].quantity > 1) {
                char quantityText[8];
                snprintf(quantityText, sizeof(quantityText), "x%d", inventory[i].quantity);
                DrawText(quantityText, slot.x + slotSize - 20, slot.y + slotSize - 15, 14, BLACK);
            }
        }
    }
}

void handleInventoryClick() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int slotSize = 60;
        int spacing = 10;
        int startX = (GetScreenWidth() - (INVENTORY_SIZE * (slotSize + spacing))) / 2;
        int startY = GetScreenHeight() - 80;

        for (int i = 0; i < INVENTORY_SIZE; i++) {
            Rectangle slot = { startX + i * (slotSize + spacing), startY, slotSize, slotSize };

            if (CheckCollisionPointRec(mousePos, slot)) {
                if (selectedItem.id == -1 && inventory[i].id != -1) {
                    // Pick up the item
                    selectedItem = inventory[i];
                    inventory[i].quantity--;
                    if (inventory[i].quantity <= 0) inventory[i].id = -1;
                } else if (selectedItem.id != -1) {
                    // Return item to inventory if clicked again
                    addItemToInventory(selectedItem.id, selectedItem.name);
                    selectedItem.id = -1;
                }
                return;
            }
        }
    }
}

/*
void placeSelectedItem(Camera2D camera) {
    if (selectedItem.id != -1 && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

        if (itemCount < MAX_ITEMS) {
            items[itemCount].id = selectedItem.id;
            items[itemCount].quantity = 1;
            items[itemCount].itemPos = worldPos;
            itemCount++;

            selectedItem.id = -1; // Remove from hand
        }
    }
}*/
void placeSelectedItem(Camera2D camera) {
    if (selectedItem.id != -1 && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);

        // Check if an item of the same type is already there
        for (int i = 0; i < itemCount; i++) {
            if (items[i].id == selectedItem.id &&
                Vector2Distance(items[i].itemPos, worldPos) < 10.0f) {  
                // If close enough, stack items instead of placing separately
                items[i].quantity++;
                selectedItem.id = -1; // Remove from hand
                return;
            }
        }

        // If no stackable item found, place a new one
        if (itemCount < MAX_ITEMS) {
            items[itemCount].id = selectedItem.id;
            items[itemCount].quantity = 1;
            items[itemCount].itemPos = worldPos;
            itemCount++;

            selectedItem.id = -1; // Remove from hand
        }
    }
}

void drawSelectedItem() {
    if (selectedItem.id != -1) {
        Vector2 mousePos = GetMousePosition();

        int itemsWidth = 32;
        int itemsHeight = 32;
        int itemsPerRow = itemsSet.width / itemsWidth;
        int itemIndex = selectedItem.id - 3000; 

        int row = itemIndex / itemsPerRow;
        int col = itemIndex % itemsPerRow;

        Rectangle itemSource = { col * itemsWidth, row * itemsHeight, itemsWidth, itemsHeight };
        Rectangle itemDest = { mousePos.x - 16, mousePos.y - 16, itemsWidth, itemsHeight };

        DrawTexturePro(itemsSet, itemSource, itemDest, (Vector2){0, 0}, 0.0f, WHITE);
    }
}
