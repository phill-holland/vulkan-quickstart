#include "doorway.h"

string doorway::filenames[2] = { string("/images/entrancebucket.jpg"), 
                                 string("/images/exitbucket.jpg") };

void doorway::reset(doorway &dw)
{
    init = false; cleanup();

    c = colour(dw.c);
    linked = dw.linked;
    type = dw.type;
    positionX = dw.positionX;
    positionY = dw.positionY;

    doors = new images(filenames,2);
    if(doors == NULL) return;
    if(!doors->initalised()) return;

    init = true;
}


void doorway::reset(int t)
{	
    init = false; cleanup();

    type = t;

    c = colour(255,255,255);		

    doors = new images(filenames,2);
    if(doors == NULL) return;
    if(!doors->initalised()) return;

    picture = doors->get(type);

    init = true;
}

void doorway::reset(int x,int y,colour i)
{
    init = false; cleanup();

    c = colour(i);		
    positionX = x; positionY = y;

    doors = new images(filenames,2);
    if(doors == NULL) return;
    if(!doors->initalised()) return;

    picture = doors->get(type);

    init = true;
}

void doorway::makeNull()
{
    doors = NULL;
}

void doorway::cleanup()
{
    if(doors != NULL) delete doors;
}