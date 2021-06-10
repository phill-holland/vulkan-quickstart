#include "effect.h"
#include "globals.h"

void effect::reset()
{
    init = false; cleanup();

    type = DEAD;

    positionX = 0; positionY = 0; offsetX = 0; offsetY = 0;

    width = globals::bonusTextWidth * globals::bonusTextCellWidth;
    height = globals::bonusTextHeight * globals::bonusTextCellHeight;

    //tempArray = new 
    picture = image::createImage(width,height);
   // pg = picture.getGraphics();		

    init = true;
}

void effect::start(int t)
{
    type = t;
    if(type==POINTS) tempArray = globals::bonusPoints;
    if(type==FASTER) tempArray = globals::bonusFaster;
    if(type==UNLIMITED) tempArray = globals::bonusUnlimited;
    if(type==LEVEL) tempArray = globals::bonusLevel;	
    if(type==CYCLE) tempArray = globals::bonusCycle;
    if(type==LIFE) tempArray = globals::bonusLife;
    if(type==RATEINC) tempArray = globals::bonusRateIncrease;
    if(type==RATEDEC) tempArray = globals::bonusRateDecrease;
    if(type==TIMEINC) tempArray = globals::bonusTimeIncrease;
    if(type==TIMEDEC) tempArray = globals::bonusTimeIncrease;

    pg.setColor(0);
    pg.fillRect(0,0,width,height);

    currentX = 0; currentY = 0;
    currentCellX = 0; currentCellY = globals::bonusTextHeight;
    
    stop = false; bonusCounter = 0;
}

void effect::update()
{
    if(type!=DEAD)
    {
        
        while(stop==false)
        {
            currentCellX++;
            if(currentCellX>=globals::bonusTextWidth)
            {
                currentCellX = 0;
                currentCellY--;
                if(currentCellY<0) { stop = true; return; }
            }

            int offset = (currentCellY * globals::bonusTextWidth) + currentCellX;

            if(tempArray[offset]==1)
            {	
                
                currentX = currentCellX * globals::bonusTextCellWidth;
                currentY = currentCellY * globals::bonusTextCellHeight;				
                pg.setColor(0xFFFFFF);
                pg.fillRect(currentX,currentY,globals::bonusTextCellWidth,globals::bonusTextCellHeight);
                return;
            }
        }

        if(stop==true)			
        {
            bonusCounter++;
            if(bonusCounter>bonusTimeout)
            {
                type = DEAD;
            }
        }
    }
}

void effect::draw(legacy::graphics g)
{
    //g.setColor(0xff0000);
    //g.fillRect(positionX,offsetY,2,2);
    if(type!=DEAD)
    {
        //g.drawImage(picture,positionX - (width / 2),positionY + (height / 2),0);
        g.drawImage(*picture,positionX - (width / 2),positionY,0);
        if(stop==false)
        {
            g.setColor(0xFFFFFF);
            g.drawLine(positionX,offsetY + 2,positionX + currentX - (width / 2),positionY + currentY);//currentX + (positionX - (width / 2)),currentY + (positionY + (height / 2)));		
        }
    }
}

	void effect::makeNull()
    {
        //tempArray = NULL;
    }

	void effect::cleanup()
    {
       // if(tempArray != NULL) delete[] tempArray;
    }