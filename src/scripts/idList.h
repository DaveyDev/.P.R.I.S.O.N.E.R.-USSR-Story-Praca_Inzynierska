//Wiktor
#ifndef IDLIST_H
#define IDLIST_H

// Define your tile types with unique IDs
typedef enum {

    //PLACEABLE
    STONE = 2000,
    IRON_ORE = 2001,
    COAL_ORE = 2002,
    GOLD_ORE = 2003,
    URANIUM_ORE = 2004,

    GREY_DOOR = 2016,
    LIGHTGREY_DOOR = 2017,

    WOODEN_FLOOR = 2032,
    STONE_FLOOR = 2033,
    
    //ITEMS
    BANKNOTE = 3000,
    AID_KIT = 3001,
    SOAP = 3002,
    CIGARETTES = 3003,
    BASIC_SOUP = 3004,
    BOTTLE_OF_VODKA = 3005,
    BREAD = 3006,
    SHOVEL = 3007,
    AXE = 3008,
    DAGGER = 3009,
    PICKAXE = 3010,

    // Add more as needed
} idList;

#endif /* IDLIST_H */
