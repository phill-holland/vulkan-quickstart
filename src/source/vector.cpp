#include "vector.h"
#include <math.h>
#include <stdlib.h>

float vector::length()
{
	return sqrtf(x * x + y * y + z * z);
}

float vector::dot(vector &src)
{
	return x * src.x + y * src.y + z * src.z;
}

vector vector::cross(vector &src)
{
	return vector(y * src.z - z * src.y, z * src.x - x * src.z, x * src.y - y * src.x, 0.0f);
}

float vector::angle(vector &src, bool normalised)
{
	vector a = *this, b = src;

	if (!normalised)
	{
		a.normalise();
		b.normalise();
	}

	return (float)acos(a.dot(b));
}

vector vector::axis(vector &src, bool normalised)
{
	vector a = *this, b = src;

	if (!normalised)
	{
		a.normalise();
		b.normalise();
	}

	a = a.cross(b);
	a.normalise();

	return a;
}

void vector::normalise()
{
	float t = 1.0f / (length() + 0.001f);
	x *= t; y *= t; z *= t;
}

bool vector::nowhere()
{
	if ((long)(x * 1000.0f) != 0L) return false;
	if ((long)(y * 1000.0f) != 0L) return false;
	if ((long)(z * 1000.0f) != 0L) return false;

	return true;
}

bool vector::decode(long index)
{
	if ((index >= 0L) && (index <= 27L))
	{
		ldiv_t r = div(index, 9L);
		z = (float)r.quot - 1L;

		ldiv_t j = div(r.rem, 3L);
		y = (float)j.quot - 1L;
		x = (float)j.rem - 1L;
		w = 0.0f;

		return true;
	}

	return false;
}

long vector::encode()
{
	long tx = (long)roundf(x) + 1L;
	long ty = (long)roundf(y) + 1L;
	long tz = (long)roundf(z) + 1L;

	if ((tx < 0L) || (tx > 2L)) return 0L;
	if ((ty < 0L) || (ty > 2L)) return 0L;
	if ((tz < 0L) || (tz > 2L)) return 0L;

	return (abs(tz) * (3L * 3L)) + (abs(ty) * 3L) + abs(tx);
}

