#include "matrix.h"

void vulkan::primatives::matrices::matrix4x4::clear()
{    
    for(int y=0;y<4;++y)
    {
        values[y][0] = 0.0f;
        values[y][1] = 0.0f;
        values[y][2] = 0.0f;
        values[y][3] = 0.0f;
    }
}

void vulkan::primatives::matrices::matrix4x4::copy(const matrix4x4 &source)
{
    for(int y=0;y<4;++y)
    {
        values[y][0] = source.values[y][0];
        values[y][1] = source.values[y][1];
        values[y][2] = source.values[y][2];
        values[y][3] = source.values[y][3];
    }
}
vulkan::primatives::matrices::matrix4x4& vulkan::primatives::matrices::matrix4x4::operator=(const matrix4x4& source)
{		
    this->copy(source);
    return *this;
}

vulkan::primatives::matrices::matrix4x4 vulkan::primatives::matrices::matrix4x4::operator*(const matrix4x4 &r)
{
    matrix4x4 temp;
    
    for(int y=0;y<4;++y)
    {
        temp.values[y][0] += (values[y][0] * r.values[0][0]);
        temp.values[y][0] += (values[y][1] * r.values[1][0]);
        temp.values[y][0] += (values[y][2] * r.values[2][0]);
        temp.values[y][0] += (values[y][3] * r.values[3][0]);

        temp.values[y][1] += (values[y][0] * r.values[0][1]);
        temp.values[y][1] += (values[y][1] * r.values[1][1]);
        temp.values[y][1] += (values[y][2] * r.values[2][1]);
        temp.values[y][1] += (values[y][3] * r.values[3][1]);

        temp.values[y][2] += (values[y][0] * r.values[0][2]);
        temp.values[y][2] += (values[y][1] * r.values[1][2]);
        temp.values[y][2] += (values[y][2] * r.values[2][2]);
        temp.values[y][2] += (values[y][3] * r.values[3][2]);

        temp.values[y][3] += (values[y][0] * r.values[0][3]);
        temp.values[y][3] += (values[y][1] * r.values[1][3]);
        temp.values[y][3] += (values[y][2] * r.values[2][3]);
        temp.values[y][3] += (values[y][3] * r.values[3][3]);
    }
    
    return temp;
}

vulkan::primatives::vector4 vulkan::primatives::matrices::matrix4x4::operator*(const vector4 &r) 
{ 
    return vector4(r.x * get(0,0) + r.y * get(0,1) + r.z * get(0,2) + r.w * get(0,3),
                   r.x * get(1,0) + r.y * get(1,1) + r.z * get(1,2) + r.w * get(1,3),
                   r.x * get(2,0) + r.y * get(2,1) + r.z * get(2,2) + r.w * get(2,3),
                   r.x * get(3,0) + r.y * get(3,1) + r.z * get(3,2) + r.w * get(3,3)); 
}