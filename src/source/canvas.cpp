#include "canvas.h"
#include "colour.h"
#include "texture.h"
//#include "targa.h"
#include "log.h"
//#include <memory.h>
//#include "pstring.h"
//#include <cstring>

void canvas::background(thread *bt)
{
	sleep(50);

	context->render();
}

void canvas::reset()//float min, float max, float high)
{
	init = false; cleanup();

	//minimum = min; maximum = max;
	//highest = high;

	//difference = 0.0f;

	context = new display::display<opengl::std::screen, opengl::std::region>();
	if (context == NULL) return;
	context->reset(0, (unsigned int)WIDTH, (unsigned int)HEIGHT);
	if (!context->initalised()) return;

	context->title(string("Graph"));
	context->region()->set(this);

	width = context->region()->getWidth();
	height = context->region()->getHeight();

	buffer = new colour[width * height];
	if (buffer == NULL) return;
	int len = width * height;
	for(int i=0;i<len;++i) buffer[i] = colour(0,0,0,0);
	//memset(buffer, 0, sizeof(colour) * width * height);

	font = new graphics::font();
	if (font == NULL) return;
	if (!font->initalised()) return;

	init = true;
}

void canvas::clear()
{
	semaphore lock(token);

	//memset(buffer, 0, sizeof(colour) * width * height);
	int len = width * height;
	for(int i=0;i<len;++i) buffer[i] = colour(0,0,0,0);

}

/*
bool canvas::save(string filename)
{
	bool result = false;

	texture *source = new texture(width, height);
	if (source != NULL)
	{
		if (source->initalised())
		{
			clear((void*)source->data);
			render((void*)source->data);

			images::targa targa(source);
			result = targa.save(filename);
		}

		delete source;
	}

	return result;
}
*/
void canvas::render(void *linear)
{
	semaphore lock(token);

	clear(linear);

	unsigned char *ptr = (unsigned char *)linear;

	int offset = 0;

	for (unsigned long i = 0UL; i < (width * height); ++i)
	{
		if ((buffer[i].red > 0) || (buffer[i].green > 0) || (buffer[i].blue > 0))
		{
			ptr[offset] = buffer[i].red;
			ptr[offset + 1] = buffer[i].green;
			ptr[offset + 2] = buffer[i].blue;
			ptr[offset + 3] = 255;
		}

		offset += 4;
	}
}
/*
void canvas::render(int x, int y, image *src)
{
	semaphore lock(token);

	for(int i=0;i<src->getHeight();++i)
	{
		for(int j=0;j<src->getWidth();++j)
		{
			colour c = src->get(j,i);
			int offset = (((y + i) * width) + (j + x));// * 4;
			buffer[offset] = c;
		}
	}
}
*/
void canvas::render(int x, int y, surface *src)
{
	semaphore lock(token);

	for(int i=0;i<src->getHeight();++i)
	{
		for(int j=0;j<src->getWidth();++j)
		{
			colour c = src->get((unsigned long)j,(unsigned long)i);
			int offset = (((y + i) * width) + (j + x));// * 4;
			buffer[offset] = c;
		}
	}
}

void canvas::clear(void *linear)
{
	unsigned char *ptr = (unsigned char *)linear;

	colour background = colour(60, 10, 95, 255);
	//colour centre = colour(255, 0, 0, 255);
	//colour most = colour(3, 252, 28, 255);

	colour *c = &background;

	unsigned int half = height / 2;

	//float y_scale = (float)height / (maximum - minimum);
	//int top = (int)((highest - minimum) * y_scale);

	for (unsigned int y = 0; y < height; ++y)
	{
		c = &background;

		//if (y == (unsigned int)top) c = &most;
		//if (y == half) c = &centre;

		for(unsigned int x = 0; x < width; ++x)
		{
			unsigned int offset = ((y * width) + x) * 4;

			ptr[offset] = c->red;
			ptr[offset + 1] = c->green;
			ptr[offset + 2] = c->blue;
			ptr[offset + 3] = c->alpha;
		}
	}
}
/*
void canvas::copy(scoring::scoring *source)
{
	semaphore lock(token);

	for (unsigned long i = 0UL; i < (width * height); ++i)
	{
		if ((((int)buffer[i].red) - 25) < 0) buffer[i].red = 0;
		else buffer[i].red -= 25;

		if ((((int)buffer[i].green) - 25) < 0) buffer[i].green = 0;
		else buffer[i].green -= 25;

		if ((((int)buffer[i].blue) - 25) < 0) buffer[i].blue = 0;
		else buffer[i].blue -= 25;
	}

	float x_scale = ((float)width) / (float)source->count();
	float y_scale = (float)height / (maximum - minimum);

	for (long i = 0L; i < source->count(); ++i)
	{
		int x = (int)(((float)i) * x_scale);

		float data = source->value(i);
		if (data < minimum) data = minimum;
		if (data > maximum) data = maximum;

		int y = (int)((data - minimum) * y_scale);

		if ((y >= 0) && (y < (int)height))
		{
			int offset = (y * width) + x;
			buffer[offset].red = 255;
		}
	}

	float mean = source->mean();
	float deviation = source->deviation(mean);
	int y = (int)((deviation - minimum) * y_scale);

	if ((y >= 0) && (y < (int)height))
	{
		for (long x = 0L; x < (long)width; ++x)
		{
			int offset = (y * width) + x;
			buffer[offset].green = 255;
		}
	}

	font->draw(string::fromFloat(difference), 5, 5, (unsigned char*)buffer, width, height);
}
*/
void canvas::makeNull()
{
	context = NULL;
	buffer = NULL;
	font = NULL;
}

void canvas::cleanup()
{
	if (font != NULL) delete font;
	if (buffer != NULL) delete[] buffer;
	if (context != NULL) delete context;	
}
