#ifndef ITEMS_H
#define ITEMS_H

#include "../../lib/raylib.h"

#define MAX_ITEMS 100
#define NAME_LEN 50


//item struct
typedef struct {
    int id;
    char itemName[NAME_LEN];
    int quantity;
    Vector2 itemPos;
    
    
} Item;


#endif