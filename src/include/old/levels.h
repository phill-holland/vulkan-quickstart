#include "thread.h"

#ifndef _LEVELS
#define _LEVELS

/* inherits from thread, manages loading levels, and ending levels, and level transistions */

class levels
{
    bool init;

public:
    levels() { makeNull(); reset(); }
    ~levels() { cleanup(); }

    bool initalised() { return init; }
    void reset() { }

protected:
    void makeNull() { }
    void cleanup();
};

#endif