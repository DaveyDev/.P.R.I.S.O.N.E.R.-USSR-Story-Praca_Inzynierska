#include "items.h"
#include "player/inventory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "textures.h"

Item items[MAX_ITEMS];  // Define the actual storage
int itemCount = 0;  

void pickUpItem(int index) {
    if (index < 0 || index >= itemCount) return;

    if (addItemToInventory(items[index].id, items[index].itemName)) {
        for (int i = index; i < itemCount - 1; i++) {
            items[i] = items[i + 1]; // Shift items left
        }
        itemCount--; // Reduce item count
    }
}

void updateItems(Camera2D camera) {
    Vector2 mousePos = GetMousePosition();
    Vector2 worldMousePos = GetScreenToWorld2D(mousePos, camera);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < itemCount; i++) {
            Rectangle itemBounds = { items[i].itemPos.x, items[i].itemPos.y, 16, 16 };
            if (CheckCollisionPointRec(worldMousePos, itemBounds)) {
                pickUpItem(i);
                break;
            }
        }
    }
}

void saveItems() {
    FILE *file = fopen("data/levels/test/items.dat", "w");
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
        // Convert world coordinates to screen coordinates
        Vector2 screenPos = GetWorldToScreen2D(items[i].itemPos, camera);


        int itemsWidth = 32;  // Width of the tree in the texture atlas
        int itemsHeight = 32; // Height of the tree in the texture atlas
        int itemsPerRow = itemsSet.width / itemsWidth;

        int itemIndex = items[i].id - 3000;
        // Calculate the x and y coordinates in the atlas
        int row = itemIndex / itemsPerRow;
        int col = itemIndex % itemsPerRow;


        int scale = 1; // Scale factor
        Rectangle itemSource = { col * itemsWidth, row * itemsHeight, itemsWidth, itemsHeight }; // Define the source rectangle
        Vector2 destPos = { screenPos.x, screenPos.y }; // Calculate scaled destination position
        Rectangle itemDest = { destPos.x, destPos.y, itemsWidth * scale, itemsHeight * scale }; // Define the destination rectangle with scaled width and height
        DrawTexturePro(itemsSet, itemSource, itemDest, (Vector2){0, 0}, 0.0f, WHITE); // Draw the scaled item
    }
}