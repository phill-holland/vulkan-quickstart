#include <X11/Xlib.h>
#include <stdint.h>
#include "interfaces/window.h"

#ifndef _VULKAN_WINDOW
#define _VULKAN_WINDOW

namespace vulkan
{
    class window : public interfaces::window
    {
        XSetWindowAttributes windowAttrib;
        Display *display;
        Window context;

        int width, height;

        bool init;

    public:
        window(int _width, int _height) { makeNull(); reset(_width, _height); }
        ~window() { cleanup(); }

        bool initalised() { return init; }
        void reset(int _width, int _height);

        int getWidth() { return width; } 
        int getHeight() { return height; }
        
    public:
        Display *getDisplay() { return display; } 
        Window getWindow() { return context; }

    protected:
        bool createWindow(uint32_t index);

    protected:
        void makeNull();
        void cleanup();
    };
};

#endif