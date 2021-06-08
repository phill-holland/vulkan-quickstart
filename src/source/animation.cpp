#include "animation.h"

void animation::reset(int count)
{
    init = false; cleanup();

    positionX = 0; positionY = 0;
    currentIndex = 0;

    totalSprites = count;
    Sprites = new sprite*[totalSprites];
    if(Sprites == NULL) return;

    for(int i=0;i<totalSprites;i++)
    {
        Sprites[i] = new sprite();
        if(Sprites[i] == NULL) return;
        if(!Sprites[i]->initalised()) return;
    }
                
    init = true;
}

void animation::setPosition(coordinate pos)
{
    positionX = pos.x;
    positionY = pos.y;
}

void animation::inc()
{
    currentIndex++;
    if(currentIndex>=totalSprites) currentIndex = 0;
}

void animation::dec()
{
    currentIndex--;
    if(currentIndex<0) currentIndex = totalSprites - 1;
}

bool animation::isCollision(animation &as)
{
    sprite *a = Sprites[0];
    sprite *b = as.Sprites[0];

    a->positionX = positionX; a->positionY = positionY;
    b->positionX = as.positionX; b->positionY = as.positionY;

    return a->isCollision(b);
}

bool animation::isCollision(sprite &b)
{
    sprite *a = Sprites[0];
    a->positionX = positionX; a->positionY = positionY;

    return a->isCollision(&b);
}

int animation::width()
{
    return Sprites[0]->width();
}

int animation::height()
{
    return Sprites[0]->height();
}

void animation::draw(legacy::graphics g)
{
    g.drawImage(Sprites[currentIndex]->picture,positionX,positionY,0);		
}

void animation::makeNull()
{
    Sprites = NULL;
}

void animation::cleanup()
{
    if(Sprites != NULL) 
    {
        for(int i = totalSprites - 1; i >= 0; --i)
        {
            if(Sprites[i] != NULL) delete Sprites[i];
        }

        delete[] Sprites;
    }
}