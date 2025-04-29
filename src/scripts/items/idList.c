#include "idList.h"

IdNamePair itemNamesByID[] = {
    { WOOD_LOG, "Wood" },
    { COAL, "Coal" },
    { IRON_NUGGET, "Iron Nugget" },
    { SHOVEL, "Shovel" },
    { PICKAXE, "Pickaxe" },
    { DAGGER, "Dagger" },
    { BANKNOTE, "Banknote" },
    { BREAD, "Bread" },
    // ...add more
};

const int FOOD_ITEM_IDS[] = {
    BASIC_SOUP,
    //BOTTLE_OF_VODKA,
    BREAD,
    //AID_KIT,
    CIGARETTES
};

const int NUM_FOOD_ITEMS = sizeof(FOOD_ITEM_IDS) / sizeof(int);

int itemNamesByIDCount = sizeof(itemNamesByID) / sizeof(itemNamesByID[0]);

const char* getNameFromId(int id) {
    for (int i = 0; i < itemNamesByIDCount; i++) {
        if (itemNamesByID[i].id == id)
            return itemNamesByID[i].name;
    }
    return "Unknown";
}
