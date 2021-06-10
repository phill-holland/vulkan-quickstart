#include "graphics.h"
#include "regions.h"

#ifndef _DISPLAY
#define _DISPLAY

namespace display
{
	class context
	{
	public:		
		virtual void reset(unsigned int device = 0, 
						   unsigned int width = 640, 
						   unsigned int height = 480, 
						   unsigned int texture_width = 256, 
						   unsigned int texture_height = 256, 
						   bool windowed = true) = 0;		
		virtual bool initalised() = 0;

		bool render() { return screen()->render(region()); }

		virtual graphics::screen *screen() = 0;
		virtual regions::region *region() = 0;

		virtual bool title(string source) = 0;
	};

	template <class S = graphics::screen, class R = regions::region> class display : public context
	{
		bool init;

		graphics::screen *_screen;
		regions::region *_region;

	public:
		display() { makeNull(); }
		~display() { cleanup(); }
		
		void reset(unsigned int device = 0,
				   unsigned int width = 640, 
				   unsigned int height = 480, 
				   unsigned int texture_width = 256,
				   unsigned int texture_height = 256,
				   bool windowed = true)
		{
			init = false; cleanup();

			_region = new R();
			if (_region == NULL) return;
			_region->reset(texture_width, texture_height);

			_screen = new S();
			if (_screen == NULL) return;
			_screen->reset(device, width, height, windowed);
			if (!_screen->initalised()) return;
			if (!_screen->get(_region)) return;

			init = true;
		}

		bool initalised() { return init; }

		graphics::screen *screen() { return _screen; }
		regions::region *region() { return _region; }

		bool title(string source) { return _screen->title(source); }

	protected:
		void makeNull()
		{
			_screen = NULL;
			_region = NULL;
		}

		void cleanup()
		{
			if (_region != NULL) delete _region;
			if (_screen != NULL) delete _screen;
		}
	};
};

#endif
