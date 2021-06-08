#include "menu.h"
#include "globals.h"

string menu::filenames[] = {string("images/sadistic.jpg"),
                            string("images/begin.jpg"),
                            string("images/copyright.jpg")};

void menu::reset()
{
    init = false; cleanup();

    hiScoreCounter = 0; beginCounter = 0;

    finished = false;
    displayScores = false;
    displayBegin = false;

    imgs = new images(filenames, 3);
    if(imgs == NULL) return;
    if(!imgs->initalised()) return;

    init = true;
}

void menu::keyPressed(int e) { }

void menu::keyReleased(int e)
{
    //if(e==GameCanvas.FIRE) finished = true;				
}

bool menu::update()
{
    hiScoreCounter++;
    if(hiScoreCounter>hiScoreTimeout)
    {
        hiScoreCounter = 0;
        if(displayScores==false) displayScores = true;
            //else displayScores = false;
    }

    beginCounter++;
    if(beginCounter>beginTimeout)
    {
        beginCounter = 0;
        if(displayBegin==false) displayBegin = true;
            else displayBegin = false;
    }

    return finished;		
}

void menu::draw(legacy::graphics g)
{
    g.setColor(0);
    g.fillRect(0,0,globals::maxWidth,globals::maxHeight);

    if(displayScores)
    {
        //g.setColor(0xFFFFFF);
        //globals.scores.draw(g);
        /*
        if(globals::Credits.update()==true)
        {
            displayScores = false;
            hiScoreCounter = 0;
        }
        globals::Credits.draw(g);
        */
    }
    else
    {
        //globals::plasmaBackground.draw(g);
        g.drawImage(imgs->get(0),6,40,0);
        g.drawImage(imgs->get(2),8,170,0);

        if(displayBegin==true)
        {
            g.drawImage(imgs->get(1),25,100,0);
        }
    }
}

void menu::makeNull()
{
    imgs = NULL;
}

void menu::cleanup()
{
    if(imgs != NULL) delete imgs;
}