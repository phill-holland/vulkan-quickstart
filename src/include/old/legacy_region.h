
#ifndef _LEGACY_REGION
#define _LEGACY_REGION

namespace legacy
{
	class region
	{
	public:
		int x,y;
		int width,height;

	public:
		region()
		{
			x = 0; y = 0;
			width = 0; height = 0;
		}

		region(int x1, int y1, int w, int h)
		{
			x = x1; y = y1; width = w; height = h;
		}

		bool isInside(int x1, int y1)
		{
			if((x1>=x)&&(x1<=(x + width)))
				if((y1>=y)&&(y1<=(y + height))) return true;

			return false;
		}
	};
};

#endif