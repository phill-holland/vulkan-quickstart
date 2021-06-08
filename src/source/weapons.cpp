#include "weapons.h"

void weapons::reset(legacy::level *l,int totalWeapons,int allowedWeapons)
{
    current = l;

    total = totalWeapons;
    allowed = allowedWeapons;
    originalAllowed = allowedWeapons;

    weaponsArray = new weapon*[total];
    if(weaponsArray == NULL) return;

    for(int i=0;i<totalWeapons;i++)
    {
        weaponsArray[i] = new weapon(current);			
        if(weaponsArray[i] == NULL) return;
        if(!weaponsArray[i]->initalised()) return;
    }
}

void weapons::reset()
{
    for(int i=0;i<total;i++)
    {
        weaponsArray[i]->state = weapon::DEAD;
    }
    allowed = originalAllowed;
    //weapon::userWeaponFireSpeedOffset = 0;
}

void weapons::add(int positionX,int positionY)
{
    int counter = 0;

    do
    {
        if(weaponsArray[counter]->state==weapon::DEAD)
        {
            weaponsArray[counter]->state = weapon::LIVE;

            weaponsArray[counter]->positionX = positionX;
            weaponsArray[counter]->positionY = positionY;

            counter = total;
        }
        
        ++counter;
    }while(counter<allowed);
}

void weapons::updateNorth()
{
    for(int i=0;i<allowed;i++)
    {
        weaponsArray[i]->updateNorth();			
    }
}

void weapons::updateSouth()
{
    for(int i=0;i<allowed;i++)
    {
        weaponsArray[i]->updateSouth();
    }
}

void weapons::checkCollision(doorways *dw)
{
    for(int i=0;i<allowed;i++)
    {
        if(((weaponsArray[i]->state & weapon::LIVE)==weapon::LIVE)&&((weaponsArray[i]->state & weapon::HIDDEN)!=weapon::HIDDEN))
        {
            weaponsArray[i]->checkCollision(dw);
        }
    }
}

void weapons::draw(legacy::graphics &g)
{
    for(int i=0;i<allowed;i++)
    {
        if(((weaponsArray[i]->state & weapon::DEAD)!=weapon::DEAD)&&((weaponsArray[i]->state & weapon::HIDDEN)!=weapon::HIDDEN))
            weaponsArray[i]->draw(g);
    }
}

void weapons::inc()
{
    allowed++;
    if(allowed>=total) allowed--;
}

void weapons::dec()
{
    if(allowed>1) allowed--;
}

void weapons::makeNull()
{
    weaponsArray = NULL;
}

void weapons::cleanup()
{
    if(weaponsArray != NULL)
    {
        for(int i = total - 1; i >= 0; --i)
        {
            if(weaponsArray[i] != NULL) delete weaponsArray[i];
        }
        
        delete[] weaponsArray;
    }
}