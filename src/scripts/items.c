#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../lib/raylib.h"

#define MAX_ITEMS 100
#define NAME_LEN 50

typedef struct {
    int id;
    //char name[NAME_LEN];
    //int type;  // 0 = weapon, 1 = armor, 2 = consumable, etc.
    int value; // Can represent damage, defense, healing, etc.
    Vector2 itemPos;
} Item;
