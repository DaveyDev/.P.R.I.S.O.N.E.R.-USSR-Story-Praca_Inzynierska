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

int itemNamesByIDCount = sizeof(itemNamesByID) / sizeof(itemNamesByID[0]);

const char* getNameFromId(int id) {
    for (int i = 0; i < itemNamesByIDCount; i++) {
        if (itemNamesByID[i].id == id)
            return itemNamesByID[i].name;
    }
    return "Unknown";
}
