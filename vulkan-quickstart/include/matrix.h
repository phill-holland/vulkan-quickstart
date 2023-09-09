#include "vector.h"

#ifndef _VULKAN_MATRIX
#define _VULKAN_MATRIX

namespace vulkan
{
    namespace primatives
    {
        namespace matrices
        {
            class matrix4x4
            {
            public:
                float values[4][4];

            public:
                matrix4x4() { clear(); }
                matrix4x4(const matrix4x4 &source) { copy(source); }

            public:
                void clear();            

            public:
                void set(float value, int x, int y) 
                { 
                    values[y][x] = value; 
                }
                float get(int x, int y) 
                { 
                    return values[y][x]; 
                }

                void row(float a, float b, float c, float d, int y) 
                { 
                    values[y][0] = a; 
                    values[y][1] = b; 
                    values[y][2] = c; 
                    values[y][3] = d; 
                }

            public:
                void copy(const matrix4x4 &source);

            public:
                matrix4x4& operator=(const matrix4x4& source);
                matrix4x4 operator*(const matrix4x4 &r);
                vector4 operator*(const vector4 &r);            
            };        

            class translation : public matrix4x4
            {
            public:
                translation(const vector3 &source)
                {
                    row(-source.x,-source.y,-source.z,1,3);
                }
            };
        };
    };
};

#endif