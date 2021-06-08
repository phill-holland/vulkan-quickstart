#include "gate.h"

void gate::reset()
{
    init = false; cleanup();

    init = true;
}