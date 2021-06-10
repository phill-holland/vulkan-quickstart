#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include "extensions.h"

#ifndef _CUBE
#define _CUBE

class cube
{
    static const char *texture_vertex_source;
    static const char *texture_fragment_source;

    GLenum program;
	GLenum fragment, vertex;

    bool init;

public:
    cube() { makeNull(); reset(); }
    ~cube() { cleanup(); }

    bool initalised() { return init; }
    void reset();

    void render();

protected:
    bool createOpenGL();
    void errorGL();
    bool compile(const char *source, GLenum type, GLenum &result);

protected:
    void makeNull();
    void cleanup();
};

#endif