#include "blocks.h"
#include "weapon.h"
#include "user.h"

std::mt19937_64 blocks::generator(std::random_device{}());

void blocks::reset(legacy::level *l,int ci,int columnPixel,int rowPixel,int numberOfBlocks)
{
    init = false; cleanup();

    Weapons = new weapons(l,totalWeapons);
    if(Weapons == NULL) return;
    if(!Weapons->initalised()) return;

    totalBlocks = numberOfBlocks;
    offsetX = columnPixel;
    columnIndex = ci;

    bottomOffsetY = rowPixel;
    virtualBottom = rowPixel;
    virtualBottomPerm = rowPixel;
    
    current = l;
    
    addCounter = 0;
    addDelay = ((block::height * current->blockDropInterval) / current->blockDropSpeed) + 20;
    delay = false;

    blockArray = new block*[totalBlocks];
    if(blockArray == NULL) return;

    for(int i=0;i<totalBlocks;i++) blockArray[i] = NULL;

    for(int i=0;i<totalBlocks;i++)
    {
        blockArray[i] = new block(current);
        if(blockArray[i] == NULL) return;
        if(!blockArray[i]->initalised()) return;
    }

    blockDropSpeedOffset = 0;

    init = true;
}

void blocks::reset()
{
    Weapons->reset();
    for(int i=0;i<totalBlocks;i++)
    {
        blockArray[i]->state = block::DEAD;
    }
    virtualBottom = virtualBottomPerm;
    blockDropSpeedOffset = 0;
}

void blocks::update(line &bl,user &u,doorways **dw,int dwlen,effects &bEffects,timeout &ct)
{
    // need to check here to see if blocks have reached ceiling !!
    std::uniform_int_distribution<int> dist{ 0, INT16_MAX };

    if(delay==true)
    {
        addCounter++;
        if(addCounter>addDelay) 
        {
            addCounter = 0;
            delay = false;
        }
    }

    for(int i=0;i<totalBlocks;i++)
    {
        blockArray[i]->update(*Weapons);
    
        if((blockArray[i]->state & block::BONUS)==block::BONUS)
        {
            blockArray[i]->bonusCounter++;
            if(blockArray[i]->bonusCounter>=current->bonusBlockTimeout)
            {
                blockArray[i]->bonusCounter = 0;
                if((dist(generator) % 2) == 0)
                //if((Math.abs(globals.rand.nextInt()) % 2)==0)
                {
                    blockArray[i]->state = blockArray[i]->state ^ block::BONUS;
                }
                else 
                {
                    blockArray[i]->state = block::DEAD;	
                    virtualBottom = blockArray[i]->positionY;

                    for(int j=0;j<totalBlocks;j++)
                    {
                        if(((blockArray[j]->state & block::STOPPED)==block::STOPPED))
                        {
                            if(blockArray[j]->positionY < blockArray[i]->positionY)
                                blockArray[j]->state = ((blockArray[j]->state ^ block::STOPPED) ^ block::MOVING);
                        }
                    }		
                }
            }
            else
            {
                // update block display .... (at intervals between 1 and 5!!)
                // actual .... umm just change colour of block for moment !
            }
        }
        // ****

        if((blockArray[i]->state & block::MOVING)==block::MOVING)
        {
            blockArray[i]->movementCounter++;
            if(blockArray[i]->movementCounter>current->blockDropInterval)
            {
                blockArray[i]->movementCounter = 0;
                blockArray[i]->positionY += current->blockDropSpeed +blockDropSpeedOffset;

                if(blockArray[i]->positionY>=virtualBottom)
                {				
                    blockArray[i]->state = (blockArray[i]->state ^ block::MOVING) ^ block::STOPPED;
                    virtualBottom -= (block::height + current->blockSpacing);
                }
            }
            // if stopped, its possible it will be destroyed! (and then all blocks with shuffle to bottom !)				
        }

        if((blockArray[i]->state & block::STOPPED)==block::STOPPED)
        {
            if(bl.checkCollision(*blockArray[i],columnIndex))
            {						
                if(bl.isLive(columnIndex))
                {
                    for(int doorwayIndex=0;doorwayIndex<dwlen;doorwayIndex++)
                    {
                        if(current->doorwayBanks[doorwayIndex]->shuffleEntrancesOrExits)
                        {
                            if((dist(generator) % 2) == 0)
                            //if((Math.abs(globals.rand.nextInt()) % 2)==0)
                                dw[doorwayIndex]->shuffleEntrances();
                            else
                                dw[doorwayIndex]->shuffleExits();
                            
                        }
                        else
                        {
                            if(current->doorwayBanks[doorwayIndex]->shuffleEntrances)
                                dw[doorwayIndex]->shuffleEntrances();
                                
                            if(current->doorwayBanks[doorwayIndex]->shuffleExits)
                                dw[doorwayIndex]->shuffleExits();
                        }
                    }
                
                    // ****
                    
                    u.score += blockArray[i]->score();

                    // *** BONUS START
                    if((blockArray[i]->state & block::BONUS)==block::BONUS)
                    {
                        // generate type of bonus block?

                            //int r = (Math.abs(globals.rand.nextInt()) % (effect::total - 1)) + 1;								
                            int r = (dist(generator) % (effect::total - 1)) + 1;
                            //if((dist(generator) % current->chanceOfBlock) == 0)

                            if(r==effect::RATEINC) u.Weapons->inc();
                            else if(r==effect::RATEDEC) u.Weapons->dec();
                            else if(r==effect::POINTS) u.score += current->bonusPoints;
                            else if(r==effect::CYCLE) 
                            {
                                for(int doorwayIndex=0;doorwayIndex<dwlen;doorwayIndex++)
                                {
                                    dw[doorwayIndex]->enableCycle();
                                }
                            }
                            else if(r==effect::TIMEINC) ct.inc(20000);
                            else if(r==effect::TIMEDEC) ct.dec(20000);
                            else if(r==effect::LEVEL)
                            {
                                if(ct.timeRemaining<60000)
                                { 
                                    ct.set(6000);
                                }
                                else 
                                {
                                    r = effect::POINTS;
                                    u.score += current->bonusPoints;
                                }
                            }
                            else if(r==effect::FASTER)
                            {
                                blockDropSpeedOffset++;
                                if(blockDropSpeedOffset>=current->maxBlockDropSpeed) --blockDropSpeedOffset;

                                // HORRIBLE CODE
                                weapon::userWeaponFireSpeedOffset++;
                                if(weapon::userWeaponFireSpeedOffset>=current->maxUserFireVerticalSpeed) --weapon::userWeaponFireSpeedOffset;
                                
                            }
                            
                            if(r==effect::LIFE)
                            {
                                if (u.lives<2) u.lives++;
                                else
                                {
                                    r = effect::POINTS;
                                    u.score += current->bonusPoints;
                                }
                            }
                
                            bEffects.add(columnIndex,r);                  
                    }
                    else if(((blockArray[i]->state & block::PERSIST)==block::PERSIST)&&(blockArray[i]->persistCounter>0))
                    {
                        blockArray[i]->persistCounter--;
                        //if(blockArray[i].persistCounter<=0) blockArray[i].state = block.DEAD;
                    }
                    
                    if((blockArray[i]->state & block::PERSIST)!=block::PERSIST)
                    {
                        blockArray[i]->state = block::DEAD;

                        virtualBottom = blockArray[i]->positionY;
                        for(int j=0;j<totalBlocks;j++)
                        {
                            if(((blockArray[j]->state & block::STOPPED)==block::STOPPED))
                            {
                                if(blockArray[j]->positionY < blockArray[i]->positionY)
                                    blockArray[j]->state = ((blockArray[j]->state ^ block::STOPPED) ^ block::MOVING);
                            }
                        }	
                    }
                    else if(blockArray[i]->persistCounter<=0)
                    {
                        blockArray[i]->state = block::DEAD;

                        virtualBottom = blockArray[i]->positionY;
                        for(int j=0;j<totalBlocks;j++)
                        {
                            if(((blockArray[j]->state & block::STOPPED)==block::STOPPED))
                            {
                                if(blockArray[j]->positionY < blockArray[i]->positionY)
                                    blockArray[j]->state = ((blockArray[j]->state ^ block::STOPPED) ^ block::MOVING);
                            }
                        }	
                    }

                    bl.kill(columnIndex);
                }

                if(((blockArray[i]->state & block::FIRE)==block::FIRE)&&(blockArray[i]->allowFire==false))
                {
                    
                    blockArray[i]->allowFire = true;
                    // or add block into fireBlockArray ???
                }
            }
        }
    }
    Weapons->updateSouth();		
}

