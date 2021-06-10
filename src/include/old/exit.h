#include "doorway.h"
#include "legacy_graphics.h"

#ifndef _EXIT
#define _EXIT

namespace doors
{
	class exit : public doorway
	{
	public:
		exit() : doorway(EXIT) { }
		
		void draw(legacy::graphics g)
		{
			doorway::draw(g);

			g.setColor(c.red,c.blue,c.green);
			g.fillRect(positionX + 5,positionY + 1,7,2);
		}
	};
};

#endif
