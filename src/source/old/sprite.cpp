#include "sprite.h"

void sprite::reset()
{
    init = false; cleanup();

    positionX = 0;
    positionY = 0;
    state = 0;

    init = true;
}

void sprite::set(image i)
{
    picture = i;

    positionX = 0;
    positionY = 0;
    state = 0;
}

int sprite::width()
{
    return picture.getWidth();
}

int sprite::height()
{
    return picture.getHeight();
}

bool sprite::isCollision(legacy::region *r)
{
    if((positionX > (r->x + r->width))||(r->x > (positionX + width())))
        return false;

    if((positionY > (r->y + r->height))||(r->y > (positionY + height())))
        return false;

    return true;		
}

bool sprite::isCollision(sprite *s)
{
    if((positionX > (s->positionX + s->width()))||(s->positionX > (positionX + width())))
        return false;

    if((positionY > (s->positionY + s->height()))||(s->positionY > (positionY + height())))
        return false;

    return true;
}

bool sprite::isInside(int x,int y)
{
    if((x>=positionX)&&(x<=(positionX + width())))
        if((y>=positionY)&&(y<=(positionY + height()))) return true;

    return false;
}
