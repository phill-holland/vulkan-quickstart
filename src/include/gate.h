#ifndef _GATE
#define _GATE

class gate
{
public:
    int totalDoorways;
    int seperatorHeight;

    int x,y; // in percentage

    bool shuffleEntrances;
    bool shuffleExits;
    bool shuffleEntrancesOrExits;
    
private:
    bool init;

public:
    gate() { makeNull(); reset(); }
    ~gate() { cleanup(); }

    bool initalised() { return init; }
    void reset();

protected:
    void makeNull() { }
    void cleanup() { }
};

#endif