#include "credits.h"
#include "globals.h"

string credits::pageOne[]= {"SADISTIC"," "," ","DEVELOPED"," "," ","AT"," "," ","OWD STUDIOS"," "," ","ENGLAND"};
string credits::pageTwo[] = {"ORIGINAL IDEA"," "," ","PHILL HOLLAND"," "," ","GAME DESIGN"," "," ","PAUL PARROT"," "," ","LEVEL DESIGN "," "," ","ALAN AARVARK"};
string credits::pageThree[] = {"GRAPHICS"," "," ","PHILL HOLLAND"," "," ","SOUND"," "," ","GARY GOPHER"," "," ","MUSIC"," "," ","STEVE BABOON"};
string credits::pageFour[] = {"PROGRAMMING"," "," ","PHILL HOLLAND"," "," ","RESEARCH"," "," ","KASIA KITTEN"," "," ","STORYLINE"," "," ","DAVE DONKEY"};
string credits::pageFive[] = {"DISTRIBUTION"," "," ","RICHARD EMU"," "," ","DIALOGUE"," "," ","SARAH SHEEP"," "," ","CREDITS"," "," ","KEVIN KIPPER"};

void credits::reset()
{
    init = false; cleanup();

    delayTimeout = 150;
	delayCounter = 0;

	leftXOffset = 0;
	rightXOffset = 0;

    temp = new int[globals::maxWidth * globals::maxHeight];
    if(temp == NULL) return;
    //black = new int[globals.maxWidth * globals.maxHeight];

    leftWipe = image::createImage(globals::maxWidth,globals::maxHeight);
    rightWipe = image::createImage(globals::maxWidth,globals::maxHeight);
            
    currentPage = 0;
    delayCounter = 0;

    wipeScreen = true; delayScreen = false;

    leftXOffset = -globals::maxWidth;
    rightXOffset = globals::maxWidth;

   // gridX = globals::maxWidth / globals::dFont.characterWidth ;
    //gridY = globals::maxHeight / globals::dFont.characterHeight;

    clear();
    drawPage(currentPage);

    init = true;
}

void credits::clear()
{
    /*
    graphics g = leftWipe->getGraphics();
    g.setColor(0x00000000);
    g.fillRect(0,0,globals::maxWidth,globals::maxHeight);


    g = rightWipe->getGraphics();
    g.setColor(0x00000000);
    g.fillRect(0,0,globals::maxWidth,globals::maxHeight);
    */
}

void credits::drawPage(int pageNum)
{
    /*
    clear();

    graphics left = leftWipe->getGraphics();
    graphics right = rightWipe->getGraphics();

    pageStrings pg = pages[pageNum];


    int startGridX = 0;
    int startGridY = (gridY / 2) - (pg.lines / 2);

    for(int i=0;i<pg.lines;i++)
    {
        startGridX = (gridX / 2) - (pg.page[i].length() / 2);

        if((i % 2)==0) left.drawImage(globals::dFont.draw(pg.page[i]),startGridX * globals::dFont.characterWidth,startGridY * globals.dFont.characterHeight,0);
            else right.drawImage(globals::dFont.draw(pg.page[i]),startGridX * globals::dFont.characterWidth,startGridY * globals.dFont.characterHeight,0);

        startGridY++;
    }
    
    rightWipe->getRGB(temp,0,globals::maxWidth,0,0,globals::maxWidth,globals::maxHeight);
    for(int i=0;i<globals::maxWidth * globals::maxHeight;i++)
    {
        if(temp[i]==0xFF000000)	temp[i] = temp[i] ^ 0XFF000000;
    }
    */
}

bool credits::update()
{
    bool result = false;
/*
    if(wipeScreen==true)
    {
        if(leftXOffset<0)
        {			
            leftXOffset+=2;
            rightXOffset-=2;
        }
        else
        {
            wipeScreen = false;
            delayScreen = true;
            delayCounter = 0;
        }
    }
    else if(delayScreen==true)
    {
        delayCounter++;
        if(delayCounter>=delayTimeout)
        {
            delayScreen = false;
            wipeScreen = true;
            leftXOffset = -globals::maxWidth;
            rightXOffset = globals::maxWidth;
            currentPage++; if(currentPage>=pages.length) { currentPage = 0; result = true; }
            drawPage(currentPage);
        }			
    }
*/
    return result;
}

void credits::draw(legacy::graphics g)
{
    //g.drawImage(leftWipe,leftXOffset,0,0);
    //g.drawRGB(temp,0,globals::maxWidth,rightXOffset,0,globals::maxWidth,globals::maxHeight,true);
}

void credits::makeNull()
{
    leftWipe = NULL;
    rightWipe = NULL;
    temp = NULL;
}

void credits::cleanup()
{
    if(temp != NULL) delete[] temp;
    if(rightWipe != NULL) delete rightWipe;
    if(leftWipe != NULL) delete leftWipe;
}