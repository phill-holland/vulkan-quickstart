#include "weapon.h"
#include "globals.h"

int weapon::constructed = 0;
int weapon::userWeaponFireSpeedOffset = 0;

void weapon::reset(legacy::level *l)
{
    init = false; cleanup();

    current = l;
    state = DEAD;

    if(constructed==0)
    {
        ++constructed;

        imgs = new images(1);
        if(imgs == NULL) return;
        imgs->load(string("/images/weapon.jpg"),0);
        //images = new imageStore(1);
        //images.load("/owd/images/weapon.png",0);
    }

    set(imgs->get(0));
    //userWeaponFireSpeedOffset = 0;

    init = true;
}

void weapon::updateNorth()
{
    if((state & LIVE)==LIVE)
    {
        update();

        if((state & HIDDEN)!=HIDDEN)
        {
            positionY -= current->userFireVerticalSpeed + userWeaponFireSpeedOffset;
            if(positionY<=0) state = DEAD;
        }
    }
}

void weapon::updateSouth()
{
    if((state & LIVE)==LIVE)
    {
        update();

        if((state & HIDDEN)!=HIDDEN)
        {			
            positionY += current->returnFireVerticalSpeed;
            if(positionY>=globals::maxHeight) state = DEAD;
        }
    }
}

void weapon::update()
{
    if((state & HIDDEN)==HIDDEN)
    {
        hiddenCounter++;
        
        if(hiddenCounter>hiddenTimeout)
        {
            hiddenCounter = 0;
            state = state ^ HIDDEN;	
            if(flip==true)
            {
                positionX = dwtemp->exits[dwtemp->findExit(id)]->positionX + (globals::doorwayWidth / 2);
            }
            else
            {
                positionX = dwtemp->entrances[dwtemp->findEntrance(id)]->positionX + (globals::doorwayWidth / 2);
            }			
        }
    }		
}

void weapon::checkCollision(doorways *dw)
{
    for(int index=0;index<dw->totalDoors;index++)
    {
        if(dw->entrances[index]->isCollision(this))
        {
            state = state ^ HIDDEN;
            hiddenCounter = 0;
            hiddenTimeout = current->passThroughTimeout;

            //positionX = dw.entrances[index].linked.positionX + (globals.doorwayWidth / 2);
            positionY = dw->entrances[index]->linked->positionY-10;				
            id = dw->entrances[index]->id;
            flip = true;
            dwtemp = dw;
        }			
        else if(dw->exits[index]->isCollision(this))
        {
            state = state ^ HIDDEN;
            hiddenCounter = 0;
            hiddenTimeout = current->passThroughTimeout;

            //positionX = dw.exits[index].linked.positionX + (globals.doorwayWidth / 2);
            positionY = dw->exits[index]->linked->positionY + 10;
            id = dw->exits[index]->id;
            flip = false;
            dwtemp = dw;
        }
    }	
}	

void weapon::makeNull()
{
    imgs = NULL;
}

void weapon::cleanup()
{
    if(imgs != NULL) delete[] imgs;
}