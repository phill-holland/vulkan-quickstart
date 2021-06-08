#ifndef _AUDIO
#define _AUDIO

class audio
{
    bool init;
public:
    audio() { makeNull(); reset(); }
    ~audio() { cleanup(); }

    bool intialised() { return init; }
    void reset();

    void go();

protected:
    void makeNull();
    void cleanup();
};

#endif