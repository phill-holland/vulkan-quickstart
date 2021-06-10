#ifndef _SHADER
#define _SHADER

class shader
{
public:
    enum type { TEXTURE = 0, VERTEX = 1 };

public:
    virtual char *getShader() = 0;
    virtual type getType() = 0;
};

#endif