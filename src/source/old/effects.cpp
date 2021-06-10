#include "effects.h"
#include "globals.h"

void effects::reset(int horzPos,int vertPos,int widPix,int totCol)
{
    init = false; cleanup();

    widthInPixels = widPix;
    totalColumns = totCol;
    positionX = horzPos; positionY = vertPos; offsetX = horzPos; offsetY = vertPos;

    pixelInterval = (widthInPixels - globals::doorwayWidth) / totalColumns;

    bonusArray = new effect*[totalColumns];
    if(bonusArray == NULL) return;

    for(int i=0;i<totalColumns;i++) bonusArray[i] = NULL;

    for(int i=0;i<totalColumns;i++)
    {
        bonusArray[i] = new effect();
        if(bonusArray[i] == NULL) return;
        if(!bonusArray[i]->initalised()) return;
    }

    init = true;
}

void effects::add(int doorwayIndex,int type)
{
    bonusArray[doorwayIndex]->offsetX = offsetX; bonusArray[doorwayIndex]->offsetY = offsetY;
    bonusArray[doorwayIndex]->positionX = offsetX + (doorwayIndex * pixelInterval) + (globals::doorwayWidth / 2);

    int index;
    if((doorwayIndex % 2)==0) index = 5;
        else index = 12;

    bonusArray[doorwayIndex]->positionY = positionY + index;

    bonusArray[doorwayIndex]->start(type);
}

void effects::update()
{
    for(int i=0;i<totalColumns;i++)
    {
        bonusArray[i]->update();
    }
}

void effects::draw(legacy::graphics g)
{
    for(int i=0;i<totalColumns;i++)
    {
        bonusArray[i]->draw(g);
    }
}

void effects::makeNull()
{
    bonusArray = NULL;
}

void effects::cleanup()
{

}