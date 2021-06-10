#include "xyzw.h"

#ifndef _VECTOR
#define _VECTOR

class vector : public xyzw<float>
{
public:
	vector(float x1 = 0.0f,float y1 = 0.0f,float z1 = 0.0f,float w1 = 0.0f) { x = x1; y = y1; z = z1; w = w1; }
	
	void reset() { x = y = z = w = 0.0f; }

	float length();
	
	float dot(vector &src);
	vector cross(vector &src);

	float angle(vector &src, bool normalised = false);	
	vector axis(vector &src, bool normalised = false);
	
	void normalise();
	
	bool nowhere();

	bool decode(long index);	
	long encode();
	
public:
	operator xyzw<long>() const { return xyzw<long>((long)x, (long)y, (long)z, 0L); }

public:
	vector operator=(const vector &src) { copy((vector&)src); return *this; }
	vector operator*(const vector &src) { return vector(y * src.z - z * src.y,z * src.x - x * src.z,x * src.y - y * src.x, 0.0f); }        
	vector operator+(const vector &src) { return vector(x + src.x,y + src.y,z + src.z, 0.0f); }
	vector operator-(const vector &src) { return vector(x - src.x,y - src.y,z - src.z, 0.0f); }
	vector operator*(float r)  { return vector(x * r,y * r,z, 0.0f); }
	vector operator/(float r) { return vector(x / r,y / r,z / r, 0.0f); }
};

#endif
