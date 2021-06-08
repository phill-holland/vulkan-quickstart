#include "images.h"

void images::reset(int e)
{
    init = false; cleanup();

    elements = e; counter = 0;

    data = new image*[elements];
    if (data == NULL) return;

    for(int i=0;i<elements;++i) data[i] = NULL;

    init = true;
}

void images::reset(string *s, int len)
{
    init = false; cleanup();

    elements = len; counter = 0;

    data = new image*[elements];
    if (data == NULL) return;

    for(int i=0;i<elements;++i) data[i] = NULL;

    for(int i=0;i<elements;i++)
    {
        load(s[i],i);
    }

    init = true;
}

image images::get(int index)
{
    return *data[index];
}

void images::load(string filename,int index)
{
    data[index] = image::createImage(filename);
}

void images::load(string filename)
{
    data[counter++] = image::createImage(filename);
}

void images::makeNull()
{
    data = NULL;
}

void images::cleanup()
{
    if(data != NULL)
    {
        for(int i=elements - 1;i >= 0;--i) 
        {
            if (data[i] != NULL) delete data[i];
        }

        delete[] data;
    }
}
