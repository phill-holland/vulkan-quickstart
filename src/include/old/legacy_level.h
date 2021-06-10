#ifndef _LEVEL
#define _LEVEL

/* make loadable from disc */

namespace legacy
{
	class level
	{
	public:
		int blockDropSpeed;
		int blockDropInterval;

		int chanceOfBlock; // the lower the number, the bigger the chance !!
		int chanceOfBonusBlock;
		int chanceOfFireBlock;
		int chanceOfPersistBlock = 20;

		int persistCount = 3;

		int bonusBlockTimeout;
		int levelTimeoutMin; // how long should the level be??
		int levelTimeoutSec;

		int returnFireVerticalSpeed;
		int returnFireInterval = 50;

		int userFireVerticalSpeed;

		int verticalStopPoint = 40;

		const static int blockSpacing = 0;

		int passThroughTimeout = 5;

		int bonusPoints = 200;
		int userAllowedFire  = 6;

		int maxBlockDropSpeed = 3;
		int maxUserFireVerticalSpeed = 3;

		// first doorway bank is always the top bank, the one in which the blocks land on !!
		class doorwayBank
		{
		public:
			int totalDoorways;
			int seperatorHeight;
		
			int x,y;

			bool shuffleEntrances;
			bool shuffleExits;
			bool shuffleEntrancesOrExits;

			doorwayBank(int td,int sp,int x1,int y1,bool sen,bool sex,bool seoe)
			{
				totalDoorways = td;
				seperatorHeight = sp;

				x = x1; y = y1;	

				shuffleEntrances = sen;
				shuffleExits = sex;
				shuffleEntrancesOrExits = seoe;
			}
		};

		int totalDoorwayBanks;
		doorwayBank **doorwayBanks;//[];
	};
};

#endif