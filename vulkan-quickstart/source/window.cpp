#include "window.h"

void vulkan::window::reset(int _width, int _height)
{
    init = false; cleanup();

    width = _width; height = _height;

    init = createWindow(0);
}

bool vulkan::window::createWindow(uint32_t index)
{
    int x = 0, y = 0;

	XInitThreads();

	display = XOpenDisplay(NULL);

	windowAttrib.border_pixel = BlackPixel(display, (int)index);
	windowAttrib.background_pixel = WhitePixel(display, (int)index);
	windowAttrib.override_redirect = True;
	windowAttrib.colormap = XCreateColormap(display, RootWindow(display, (int)index),
    DefaultVisual(display, index), AllocNone);
	windowAttrib.event_mask = ExposureMask;

	context = XCreateWindow(display, RootWindow(display, (int)index),
                           x, y, width, height, 0,
                           DefaultDepth(display, index),
                           CopyFromParent,
                           DefaultVisual(display, index),
                           CWBackPixel | CWColormap | CWBorderPixel | CWEventMask,
                           &windowAttrib);

	XSelectInput(display, context, ExposureMask | StructureNotifyMask);

	XMapWindow(display, context);

	char caption[20] = "Screen\0";

    XStoreName(display, context, caption);

	return true;
}

void vulkan::window::makeNull()
{
    display = NULL;
}

void vulkan::window::cleanup()
{
    if (display != NULL)
	{
		XFreeColormap(display, windowAttrib.colormap);
		XDestroyWindow(display, context);
		XCloseDisplay(display);
	}
}