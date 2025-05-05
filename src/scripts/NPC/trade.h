#ifndef TRADE_H
#define TRADE_H

typedef struct {
    int wantedItemID;
    int rewardItemID;
} TradePair;

extern TradePair tradeList[];
extern const int tradeListSize;

#endif