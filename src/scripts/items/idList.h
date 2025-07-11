//Wiktor
#ifndef IDLIST_H
#define IDLIST_H

// Define your tile types with unique IDs
typedef enum {

    //WALL = 11,
    WINNING_BLOCK = 12,
    LUMBER_WORK_CHEST = 13,
    PATROL_BLOCK = 14,
    SPAWN_BLOCK = 15,
    FOOD_WORK_BLOCK = 16,
    FOOD_TAKE_BLOCK = 17,
    FOOD_WORK_CHEST = 18,
    MINE_WORK_CHEST = 19,
    INMATE_SPAWN = 20,
    GUARD_SPAWN = 21,
    FREE_TIME_BLOCK = 22,
    ROLLCALL_BLOCK = 23,
    ROLLCALL_GUARD_BLOCK = 24,




    //TREES
    SPRUCE = 1000,
    BETONIARZ_SPRUCE = 1001,
    ASH = 1002,
    TREE_STUMP = 1003,

    
    //PLACEABLE
    SIMPLE_STONE = 2000,
    COPPER_ORE = 2001,
    COAL_ORE = 2002,
    IRON_ORE = 2003,
    URANIUM_ORE = 2004,
    STONE = 2005,
    GOLD_ORE = 2006,
    SMALL_TABLE = 2007,
    LEFT_TABLE = 2008,
    MIDDLE_TABLE = 2009,
    RIGHT_TABLE = 2010,
    FANCY_BED_UPPER = 2011,
    FANCY_BED_DOWN = 2012,
    WOODEN_FLOOR = 2013,
    STONE_FLOOR = 2014,
    STALIN_PIC = 2015,
    GREY_DOOR = 2016,
    LIGHTGREY_DOOR = 2017,
    OPEN_GREY_DOOR = 2018,
    OPEN_LIGHTGREY_DOOR = 2019,
    LOCKED_GREY_DOOR = 2020,
    LOCKED_LIGHTGREY_DOOR = 2021,
    STOOL = 2022,
    CABINET = 2023,
    GREEN_BED_UPPER = 2024,
    GREEN_BED_BOTTOM = 2025,
    DARKGREY_EMPTY = 2026,
    BARBED_WIRE = 2027,
    OAK_WOODEN_FLOOR = 2028,
    BARBED_WIRE_VERTICAL = 2029,
    USELESS_BED_UPPER = 2030,
    USELESS_BED_BOTTOM = 2031,
    WOODEN_BARREL = 2032,
    CHEST = 2033,
    IRON_BARS = 2034,
    IRON_BARS_VECTICAL = 2035,
    


    
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
    WOOD_LOG = 3011,
    COAL = 3012,
    IRON_NUGGET = 3013,
    COPPER_NUGGET = 3014,
    URANIUM_NUGGET = 3015,
    GOLD_NUGGET = 3016,
    REVOLVER = 3017,
    HANDY_STALIN_PIC = 3018,
    GREY_KEY = 3019,
    STONE_ITEM = 3020,
    OLD_FILE = 3021,
    LETTER = 3022,

    // Add more as needed
} idList;

typedef struct {
    int id;
    const char* name;
} IdNamePair;

// Just declarations — no values here!
extern const int FOOD_ITEM_IDS[];
extern const int NUM_FOOD_ITEMS;

// Declare, don't define
extern IdNamePair itemNamesByID[];
extern int itemNamesByIDCount;

const char* getNameFromId(int id);

#endif /* IDLIST_H */