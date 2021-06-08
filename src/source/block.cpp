#include "block.h"

string block::filenames[4] = { string("/images/blue.jpg"),
                               string("/images/green.jpg"),
                               string("/images/red.jpg"),
                               string("/images/yellow.jpg") };

void block::reset(legacy::level *l)
{
    init = false; cleanup();

    current = l;

    persistCounter = current->persistCount;
    positionX = 0; positionY = 0;
    bonusCounter = 0; movementCounter = 0;
    state = DEAD;		

    allowFire = false;
    fireCounter = 0;

    cubes = new images(filenames, 4);
    if(cubes == NULL) return;
    if(!cubes->initalised()) return;

    picture = cubes->get(NORMAL_IMG);

    init = true;
}

void block::update(weapons &w)
{
    if(((state & FIRE)==FIRE)&&((state & DEAD)!=DEAD))
    {
        if(allowFire)
        {
            fireCounter++;
            if(fireCounter>current->returnFireInterval)
            {
                fireCounter = 0;
                w.add(positionX + (width / 2),positionY + height);				
            }
        }
    }
}

int block::score()
{
    if((state & BONUS)==BONUS) return BONUS_SCORE;
    else if((state & PERSIST)==PERSIST) return PERSIST_SCORE;
    else if((state & FIRE)==FIRE) return FIRE_SCORE;
    else if((state & DEAD)!=DEAD) return NORMAL_SCORE;

    return 0;
}

void block::draw(legacy::graphics &g)
{
    if((state & DEAD)!=DEAD)
    {
        if((state & BONUS)==BONUS)
            picture = cubes->get(BONUS_IMG);
        else if ((state & PERSIST)==PERSIST)
            picture = cubes->get(PERSIST_IMG);
        else if((state & FIRE)==FIRE)
            picture = cubes->get(FIRE_IMG);	
        else 
            picture = cubes->get(NORMAL_IMG);

        sprite::draw(g);
    }
}

void block::makeNull()
{
    cubes = NULL;
}

void block::cleanup()
{
    if (cubes != NULL) delete cubes;
}