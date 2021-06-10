#include <vector>
#include "json.h"
#include "vector.h"
#include "gate.h"

#ifndef _LEVEL
#define _LEVEL

class level
{
    int blockDropSpeed;
    int blockDropInterval;

    int chanceOfBlock;
    int chanceOfBonusBlock;
    int chanceOfFireBlock;
    int chanceOfPersistBlock;

    int persistCount;

    int bonusBlockTimeout;

    int levelTimeoutMin;
    int levelTimeoutSec;

    int returnFireVerticalSpeed;
    int returnFireInterval;

    int userFireVerticalSpeed;

    int verticalStopPoint;

    int blockSpacing;

    int passThroughTimeout;

    int bonusPoints;
    int userAllowedFire;

    int maxBlockDropSpeed;
    int maxUserFireVerticalSpeed;

    vector gravity;

    std::vector<gate> gates;

private:
    bool init;

public:
    level() { makeNull(); reset(); }
    ~level() { cleanup(); }

    bool initalised() { return init; }
    void reset();

protected:
    void makeNull() { }
    void cleanup() { }
};

#endif