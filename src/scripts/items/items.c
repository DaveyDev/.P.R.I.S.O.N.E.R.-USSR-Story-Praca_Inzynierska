#include "items.h"
#include "../player/inventory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../textures.h"

Item items[MAX_ITEMS];  // Define the actual storage
int itemCount = 0;  

void pickUpItem(int index) {
    if (index < 0 || index >= itemCount) return;

    printf("Attempting to pick up item: %d (%s) at index %d\n", items[index].id, items[index].itemName, index);

    if (addItemToInventory(items[index].id, items[index].itemName)) {
        printf("Item added to inventory successfully!\n");

        // Shift the remaining items in the array to "remove" it
        for (int i = index; i < itemCount - 1; i++) {
            items[i] = items[i + 1];
        }

        itemCount--; // Reduce item count
        printf("Item removed from the map. Remaining items: %d\n", itemCount);
    } else {
        printf("Inventory is full. Cannot pick up item.\n");
    }
}

void updateItems(Camera2D camera) {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < itemCount; i++) {
            Rectangle itemBounds = { items[i].itemPos.x, items[i].itemPos.y, 16, 16 };
            if (CheckCollisionPointRec(worldMousePos, itemBounds)) {
                // Try adding the item to the inventory
                if (addItemToInventory(items[i].id, "Item")) {
                    // If successful, reduce quantity or remove it
                    items[i].quantity--;
                    if (items[i].quantity <= 0) {
                        // Remove from map by shifting remaining items
                        for (int j = i; j < itemCount - 1; j++) {
                            items[j] = items[j + 1];
                        }
                        itemCount--; 
                    }
                }
                return;
            }
        }
    }
}



void saveItems() {
    FILE *file = fopen("data/saves/save1/items.dat", "w");
    if (!file) {
        fprintf(stderr, "Error saving items\n");
        return;
    }
    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d:%d:%f:%f:%s\n",
                items[i].id, items[i].quantity,
                items[i].itemPos.x, items[i].itemPos.y,
                items[i].itemName);
    }
    fclose(file);
}

void saveItemsE(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error saving items\n");
        return;
    }
    for (int i = 0; i < itemCount; i++) {
        fprintf(file, "%d:%d:%f:%f:%s\n",
                items[i].id, items[i].quantity,
                items[i].itemPos.x, items[i].itemPos.y,
                items[i].itemName);
    }
    fclose(file);
}


void loadItems(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    char line[100]; // Assuming max line length is 100 characters

    while (fgets(line, sizeof(line), file)) {
        if (itemCount >= MAX_ITEMS) {
            fprintf(stderr, "Maximum number of items exceeded.\n");
            break;
        }

        // Parse line into id, quantity, posx, posy
        int id, quantity;
        float posx, posy;
        if (sscanf(line, "%d:%d:%f:%f", &id, &quantity, &posx, &posy) != 4) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }

        // Assign values to the current item in the array
        items[itemCount].id = id;
        items[itemCount].quantity = quantity;
        items[itemCount].itemPos.x = posx;
        items[itemCount].itemPos.y = posy;

        // Read item name (assuming it's the rest of the line after coordinates)
        char *nameStart = strchr(line, ' ');
        if (nameStart != NULL) {
            strncpy(items[itemCount].itemName, nameStart + 1, NAME_LEN - 1); // Copy name, excluding space
            items[itemCount].itemName[NAME_LEN - 1] = '\0'; // Ensure null termination
        } else {
            fprintf(stderr, "Error parsing item name.\n");
            strncpy(items[itemCount].itemName, "Unknown", NAME_LEN); // Default name
        }

        itemCount++;
    }

    fclose(file);
}


// Function to draw items on the screen
// Function to draw items on the screen relative to the camera
void drawItems(Camera2D camera) {
    for (int i = 0; i < itemCount; i++) {
        Vector2 screenPos = GetWorldToScreen2D(items[i].itemPos, camera);

        int itemsWidth = 32;
        int itemsHeight = 32;
        int itemsPerRow = itemsSet.width / itemsWidth;
        int itemIndex = items[i].id - 3000;

        int row = itemIndex / itemsPerRow;
        int col = itemIndex % itemsPerRow;

        Rectangle itemSource = { col * itemsWidth, row * itemsHeight, itemsWidth, itemsHeight };
        Rectangle itemDest = { screenPos.x, screenPos.y, itemsWidth, itemsHeight };

        DrawTexturePro(itemsSet, itemSource, itemDest, (Vector2){0, 0}, 0.0f, WHITE);

        // If stackable, draw quantity text
        if (items[i].quantity > 1) {
            char quantityText[8];
            snprintf(quantityText, sizeof(quantityText), "x%d", items[i].quantity);
            DrawText(quantityText, screenPos.x + 10, screenPos.y + 10, 14, BLACK);
        }
    }
}

void addItem(Vector2 position, int itemID, int quantity, const char *name) {
    if (itemCount >= MAX_ITEMS) {
        printf("Cannot add more items, inventory is full!\n");
        return;
    }

    items[itemCount].id = itemID;
    items[itemCount].quantity = quantity;
    items[itemCount].itemPos = position;
    strncpy(items[itemCount].itemName, name, NAME_LEN - 1);
    items[itemCount].itemName[NAME_LEN - 1] = '\0'; // Ensure null termination

    printf("Added item %d (%s) at (%.2f, %.2f)\n", itemID, name, position.x, position.y);

    itemCount++;
}

//Hubert
void drawItemEditor(int itemIndex, Rectangle block) {
    int itemWidth = 32;  // Width of the tree in the texture atlas
    int itemHeight = 32; // Height of the tree in the texture atlas
    int itemsPerRow = itemsSet.width / itemWidth;

    // Calculate the x and y coordinates in the atlas
    int row = itemIndex / itemsPerRow;
    int col = itemIndex % itemsPerRow;

    Rectangle itemSource = { col * itemWidth, row * itemHeight, itemWidth, itemHeight };
    DrawTextureRec(itemsSet, itemSource, (Vector2){ block.x, block.y}, WHITE);  // Adjust y offset as needed
}
