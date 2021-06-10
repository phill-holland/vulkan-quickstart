#include "doorway.h"
#include "legacy_graphics.h"

#ifndef _ENTRANCE
#define _ENTRANCE

namespace doors
{
	class entrance : public doorway
	{
	public:
		entrance() : doorway(ENTRANCE) { }

		void draw(legacy::graphics g)
		{
			doorway::draw(g);

			g.setColor(c.red,c.blue,c.green);		
			g.fillRect(positionX + 5,positionY + 7,7,2);
		}
	};
};

#endif