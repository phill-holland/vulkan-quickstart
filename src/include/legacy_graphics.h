#include "image.h"

#ifndef _LEGACY_GRAPHICS
#define _LEGACY_GRAPHICS

namespace legacy
{
    class graphics
    {
        int width, height;
        unsigned long length;
        unsigned char *buffer;

    public:
        void drawImage(image img, int x, int y, int stride)
        {

        }

        void setColor(int colour) 
        {

        }

        void setColor(int red, int blue, int green)
        {

        }

        void fillRect(int x, int y, int width, int height)
        {

        }

        void drawRGB(int *x, int y, int width, int xOfx, int yOfs, int w, int h, bool m)
        {
            //temp,0,globals.maxWidth,rightXOffset,0,globals.maxWidth,globals.maxHeight,true)
        }

        void drawLine(int a, int b, int c, int d)
        {

        }
            //positionX,offsetY + 2,positionX + currentX - (width / 2),positionY + currentY);//currentX + (positionX - (width / 2)),currentY + (positionY + (height / 2)));		
    };
};

#endif