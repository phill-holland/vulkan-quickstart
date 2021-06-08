#include "level.h"

void level::reset()
{
    init = false; cleanup();

    init = true;
}