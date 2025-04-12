#include "../../../lib/raylib.h"
#include "../../../lib/raymath.h"
#include "../map/objects.h"
#include <stdio.h>
#include <string.h>
#include "inventory.h"
#include "../items/items.h"
#include "../textures.h"
#include "../items/storage.h"
#include "../items/idList.h"
#include "../sound/soundManager.h"


//#define INVENTORY_SIZE 5

InventoryItem inventory[INVENTORY_SIZE]; 
InventoryItem selectedItem = { .id = -1, .quantity = 0 };

int activeItemIndex = -1;



void initInventory() {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        inventory[i].id = -1; // -1 means empty slot
        strcpy(inventory[i].name, "");
        inventory[i].quantity = 0;
    }
}

bool eatItem(int index) {
    if (inventory[index].id == -1) return false;

    switch (inventory[index].id) {
        case BREAD:
            useFood(-4.0f);
            break;
        case CIGARETTES:
            useFood(-15.0f);
            player.health = player.health - 10;
            break;
        case BASIC_SOUP:
            useFood(-6.0f);
            break;
        case BOTTLE_OF_VODKA:
            useFood(-50.0f);
            player.health = 1.0f;
            break;
        case AID_KIT:
            //useFood(-50.0f);
            player.health = player.health + 30.0f;
            break;
        default:
            return false; // Not edible
    }

    inventory[index].quantity--;
    if (inventory[index].quantity <= 0) {
        inventory[index].id = -1;
    }

    printf("Ate item %d, remaining: %d\n", inventory[index].id, inventory[index].quantity);
    return true;
}

int addItemToInventory(int id, const char *name) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i].id == -1) { // Find first empty slot
            inventory[i].id = id;
            strncpy(inventory[i].name, name, sizeof(inventory[i].name) - 1);
            inventory[i].quantity = 1;
            printf("Inventory slot %d filled with item %d (%s)\n", i, id, name);
            playPickupItemSound();
            useFood(0.01f); // uses food each time player picks up item.
            return 1; // Successfully added
            
        }
    }
    printf("Failed to add item to inventory: No space\n");
    return 0; // Inventory full
}

void saveInventory() {
    FILE *file = fopen("data/saves/save1/inventory.dat", "w"); //just temporary 
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
    FILE *file = fopen("data/saves/save1/inventory.dat", "r"); //just temporary 
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
        //DrawRectangleRec(slot, GRAY);
        Color slotColor = (i == activeItemIndex) ? GREEN : GRAY;
        DrawRectangleRec(slot, slotColor);

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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int slotSize = 60;
        int spacing = 10;
        int startX = (GetScreenWidth() - (INVENTORY_SIZE * (slotSize + spacing))) / 2;
        int startY = GetScreenHeight() - 80;

        for (int i = 0; i < INVENTORY_SIZE; i++) {
            Rectangle slot = { startX + i * (slotSize + spacing), startY, slotSize, slotSize };

            if (CheckCollisionPointRec(mousePos, slot)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Left click: Pick up or return item
                    if (selectedItem.id == -1 && inventory[i].id != -1) {
                        // Pick up the item
                        selectedItem = inventory[i];
                        inventory[i].quantity--;
                        if (inventory[i].quantity <= 0) inventory[i].id = -1;
                    } else if (selectedItem.id != -1) {
                        // Return item to inventory
                        addItemToInventory(selectedItem.id, selectedItem.name);
                        selectedItem.id = -1;
                    }
                } 
                else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && chestUIOpen) {
                    // Right click: Store item in the chest
                    if (inventory[i].id != -1) {
                        if (storeItemInChest(openedChestRow, openedChestCol, inventory[i].id, &playerInventory)) {
                            printf("Item stored in chest!\n");
                            inventory[i].quantity--;
                            if (inventory[i].quantity <= 0) inventory[i].id = -1;
                        } else {
                            printf("Chest is full!\n");
                        }
                    }
                } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !chestUIOpen) {
                    if (inventory[i].id != -1) {
                        if (!eatItem(i)) {
                            // Not edible, so treat it like a tool toggle
                            if (activeItemIndex == i) {
                                activeItemIndex = -1;
                            } else {
                                activeItemIndex = i;
                            }
                        }
                    }
                }



                return;
            }
        }
    }
}



