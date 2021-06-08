#include "xywz.h"

#ifndef _GEOMETRY
#define _GEOMETRY

class geometry
{
public:
    virtual xywz<float> getVertex() = 0;
    virtual unsigned long getVertexLength() = 0;

    virtual int getTextureShaderIdx() = 0;
    virtual int getVertexShaderIdx() = 0;
};

class shader
{
    int idx;

public:
    virtual char *getShader() = 0;
};

#endif