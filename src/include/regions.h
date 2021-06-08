#include "graphics.h"

#ifndef _REGION
#define _REGION

namespace regions
{
	class context
	{
	public:
		virtual void render(void *linear) = 0;
	};

	class region : public graphics::region
	{
	public:
		virtual void set(context *destination) = 0;
	};
};

#endif