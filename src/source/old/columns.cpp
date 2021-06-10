#include "columns.h"
#include "globals.h"

std::mt19937_64 columns::generator(std::random_device{}());

void columns::reset(legacy::level *l,int x,int bl,int wp,int tc)
{
    init = false; cleanup();

    current = l;

    widthInPixels = wp;
    totalColumns = tc;
    bottomLine = bl;				
    positionX = x;

    pixelInterval = (widthInPixels - globals::doorwayWidth) / totalColumns;

    int maxVerticalBlocks = (bottomLine / block::height) + 1;

    blocksArray = new blocks*[totalColumns];
    if(blocksArray == NULL) return;

    for(int i=0;i<totalColumns;i++) blocksArray[i] = NULL;

    int offsetX = positionX;
    for(int i=0;i<totalColumns;i++)
    {
        blocksArray[i] = new blocks(current,i,offsetX,bottomLine,maxVerticalBlocks);
        if(blocksArray[i] == NULL) return;
        if(!blocksArray[i]->initalised()) return;

        offsetX += pixelInterval;
    }

    init = true;
}

void columns::reset()
{
    for(int i=0;i<totalColumns;i++)
    {
        blocksArray[i]->reset();
    }
}

void columns::update(line &bl,user &u,doorways **dw,int dwlen,effects &bEffects,timeout &ct)
{
    generate();

    for(int i=0;i<totalColumns;i++)
    {
        blocksArray[i]->update(bl,u,dw,dwlen,bEffects,ct);
    }
}

void columns::checkCollision(doorways &dw)
{
    for(int i=0;i<totalColumns;i++)
    {
        blocksArray[i]->Weapons->checkCollision(&dw);
    }
}

bool columns::isFull()
{
    for(int i=0;i<totalColumns;i++)
    {
        if(blocksArray[i]->isFull()) return true;
    }

    return false;
}

void columns::draw(legacy::graphics &g)
{
    for(int i=0;i<totalColumns;i++)
    {
        blocksArray[i]->draw(g);
    }
}

void columns::generate()
{
    std::uniform_int_distribution<int> dist{ 0, INT16_MAX };

    //if((Math.abs(globals.rand.nextInt()) % current->chanceOfBlock)==0)
    if((dist(generator) % current->chanceOfBlock) == 0)
    {
        //int r = Math.abs(globals.rand.nextInt()) % totalColumns;
        int r = dist(generator) % totalColumns;
        blocksArray[r]->add();
    }
}

void columns::makeNull()
{
    blocksArray = NULL;
}

void columns::cleanup()
{
    if(blocksArray != NULL)
    {
        for(int i = totalColumns - 1; i >= 0; --i)
        {
            if(blocksArray[i] != NULL) delete blocksArray[i];
        }
        delete[] blocksArray;
    }
}
