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

                void row(float a, float b, float c, float d, int y) 
                { 
                    values[y][0] = a; 
                    values[y][1] = b; 
                    values[y][2] = c; 
                    values[y][3] = d; 
                }

                void identity()
                {
                    values[0][0] = 1.0f;
                    values[1][1] = 1.0f;
                    values[2][2] = 1.0f;
                    values[3][3] = 1.0f;
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
                    identity();
                                        
                    values[3][0] = source.x;
                    values[3][1] = source.y;
                    values[3][2] = source.z;
                }

            public:
                void x(float value) { values[3][0] = value; }
                void y(float value) { values[3][1] = value; }
                void z(float value) { values[3][2] = value; }
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