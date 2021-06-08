#include "pstring.h"

#ifndef _GRAPHICS
#define _GRAPHICS

namespace graphics
{
	class device
	{
	public:
		virtual unsigned int getDeviceID() = 0;

		virtual void *getDevice() = 0;
		virtual void *getContext() = 0;
		virtual void *getWindow() = 0;

		virtual bool setDevice() = 0;
	};

	class region
	{	
	public:
		virtual bool initalise(device *d) = 0;
		virtual bool render(device *d) = 0;

		virtual void reset(unsigned int width, unsigned int height) = 0;

		virtual unsigned int getWidth() = 0;
		virtual unsigned int getHeight() = 0;
	};

	class screen
	{
	public:
		virtual bool initalised() = 0;
		virtual bool get(region *destination) = 0;
		virtual bool render(region *source) = 0;

		virtual bool title(string source) = 0;

		virtual void reset(unsigned int device, unsigned int width, unsigned int height, bool windowed) = 0;
	};	
};

#endif