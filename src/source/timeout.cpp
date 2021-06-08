#include "timeout.h"
#include "globals.h"

void timeout::reset()
{
    startTime = 0;
    timeRemaining = 0;
    previousTime = 0;
    flashCounter = 0;
    red = false;
}

void timeout::start(int mm,int ss)
{
    reset();

    timeRemaining = ((mm * 60) + ss) * 1000;
    //startTime = System.currentTimeMillis();
    previousTime = startTime;
}

void timeout::start(int ms)
{
    reset();
    timeRemaining = ms;
    //startTime = System.currentTimeMillis();
}

void timeout::update()
{
    if(timeRemaining!=0)
    {
        long currentTime = 0;//System.currentTimeMillis();
        //System.out.println(currentTime - previousTime);
        timeRemaining -= (currentTime - previousTime);

        if(timeRemaining < 0) timeRemaining = 0;
        previousTime = currentTime;
    }
}

bool timeout::isFinished() { return (timeRemaining==0); }

void timeout::set(int ms)
{
    if((timeRemaining - ms)>0) timeRemaining = ms;		
}

void timeout::dec(int ms)
{
    if(timeRemaining > ms) timeRemaining -= ms;
}

void timeout::inc(int ms)
{
    timeRemaining += ms;
}

void timeout::draw(legacy::graphics g)
{
    string output("00m00s");		

    if(timeRemaining>0)
    {
        long seconds = timeRemaining / 1000;
        long minutes = seconds / 60;
        seconds = seconds % 60;

        string min ;//= "00" + Integer.toString((int)minutes);
       // min = min.substring(min.length()-2,min.length());
        
        string sec;// = "00" + Integer.toString((int)seconds);
        //sec = sec.substring(sec.length()-2,sec.length());

        output = min + "m" + sec + "s";
    }		
    
    if(timeRemaining<10000)
    {			
        flashCounter++;
        if((flashCounter % flashInterval)==0)
        {				
            if(red==false) red = true;
            else red = false;
            //System.out.println("moo" + red);
        }
    }
    
    int r = 255; int grn = 0; int b = 0;

    if(red==false) { r= 165; grn = 104; b = 4; }
    g.setColor(r,grn,b);

    g.fillRect(0,globals::maxHeight - 11,globals::maxWidth,11); // -30 - was 30 !

   // g.drawImage(globals::dFont.draw(output,r,grn,b),globals::maxWidth - 50,globals::maxHeight - 10,0); //  was 27 !1
}