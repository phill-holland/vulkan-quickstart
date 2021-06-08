#include "doorways.h"
#include "globals.h"
#include "entrance.h"
#include "exit.h"

std::mt19937_64 doorways::generator(std::random_device{}());

string doorways::filenames[2] = { string("/images/entranceline.jpg"), 
                                 string("/images/exitline.jpg") };

colour doorways::uniqueColours[7] = {colour(0,0,255),colour(0,255,0),colour(255,0,0),colour(255,255,0),colour(0,255,255),colour(255,0,255),colour(255,255,255)};

void doorways::reset(int x,int y,int td,int wp,int sh)
{
    init = false; cleanup();

    doors = new images(filenames,2);
    if(doors == NULL) return;
    if(!doors->initalised()) return;

    cycleCounter = 0;
	cycleDelay = 20;

    int index,xoffset;

    positionX = x; positionY = y;
    xoffset = positionX;

    widthInPixels = wp; totalDoors = td;
    pixelInterval = (widthInPixels - globals::doorwayWidth) / totalDoors;

    entrances = new doorway*[totalDoors];
    if(entrances == NULL) return;
    for(int i=0;i<totalDoors;++i) entrances[i] = NULL;

    exits = new doorway*[totalDoors];
    if(exits == NULL) return;
    for(int i=0;i<totalDoors;++i) exits[i] = NULL;

    if(sh==0) seperatorHeight = globals::doorwayHeight * 3;
        else seperatorHeight = sh;

    for(index=0;index<totalDoors;index++)
    {
        entrances[index] = new doors::entrance();
        if(entrances[index]==NULL) return;
        
        exits[index] = new doors::exit();	
        if(exits[index]==NULL) return;

        exits[index]->reset(xoffset,positionY,uniqueColours[index]);
        entrances[index]->reset(xoffset,positionY  + seperatorHeight ,uniqueColours[index]);

        exits[index]->linked = entrances[index];
        entrances[index]->linked = exits[index];

        exits[index]->id = index;
        entrances[index]->id = index;

        xoffset += pixelInterval;
    }

    shuffleExits();
    shuffleEntrances();

    init = true;
}

void doorways::update()
{
    if(cycleCounter>0)
    {
        if((cycleCounter % cycleDelay)==0)
        {
            shuffleExits();
            shuffleEntrances();
        }
        cycleCounter++;
        if(cycleCounter>cycleTimeout) cycleCounter = 0;
    }
}

void doorways::enableCycle()
{
    cycleCounter = 1;
}

void doorways::shuffleEntrances()
{
    std::uniform_int_distribution<int> dist{ 0, INT16_MAX };
    int half = totalDoors / 2;

    for(int index=0;index<half;index++)
    {
        //int r = (Math.abs(globals.rand.nextInt()) % half) + half;
    //std::uniform_int_distribution<int> dist{ 0, INT16_MAX };
    int r = (dist(generator) % half) + half;

        colour temp = entrances[r]->c;
        doorway *tlink = entrances[r]->linked;
        int id = entrances[r]->id;

        entrances[r]->c = entrances[index]->c;
        entrances[r]->linked = entrances[index]->linked;
        entrances[r]->id = entrances[index]->id;

        entrances[index]->c = temp;
        entrances[index]->linked = tlink;			
        entrances[index]->id = id;

        entrances[r]->linked->linked = entrances[r];
        entrances[index]->linked->linked = entrances[index];
    }
}

void doorways::shuffleExits()
{
    int half = totalDoors / 2;
    std::uniform_int_distribution<int> dist{ 0, INT16_MAX };

    for(int index=0;index<half;index++)
    {
        //int r = (Math.abs(globals.rand.nextInt()) % half) + half;
        
    int r = (dist(generator) % half) + half;

        colour temp = exits[r]->c;
        doorway *tlink = exits[r]->linked;
        int id = exits[r]->id;

        exits[r]->c = exits[index]->c;
        exits[r]->linked = exits[index]->linked;
        exits[r]->id = exits[index]->id;

        exits[index]->c = temp;
        exits[index]->linked = tlink;			
        exits[index]->id = id;

        exits[r]->linked->linked = exits[r];
        exits[index]->linked->linked = exits[index];		
    }
}

int doorways::findEntrance(int id)
{
    for(int index=0;index<totalDoors;index++)
    {
        if(entrances[index]->id == id) return index;
    }
    return -1;
}

int doorways::findExit(int id)
{
    for(int index=0;index<totalDoors;index++)
    {
        if(exits[index]->id == id) return index;
    }
    return -1;
}

void doorways::draw(legacy::graphics g)
{
    int offsetY = positionY + seperatorHeight;//compil + globals.doorwayHeight - 1;

    g.drawImage(doors->get(EXIT),0,positionY,0);
    g.drawImage(doors->get(ENTRANCE),0,offsetY,0);

    //g.setColor(0xFFFFFF);
    //g.drawLine(0,positionY,globals.maxWidth,positionY);

    
    //g.drawLine(0,offsetY,globals.maxWidth,offsetY);

    for(int index=0;index<totalDoors;index++)
    {
        entrances[index]->draw(g);
        exits[index]->draw(g);
    }
}

void doorways::makeNull()
{
    doors = NULL;
    entrances = NULL;
    exits = NULL;
}

void doorways::cleanup()
{
    if(exits != NULL)
    {
        for(int i = totalDoors - 1; i >= 0; --i)
        {
            if(exits[i] != NULL) delete exits[i];
        }
        delete[] exits;
    }

    if(entrances != NULL)
    {   
        for(int i = totalDoors - 1; i >= 0; --i)
        {
            if(entrances[i] != NULL) delete entrances[i];
        }
        delete[] entrances;
    }

    if(doors != NULL) delete doors;
}