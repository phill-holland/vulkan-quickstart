#include "engine.h"
#include "level1.h"
#include "globals.h"

void engine::reset(int theLevel)
{
    init = false; cleanup();

    levels = new legacy::level*[1];
    if(levels == NULL) return;

    levels[0] = new level1();
    if(levels[0] == NULL) return;

    currentLevel = theLevel;
    finished = false;
    currentLevelFinished = false;

    deathPause = false;
    deathPauseCounter = 0;

    startLevel = true;
    startLevelCounter = 0;
    
    initObjects();

    init =true;
}

bool engine::initObjects()
{
    // SHOULD PROBABLY CLEANUP HERE

    legacy::level *cl = levels[currentLevel];
            
    //dw = new doorways(0,50,6,globals.maxWidth,50);

    dw = new doorways*[cl->totalDoorwayBanks];
    if(dw == NULL) return false;

    for(int i=0;i<cl->totalDoorwayBanks;i++)
    {
        legacy::level::doorwayBank *db = cl->doorwayBanks[i];
        dw[i] = new doorways(db->x,db->y,db->totalDoorways,globals::maxWidth,db->seperatorHeight);
        if(dw[i] == NULL) return false;
        if(!dw[i]->initalised()) return false;
    }

    bc = new columns(cl,cl->doorwayBanks[0]->x,cl->verticalStopPoint,globals::maxWidth,cl->doorwayBanks[0]->totalDoorways);
    if(bc == NULL) return false;
    if(!bc->initalised()) return false;

    bl = new line(cl->doorwayBanks[0]->x,cl->verticalStopPoint+block::height,globals::maxWidth,cl->doorwayBanks[0]->totalDoorways);
    if(bl == NULL) return false;
    if(!bl->initalised()) return false;

    u = new user(cl,cl->doorwayBanks[0]->x,globals::maxHeight - 27,globals::maxWidth,cl->doorwayBanks[0]->totalDoorways);
    if(u == NULL) return false;
    //if(!u->initalised()) return;

    ct = new timeout();
    if(ct == NULL) return false;
    //if(!ct->initalised()) return;

    ct->start(levels[currentLevel]->levelTimeoutMin,levels[currentLevel]->levelTimeoutSec);

    bEffects = new effects(cl->doorwayBanks[0]->x,cl->verticalStopPoint+block::height,globals::maxWidth,cl->doorwayBanks[0]->totalDoorways);
    if(bEffects != NULL) return false; 
    if(!bEffects-initalised()) return false;

    return true;
}

void engine::updateObjects()
{
    legacy::level *cl = levels[currentLevel];

    bc->update(*bl,*u,dw,cl->totalDoorwayBanks,*bEffects,*ct);
    u->update();

    for(int i=0;i<levels[currentLevel]->totalDoorwayBanks;i++)
    {
        u->Weapons->checkCollision(dw[i]);
        bc->checkCollision(*dw[i]);

    }

    for(int j=0;j<u->Weapons->total;j++)
        bl->checkCollision(*u->Weapons->weaponsArray[j]);


    for(int i=0;i<bc->totalColumns;i++)
    {
        if(u->isCollision(*bc->blocksArray[i]->Weapons))
        {
            u->lives--;
            deathPause = true;
        }		
        
    }

    //level *cl = levels[currentLevel];

    for(int i=0;i<cl->totalDoorwayBanks;i++)
    {
        dw[i]->update();
    }

    if(deathPause==false)
    {
        if(bc->isFull())
        {
            u->lives--;
            deathPause = true;
        }
    }

    bl->update();
    ct->update();

    bEffects->update();
}

void engine::drawObjects(legacy::graphics g)
{
    u->draw(g);

    for(int i=0;i<levels[currentLevel]->totalDoorwayBanks;i++)
    {
        dw[i]->draw(g);
    }

    bc->draw(g);
    bl->draw(g);
    
    ct->draw(g);
    u->red = ct->red;
    u->drawStatistics(g);

    bEffects->draw(g);
}

bool engine::update()
{
    if(currentLevelFinished==true) 
    { 
        if(currentLevel>=1/*levels.length*/) { finished = true; return finished; } 

        int tempScore = u->score;
        int tempLives = u->lives;

        reset(++currentLevel); 

        u->score = tempScore;
        u->lives = tempLives;

    } 

    if(!currentLevelFinished)
    {
        if((deathPause==false)&&(startLevel==false))
        {
            updateObjects();
        }
        else if(deathPause)
        {
            deathPauseCounter++;
            if(deathPauseCounter>=deathPauseTimeout)
            {
                deathPauseCounter = 0;
                deathPause = false;

                bc->reset();
                u->reset();
            }
        }		
        else if(startLevel)
        {
            startLevelCounter++;
            if(startLevelCounter>=startLevelTimeout)
            {
                startLevelCounter = 0;
                startLevel = false;
                ct->start(levels[currentLevel]->levelTimeoutMin,levels[currentLevel]->levelTimeoutSec);
            }
        }

        // ***
        currentLevelFinished = ct->isFinished();
        // ***
    }


    if((u->lives<=0)&&(deathPause==false)) finished = true;

    return finished;
}

void engine::keyPressed(int e)
{
    u->keyPressed(e);
}

void engine::keyReleased(int e)
{
    u->keyReleased(e);
}

void engine::draw(legacy::graphics g)
{
    g.setColor(0);
    g.fillRect(0,0,globals::maxWidth,globals::maxHeight);
    
    drawObjects(g);

    if(deathPause)
    {
        //g.drawImage(globals::dFont.draw("life lost"),44,75,0);
        
        if(u->lives>0)
        {
            //g.drawImage(globals::dFont.draw("get ready"),45,100,0);
        }
        else
        {
            //g.drawImage(globals.dFont.draw("game over"),43,100,0);
        }
        
    }

    if(startLevel)
    {
        //g.drawImage(globals.dFont.draw("level " + Integer.toString(currentLevel + 1)),55,85,0);
        //g.drawImage(globals.dFont.draw("get ready"),45,100,0);			
    }
}

void engine::makeNull()
{
    levels = NULL;
	dw = NULL;
	bc = NULL;
	bl = NULL;
	u = NULL;
	ct = NULL;
	bEffects = NULL;
}

void engine::cleanup()
{
    if(bEffects != NULL) delete bEffects;
    if(ct != NULL) delete ct;
    if(u != NULL) delete u;
    if(bl != NULL) delete bl;
    if(bc != NULL) delete bc;
    if(dw != NULL)
    {
        legacy::level *cl = levels[currentLevel];
        for(int i=cl->totalDoorwayBanks-1;i>=0;--i)
        {
            if(dw[i]!=NULL) delete dw[i];
        }
        delete[] dw;
    }
    if(levels != NULL)
    {
        for(int i=0;i<1;++i) 
        {
            if(levels[i] != NULL) delete levels[i];
        }
        delete[] levels;
    }
}