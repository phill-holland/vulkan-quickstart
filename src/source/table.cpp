#include "table.h"

void table::reset()
{
    init = false; cleanup();

    scores = new score*[maxNames];
    if(scores == NULL) return;

    for(int i=0; i < maxNames; ++i) scores[i] = NULL;

    currentCharacter = (int)'A';
    currentName = "";

    int currentScore = 4000;
    int levelIndex = 10;

    for(int i=0;i<maxNames;i++)
    {
        string character = 'a';//String.valueOf((char)((int)'A' + i));						
        string name = character + character + character;
        
        scores[i] = new score(name,levelIndex--,currentScore);
        if(scores[i] == NULL) return;
        //if(!scores[i]->initalised()) return;

        currentScore -= 300;
    }

    up = false; down = false; inputMode = false;

    init = true;
}

int table::isHighScore(int points)
{
    for(int i=(maxNames-1);i>=0;i--)
    {
        if(points>=scores[i]->points) return i;
    }

    return -1;
}

void table::add(string name,int levelIndex,int points)
{
    int i=0;
    bool stop=false;

    do
    {
        if(points>scores[i]->points)
        {
            stop = true;
        }
        else i++;

    }while((i<maxNames)||(stop==true));		

    // ****

    int position = i;

    for(i=position;i<maxNames-1;i++)
    {
        scores[i+1]->points = scores[i]->points;
        scores[i+1]->levelIndex = scores[i]->levelIndex;
        scores[i+1]->name = scores[i]->name;
    }

    scores[position]->points = points;
    scores[position]->levelIndex = levelIndex;
    scores[position]->name = name;

}

bool table::update()
{
    if(up==true)
    {
        currentCharacter++;
        if(currentCharacter>(int)'Z') currentCharacter = (int)'A';
    }
    else if(down==true)
    {
        --currentCharacter;
        if(currentCharacter<(int)'A') currentCharacter = (int)'Z';
    }

    if(currentName.length()>=3)
    {
        inputMode = false;
        return false;
    }
    else return true;
}

void table::keyPressed(int e)
{
    /*
    if(e==GameCanvas.UP)
    {
        if(down==false)
        {
            up = true;
        }
    }
    if(e==GameCanvas.DOWN)
    {
        if(up==false)
        {
            down = true;
        }		
    }
    */
}

void table::keyReleased(int e)
{
    /*
    up = false;
    down = false;

    if(e==GameCanvas.FIRE)
    {
        currentName += String.valueOf((char)(currentCharacter));
    }
    */
}

void table::draw(legacy::graphics g)
{
    /*
    int yOffset = 50;

    g.drawImage(globals::dFont.draw("hi scores"),45,5,0);

    g.drawImage(globals::dFont.draw("scores"),5,30,0);
    g.drawImage(globals::dFont.draw("level"),70,30,0);
    g.drawImage(globals::dFont.draw("name"),130,30,0);

    for(int i=0;i<maxNames;i++)
    {
        score *currentScore = scores[i];

        g.drawImage(globals::dFont.draw(Integer.toString(currentScore.points)),10,yOffset,0);
        g.drawImage(globals::dFont.draw(Integer.toString(currentScore.levelIndex)),83,yOffset,0);
        g.drawImage(globals::dFont.draw(currentScore.name),133,yOffset,0);

        yOffset += 11;
    }

    if(inputMode==true)
    {
        g.drawString("ENTER INITIALS : ___",20,190,0);
        g.drawString("                 " + currentName + String.valueOf((char)(currentCharacter)),20,190,0);
    }
    */
}

void table::makeNull()
{
    scores = NULL;
}

void table::cleanup()
{
    if(scores != NULL)
    {
        for(int i=maxNames - 1; i>=0;--i)
        {
            if(scores[i] != NULL) delete scores[i];
        }

        delete[] scores;
    }
}