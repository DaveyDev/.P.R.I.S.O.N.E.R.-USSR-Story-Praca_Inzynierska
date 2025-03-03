//Wiktor
#include "../../lib/raylib.h"
#include "../../lib/raygui.h"
#include "scenes.h"
#include <stdio.h>
#include "../scripts/global.h"
#include "../scripts/map/map.h"
#include "../scripts/translation.h"
#include "../scripts/items.h"
int getTotalItems(){
    return 5;
}
char* getItemName(int i){
    return "test";
}
// Function to display inventory
void inventoryScene() {
    ClearBackground(bgColor);

    // Draw inventory title
    const char* inventoryTitle = getTranslation("inventory_title");  // Replace with actual translation function
    int fontSize = 32;
    DrawText(inventoryTitle, GetScreenWidth() / 2 - MeasureText(inventoryTitle, fontSize) / 2, 50, fontSize, WHITE);

    // Example: Display inventory items
    int startY = 100;
    int itemHeight = 40;
    for (int i = 0; i < getTotalItems(); i++) {  // Assuming you have a function like getTotalItems() to get the number of items
        Rectangle itemRect = { GetScreenWidth() / 2 - 150, startY + i * (itemHeight + 10), 300, itemHeight };
        char* itemName = getItemName(i);  // Replace with function to get item name
        if (GuiButton(itemRect, itemName)) {
            //useItem(i);  // Example function to use or interact with the item
        }
    }

    // Example: Button to close inventory
    Rectangle closeBtn = { GetScreenWidth() / 2 - 100, GetScreenHeight() - 100, 200, 50 };
    if (GuiButton(closeBtn, getTranslation("inventory_close"))) {
        // Close inventory scene and return to game
        *currentScene = GAME;  // Assuming currentScene is a pointer to manage scene transitions
    }
}


