#include "line.h"
#include "globals.h"
#include "weapon.h"

string line::filenames[1] = { string("/images/blockline.jpg") };
           
void line::reset(int horizontalPosition,int verticalPosition,int wp,int tc)
{
    init = false; cleanup();

    positionX = 0;
    hPosition = horizontalPosition;
    positionY = verticalPosition;

    widthInPixels = wp; totalColumns = tc;

    imgs = new images(filenames, 1);
    if(imgs == NULL) return;
    if(!imgs->initalised()) return;

    gateway = new int[totalColumns];
    if(gateway == NULL) return;

    gatewayRegions = new legacy::region*[totalColumns];
    if(gatewayRegions == NULL) return;

    for(int i=0;i<totalColumns;++i) gatewayRegions[i] = NULL;

    for(int i=0;i<totalColumns;++i) 
    {
        gatewayRegions[i] = new legacy::region();
        if(gatewayRegions[i] == NULL) return;
    }

    pixelInterval = (widthInPixels - globals::doorwayWidth) / totalColumns;

    picture = imgs->get(0);

    int offsetX = hPosition;

    for(int i=0;i<totalColumns;i++)
    {
        gateway[i] = 0;
        gatewayRegions[i] = new legacy::region(offsetX,positionY - 2, globals::doorwayWidth,2);
        offsetX += pixelInterval;
    }

    init = true;
}

void line::update()
{
    for(int i=0;i<totalColumns;i++)
    {
        if(gateway[i]!=0)
        {
            gateway[i]++;
            if(gateway[i]>=gatewayTimeout)
            {
                gateway[i] = 0;
            }
        }
    }
}

void line::checkCollision(weapon w)
{
    for(int i=0;i<totalColumns;i++)
    {
        if((w.state & weapon::LIVE)==weapon::LIVE)
        {
            if(w.isCollision(gatewayRegions[i]))
            {
                gateway[i]++;
                w.state = weapon::DEAD;
            }
        }
    }
}

bool line::isLive(int column)
{
    if(gateway[column]>0) return true; 
    return false;
}

bool line::checkCollision(block b,int column)
{
    if(b.isCollision(gatewayRegions[column])) return true; 

    return false;
}

void line::draw(legacy::graphics g)
{
    sprite::draw(g);
    int offsetX = hPosition;

    g.setColor(0XFFFFFF);
    for(int i=0;i<totalColumns;i++)
    {
        if(gateway[i]>0)
        {
            // needs to flash ??
            g.fillRect(offsetX,positionY+1,globals::doorwayWidth,2);		
        }
        offsetX += pixelInterval;
    }
}

void line::makeNull()
{
    imgs = NULL;
    gateway = NULL;
    gatewayRegions = NULL;
}

void line::cleanup()
{
    if(gatewayRegions != NULL)
    {
        for(int i=totalColumns-1;i>=0;--i) 
        {
            if(gatewayRegions[i] != NULL) delete gatewayRegions[i];
        }

        delete[] gatewayRegions;
    }

    if(gateway != NULL) delete[] gateway;
    if(imgs != NULL) delete imgs;
}