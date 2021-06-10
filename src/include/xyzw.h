//#include "properties.h"

#ifndef _XYZW
#define _XYZW

template <class X> class xyzw
{
public:
	X x, y, z, w;

public:
	xyzw() { x = (X)0; y = (X)0; z = (X)0; w = (X)0; }
	xyzw(X x1, X y1, X z1) { x = x1; y = y1; z = z1; }
	xyzw(X x1, X y1, X z1, X w1) { x = x1; y = y1; z = z1; w = w1; }
	xyzw(const xyzw<X> &source) { copy(source); }

	void copy(const xyzw<X> &src) 
	{ 
		x = src.x; 
		y = src.y; 
		z = src.z; 
		w = src.w; 
	}

	xyzw<X> operator=(const xyzw<X> &src) { copy(src); return *this; }
	xyzw<X> operator+(const xyzw<X> &src) { return xyzw<X>(x + src.x, y + src.y, z + src.z); }
	xyzw<X> operator-(const xyzw<X> &src) { return xyzw<X>(x - src.x, y - src.y, z - src.z); }
	xyzw<X> operator*(const xyzw<X> &src) { return xyzw<X>(x * src.x, y * src.y, z * src.z); }

/*
	bool operator==(const xyzw<X>& rhs) const 
	{
		return std::tie(x, y, z, w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
	}
		
	constexpr static auto properties = std::make_tuple(
			json::property(&xyzw::x, "x"),
			json::property(&xyzw::y, "y"),
			json::property(&xyzw::z, "z"),
			json::property(&xyzw::w, "w")
		);
		*/
};

#endif