void placeSelectedItem(Camera2D camera) {
    if (selectedItem.id != -1 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
        worldPos.x = worldPos.x - 8; //offset so placing items feels more natural
        worldPos.y = worldPos.y - 8;

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

void storeSelectedItem(int selectedItemIndex, Inventory *playerInventory) {
    if (!chestUIOpen) return;  // No chest is open, can't store

    int itemID = playerInventory->items[selectedItemIndex];  // Get item from inventory

    // Try to store the item in the chest at the opened location
    if (storeItemInChest(openedChestRow, openedChestCol, itemID, playerInventory)) {
        printf("Item stored successfully!\n");
    } else {
        printf("Chest is full!\n");
    }
}
void tryUseActiveItem(Vector2 worldPos) {
    if (activeItemIndex == -1) return;

    int itemId = inventory[activeItemIndex].id;

    int col = (int)(worldPos.x / 32);
    int row = (int)(worldPos.y / 32);

    if (row < 0 || row >= rows || col < 0 || col >= cols) return;

    int obj = objects[row][col];

    // 🪓 Axe logic (chop trees)
    if (itemId == AXE) {
        if (obj >= 1000 && obj <= 1999 && obj != TREE_STUMP) { // Tree range
            printf("Tree chopped at tile (%d, %d)!\n", col, row);

            if (itemCount < MAX_ITEMS) {
                items[itemCount].id = WOOD_LOG;
                items[itemCount].quantity = 1;
                items[itemCount].itemPos = (Vector2){ col * 32, row * 32 };
                itemCount++;
            }

            objects[row][col] = TREE_STUMP;
            return;
        }
        if (obj == TREE_STUMP) { // Tree range
            printf("Tree chopped at tile (%d, %d)!\n", col, row);

            if (itemCount < MAX_ITEMS) {
                items[itemCount].id = WOOD_LOG;
                items[itemCount].quantity = 1;
                items[itemCount].itemPos = (Vector2){ col * 32, row * 32 };
                itemCount++;
            }

            objects[row][col] = 0;
            return;
        }
    }

    // ⛏️ Pickaxe logic (mine rocks/ores)
    if (itemId == PICKAXE) {
        int dropItem = -1;

        switch (obj) {
            case SIMPLE_STONE: dropItem = STONE_ITEM; break;
            case STONE:        dropItem = STONE_ITEM; break;
            case COPPER_ORE:   dropItem = COPPER_NUGGET; break;
            case COAL_ORE:     dropItem = COAL; break;
            case IRON_ORE:     dropItem = IRON_NUGGET; break;
            case URANIUM_ORE:  dropItem = URANIUM_NUGGET; break;
            case GOLD_ORE:     dropItem = GOLD_NUGGET; break;
            default: break;
        }

        if (dropItem != -1) {
            printf("Mined %d at tile (%d, %d)\n", dropItem, col, row);

            if (itemCount < MAX_ITEMS) {
                items[itemCount].id = dropItem;
                items[itemCount].quantity = 1;
                items[itemCount].itemPos = (Vector2){ col * 32, row * 32 };
                itemCount++;
            }

            objects[row][col] = 0;
            return;
        }
    }
    // ⛏️ Pickaxe logic (mine rocks/ores)
    if (itemId == OLD_FILE) {
        int dropItem = -1;

        switch (obj) {
            case BARBED_WIRE: dropItem = 0; break;
            case BARBED_WIRE_VERTICAL: dropItem = 0; break;
            default: break;
        }

        if (dropItem != -1) {
            printf("wire fence at tile (%d, %d) was cut\n", col, row);

            objects[row][col] = 0;
            return;
        }
    }
}






