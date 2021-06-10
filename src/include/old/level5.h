#include "legacy_level.h"

#ifndef _LEVEL5
#define _LEVEL5

class level5 : public legacy::level
{
public:
{
	level5()
	{
		blockDropSpeed = 1; // not used at moment !!!! - need some sort of block drop delay !!
		blockDropInterval = 3; // every 5 frames move block once !

		chanceOfBlock = 50;
		chanceOfBonusBlock = 5; // was 10
		chanceOfFireBlock = 5;

		bonusBlockTimeout = 500;

		levelTimeoutMin = 4;
		levelTimeoutSec = 30;

		returnFireVerticalSpeed = 1; // may need interval too !!
		userFireVerticalSpeed = 1;

		passThroughTimeout = 10;

		verticalStopPoint = 50;

		totalDoorwayBanks = 2;
		doorwayBanks = new doorwayBank[totalDoorwayBanks];
		
		// totalDoors,SeperatorHeight,x,y,passTimeout,shuffleEntrances,shuffleExists,shuffleExtrancesOrExits
		doorwayBanks[0] = new doorwayBank(6,15,13,130,false,true,false);
		doorwayBanks[1] = new doorwayBank(6,15,13,80,false,true,false);	
	}
};

#endif

