#include "../../../lib/raylib.h"
#include "../map/objects.h"

#define INVENTORY_SIZE 5

int getTotalItems(){
    return 5;
}
char* getItemName(int i){
    return "test";
}

void drawInventory() {
    int slotSize = 60; // Size of each slot
    int spacing = 10; // Spacing between slots
    int startX = (GetScreenWidth() - (INVENTORY_SIZE * (slotSize + spacing))) / 2;
    int startY = GetScreenHeight() - 80; // Position at the bottom of the screen

    //int startX = GetScreenWidth() - 80;
    //int startY = (GetScreenHeight() - (INVENTORY_SIZE * (slotSize + spacing))) / 2;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        Rectangle slot = { startX + i * (slotSize + spacing), startY, slotSize, slotSize };
        //Rectangle slot = { startX, startY  + i * (slotSize + spacing), slotSize, slotSize };
        DrawRectangleRec(slot, GRAY); // Draw slot background
        DrawRectangleLines(slot.x, slot.y, slot.width, slot.height, DARKGRAY); // Draw border

        // Draw item icon (assuming each item has an index in a tileset)
        if (i < getTotalItems()) {
            drawPlaceable(i, slot);
        }
    }
}