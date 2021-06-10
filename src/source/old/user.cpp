#include "user.h"
#include "globals.h"

string user::filenames[3] = { string("/images/user.jpg"),
                               string("/images/gray.jpg"),
                               string("/images/tinyuser.jpg")
                                };

void user::reset(legacy::level *l,int horizontalPosition,int verticalPosition,int wp,int tc)
{
    init = false; cleanup();

    current = l;

    movementTimeout = 10; movementCounter = 0;
    weaponCounter = 0;

    imgs = new images(filenames,3);
    if(imgs == NULL) return;
    if(!imgs->initalised()) return;

    //Weapon = new weapon(currentLevel);
    Weapons = new weapons(current,current->userAllowedFire,totalWeapons);
    if(Weapons == NULL) return;
    if(!Weapons->initalised()) return;

    positionX = horizontalPosition;
    positionY = verticalPosition;
    widthInPixels = wp; totalColumns = tc;

    pixelInterval = (widthInPixels - globals::doorwayWidth) / totalColumns;

    columnPosition = 0;

    left = false; right = false; fire = false;

    score = 0; lives = 3;
    weaponCounter = 0;

    red = false;

    init = true;
}

void user::reset()
{
    Weapons->reset();
    columnPosition = 0;
}

void user::update()
{
    if(left==true)
    {
        if(movementCounter==0)
        {
            right = false;
            dec();
        }
        movementCounter++;
        if(movementCounter>movementTimeout) movementCounter = 0;
    }
    if(right==true)
    {
        if(movementCounter==0)
        {
            left = false;
            inc();
        }
        movementCounter++;
        if(movementCounter>movementTimeout) movementCounter = 0;
    }
    if(fire==true)
    {
        if(weaponCounter==0)
        {
            // allow one weapon per column to exists - per user fire power !!
            //if((Weapon.state & weapon.DEAD)==weapon.DEAD)
            //{
                //Weapon.positionX = ((columnPosition * pixelInterval) + positionX) + (width / 2);
                //Weapon.positionY = positionY;

                //Weapon.state = weapon.LIVE;
                Weapons->add(((columnPosition * pixelInterval) + positionX) + (width / 2),positionY);
            //}
        } else if(weaponCounter>=weaponTimeout) weaponCounter = -1;
        weaponCounter++;
        
    }
    
    Weapons->updateNorth();
}

void user::draw(legacy::graphics &g)
{
    int xOffset = positionX;
    for(int i=0;i<totalColumns;i++)
    {
        if(i==columnPosition) g.drawImage(imgs->get(0),xOffset,positionY,0);
            else g.drawImage(imgs->get(1),xOffset,positionY,0);

        xOffset += pixelInterval;
    }		

    //if(((Weapon.state & weapon.DEAD)!=weapon.DEAD)&&((Weapon.state & weapon.HIDDEN)!=weapon.HIDDEN))
    //	Weapon.draw(g);
    Weapons->draw(g);
}

void user::drawStatistics(legacy::graphics &g)
{
    /*
    string temp = "000000" + Integer.toString(score);
    temp = temp.substring(temp.length()-6,temp.length());

    int r = 255; int grn = 0; int b = 0;
    if(red==false) { r= 165; grn = 104; b = 4; }

    g.drawImage(globals.dFont.draw(temp,r,grn,b),2,globals.maxHeight - 10,0);

    int xOffset = 70;
    for(int i=0;i<lives;i++)
    {
        g.drawImage(images.get(2),xOffset,globals.maxHeight - 10,0);
        xOffset += 9;
    }
    */
}

void user::inc()
{		
    ++columnPosition;
    if(columnPosition>=totalColumns) --columnPosition;
}

void user::dec()
{
    --columnPosition;
    if(columnPosition<0) ++columnPosition;
}

bool user::isCollision(weapons ws)
{
    int x = (pixelInterval * columnPosition) + positionX;

    //region r = new region(x,positionY,width,height);
    legacy::region r(x,positionY,width,height);
    for(int i=0;i<ws.total;i++)
    {
        if(((ws.weaponsArray[i]->state & weapon::LIVE)==weapon::LIVE)&&((ws.weaponsArray[i]->state & weapon::HIDDEN)!=weapon::HIDDEN))
            if(ws.weaponsArray[i]->isCollision(&r)==true) return true;
    }
    return false;
}

void user::keyPressed(int e)
{
    /*
    if(e==GameCanvas.LEFT)
    {
        movementCounter = 0;
        left = true;
        right = false;
    }
    if(e==GameCanvas.RIGHT)
    {
        movementCounter = 0;
        left = false;
        right = true;
    }
    if((e==GameCanvas.FIRE)||(e==GameCanvas.UP))
    {
        fire = true;
        weaponCounter = 0;
    }
    */
}

void user::keyReleased(int e)
{
    /*
    if((e==GameCanvas.FIRE)||(e==GameCanvas.UP))
    {
        fire = false;
        weaponCounter = 0;
    }
    else
    {
        left = false;
        right = false;
    }
    */
}

void user::makeNull()
{
    imgs = NULL;
    Weapons = NULL;
}

void user::cleanup()
{
    if(Weapons != NULL) delete Weapons;
    if(imgs != NULL) delete imgs;
}