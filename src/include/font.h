#include "pstring.h"

#ifndef _FONT
#define _FONT

namespace graphics
{
	class font
	{
        static unsigned char charset[128][8];

        bool init;

	public:
		font() { makeNull(); reset(); }
		
        bool initalised() { return init; }
		void reset();
		
        void draw(string value, unsigned int x, unsigned int y, unsigned char *buffer, unsigned int width, unsigned int height);
        void draw(char value, unsigned int x, unsigned int y, unsigned char *buffer, unsigned int width, unsigned int height);

    protected:
        void makeNull();
        void cleanup();
	};
};

#endif