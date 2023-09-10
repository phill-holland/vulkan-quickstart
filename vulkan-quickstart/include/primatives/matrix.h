#include "vector.h"
#include <math.h>

#ifndef _VULKAN_PRIMATIVE_MATRIX
#define _VULKAN_PRIMATIVE_MATRIX

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
                    values[x][y] = value; 
                }

                float get(int x, int y) 
                { 
                    return values[x][y]; 
                }

                void row(float a, float b, float c, float d, int x) 
                { 
                    values[0][x] = a; 
                    values[1][x] = b; 
                    values[2][x] = c; 
                    values[3][x] = d; 
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
                    row(source.x, source.y, source.z, 1, 3);
                }
            };

            class projection : public matrix4x4
            {
            public:
                projection(const float fov, const float ratio, 
                           const float near, const float far);                
            };
        };
    };
};

#endif