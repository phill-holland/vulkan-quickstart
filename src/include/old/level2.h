#include "legacy_level.h"

#ifndef _LEVEL2
#define _LEVEL2

class level2 : public legacy::level
{
public:
{
	level2()
	{
		blockDropSpeed = 1; // not used at moment !!!! - need some sort of block drop delay !!
		blockDropInterval = 3; // every 5 frames move block once !

		chanceOfBlock = 50;
		chanceOfBonusBlock = 10; // was 10
		chanceOfFireBlock = 5;

		bonusBlockTimeout = 500;

		levelTimeoutMin = 3;
		levelTimeoutSec = 30;

		returnFireVerticalSpeed = 1; // may need interval too !!
		userFireVerticalSpeed = 1;

		passThroughTimeout = 20;

		verticalStopPoint = 50;

		totalDoorwayBanks = 1;
		doorwayBanks = new doorwayBank[totalDoorwayBanks];
		
		// totalDoors,SeperatorHeight,x,y,passTimeout,shuffleEntrances,shuffleExists,shuffleExtrancesOrExits
		doorwayBanks[0] = new doorwayBank(4,20,18,100,true,true,false);
	}
};

#endif