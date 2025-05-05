#include "trade.h"
#include "../items/idList.h"

TradePair tradeList[] = {
    { BREAD, BASIC_SOUP },
    { URANIUM_NUGGET, OLD_FILE },
    { COAL, BREAD },
    //{ ITEM_WOOD, ITEM_STONE },
    //{ ITEM_COAL, ITEM_IRON },
    //{ ITEM_KEY, ITEM_HEALTH_KIT },
    // Add more as needed
};

const int tradeListSize = sizeof(tradeList) / sizeof(tradeList[0]);
