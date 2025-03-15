//Wiktor
#ifndef ITEMS_H
#define ITEMS_H

#include "../../lib/raylib.h"

#define MAX_ITEMS 1024
#define NAME_LEN 50


//item struct
typedef struct {
    int id;
    char itemName[NAME_LEN];
    int quantity;
    Vector2 itemPos;
    
    
} Item;

// Function declarations
void loadItems(const char *filename);  // Load items from items.dat
void drawItems(Camera2D camera);  // Draw items on screen
void updateItems(Camera2D camera);
void saveItems();


#endif