void blocks::add()
{
    std::uniform_int_distribution<int> dist{ 0, INT16_MAX };

    if(delay==false)
    {
        delay = true;
        addCounter = 0;

        int i=0;

        while(((blockArray[i]->state & block::DEAD)!=block::DEAD)&&(i<totalBlocks-1))		
        {
            i++;
        }

        if((blockArray[i]->state & block::DEAD)==block::DEAD)
        {
            blockArray[i]->allowFire = false; blockArray[i]->fireCounter = 0;
            blockArray[i]->positionY = -block::height;
            blockArray[i]->positionX = offsetX;

            blockArray[i]->state = block::MOVING;
            blockArray[i]->movementCounter = 0;

            if((dist(generator) % current->chanceOfBonusBlock) == 0)
            //if((Math.abs(globals.rand.nextInt()) % current->chanceOfBonusBlock)==0)
            {
                blockArray[i]->state = blockArray[i]->state | block::BONUS;
                blockArray[i]->bonusCounter = 0;
            }
            else if((dist(generator) % current->chanceOfFireBlock) == 0)
            //else if((Math.abs(globals.rand.nextInt()) % current->chanceOfFireBlock)==0)
            {					
                blockArray[i]->state = blockArray[i]->state | block::FIRE;
            }
            else if((dist(generator) % current->chanceOfPersistBlock) == 0)
            //else if((Math.abs(globals.rand.nextInt()) % current->chanceOfPersistBlock)==0)
            {
                blockArray[i]->state = blockArray[i]->state | block::PERSIST;
                blockArray[i]->persistCounter = current->persistCount;
            }
        }
    }
}

bool blocks::isFull()
{
    //for(int i=0;i<totalBlocks;i++)
    //{
        //if((blockArray[i].state & block.DEAD & block.MOVING)==(block.DEAD & block.MOVING)) return false;			
        //if((blockArray[i].state & block.MOVING)==block.MOVING) return false;
        if(virtualBottom<0) return true;
    //}

    return false;
}

void blocks::draw(legacy::graphics &g)
{
    for(int i=0;i<totalBlocks;i++)
    {
        blockArray[i]->draw(g);
    }

    Weapons->draw(g);
}


void blocks::makeNull()
{
    blockArray = NULL;
    Weapons = NULL;
}

void blocks::cleanup()
{
    if(Weapons != NULL) delete Weapons;
    if(blockArray != NULL)
    {
        for(int i = totalBlocks - 1; i >= 0; --i)
        {
            if(blockArray[i] != NULL) delete blockArray[i];
        }
        delete[] blockArray;
    }
}
