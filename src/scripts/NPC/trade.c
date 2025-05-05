#include "trade.h"
#include "../items/idList.h"

TradePair tradeList[] = {
    // Basic consumables trades (loop)
    { SOAP, BREAD },
    { BREAD, BASIC_SOUP },
    { BASIC_SOUP, AID_KIT },
    { AID_KIT, BOTTLE_OF_VODKA },
    { BOTTLE_OF_VODKA, CIGARETTES },
    { CIGARETTES, SOAP },

    // Reverse consumables
    { BREAD, SOAP },
    { BASIC_SOUP, BREAD },
    { AID_KIT, BASIC_SOUP },
    { BOTTLE_OF_VODKA, AID_KIT },
    { CIGARETTES, BOTTLE_OF_VODKA },
    { SOAP, CIGARETTES },

    // Materials progression (loopable)
    { STONE_ITEM, WOOD_LOG },
    { WOOD_LOG, COAL },
    { COAL, IRON_NUGGET },
    { IRON_NUGGET, COPPER_NUGGET },
    { COPPER_NUGGET, GOLD_NUGGET },
    { GOLD_NUGGET, URANIUM_NUGGET },

    // Reverse materials
    { WOOD_LOG, STONE_ITEM },
    { COAL, WOOD_LOG },
    { IRON_NUGGET, COAL },
    { COPPER_NUGGET, IRON_NUGGET },
    { GOLD_NUGGET, COPPER_NUGGET },
    { URANIUM_NUGGET, GOLD_NUGGET },

    // Tools and weapons progression
    { STONE_ITEM, SHOVEL },
    { SHOVEL, AXE },
    { AXE, PICKAXE },
    { PICKAXE, DAGGER },

    // Reverse tools
    { SHOVEL, STONE_ITEM },
    { AXE, SHOVEL },
    { PICKAXE, AXE },
    { DAGGER, PICKAXE },

    // Resource-to-utility cross trades
    { IRON_NUGGET, SHOVEL },
    { COPPER_NUGGET, SOAP },
    { GOLD_NUGGET, AID_KIT },
    { URANIUM_NUGGET, HANDY_STALIN_PIC },

    // Special items path
    { HANDY_STALIN_PIC, DAGGER },
    { DAGGER, OLD_FILE },

    // Reverse special trades
    { OLD_FILE, DAGGER },
    { DAGGER, HANDY_STALIN_PIC },
    { HANDY_STALIN_PIC, GOLD_NUGGET },

    // Desperate / trash trades (risky but possible)
    { SOAP, STONE_ITEM },
    { WOOD_LOG, SOAP },
    { STONE_ITEM, CIGARETTES },
    { COAL, SOAP },

};


const int tradeListSize = sizeof(tradeList) / sizeof(tradeList[0]);
