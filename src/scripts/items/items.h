#ifndef ITEMS_H
#define ITEMS_H

#include "../../../lib/raylib.h"

#define MAX_ITEMS 100
#define NAME_LEN 20

typedef struct {
    int id;
    int quantity;
    Vector2 itemPos;
    char itemName[NAME_LEN];
} Item;

extern Item items[MAX_ITEMS]; // Declare as extern
extern int itemCount; // Declare as extern

void loadItems(const char *filename);
void drawItems(Camera2D camera);
void updateItems(Camera2D camera);
void saveItems();
void pickUpItem(int index);
void addItem(Vector2 position, int itemID, int quantity, const char *name);
void drawItemEditor(int itemIndex, Rectangle block);


#endif
