#ifndef VECTOR_H
#define VECTOR_H

#include <string>
#include <stdexcept>

#ifndef VERY_SMALL
#define VERY_SMALL 1.0e-7f
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#ifndef DEGREES_TO_RADIANS
#define DEGREES_TO_RADIANS (M_PI / 180.0f)
#endif	

class Vector2
{
public:
	float x, y;

	inline void Load(float X, float Y) 
	{
		x = X; y = Y;
	}

	inline Vector2(float s = float(0.0)) 
	{
		Load(s, s);
	}

	inline Vector2(float X, float Y) 
	{
		Load(X, Y);
	}

	inline Vector2(const Vector2& v) 
	{
		Load(v.x, v.y);
	}
 
	inline Vector2& operator = (const Vector2& v) 
	{
		Load(v.x, v.y);
		return *this;
	}

	inline const float operator [] (int index) const
	{
		return *(&x + index);
	}

	inline float& operator [] (int index) 
	{
		return *(&x + index);					
	}

	inline const Vector2 operator + (const Vector2& v) const 
	{
		return Vector2(x + v.x, y + v.y);
	}

	// Add a vector to itself
	inline Vector2& operator += (const Vector2& v) 
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	// Take the negative of a vector
	inline const Vector2 operator - () const 
	{
		return Vector2(-x, -y);
	}

	// Subtract two vectors
	inline const Vector2 operator - (const Vector2& v) const 
	{
		return Vector2(x - v.x, y - v.y);
	}

	// Subtract a vector from itself
	inline Vector2& operator -= (const Vector2& v) 
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	// Multiply a vector by a scalar
	inline const Vector2  operator * (const float s) const 
	{
		return Vector2(s * x, s * y);
	}


	inline friend Vector2 operator * (const float s, const Vector2& v) 
	{
		return v * s;
	}

	inline Vector2& operator *= (const float s) 
	{
		x *= s;
		y *= s;
		return *this;
	}

	// Divide by a scalar - watch for divide by zero issues
	inline const Vector2 operator / (const float s) const 
	{
#ifdef _DEBUG
		if (fabs(s) < VERY_SMALL) {
			throw std::runtime_error("Divide by nearly zero! ");
		}
#endif
		float r = 1.0f / s;
		return *this * r;
	}


	inline Vector2& operator /= (const float s) 
	{
#ifdef _DEBUG
		if (fabs(s) < VERY_SMALL) {
			throw std::runtime_error("Divide by nearly zero! ");
		}
#endif // DEBUG
		float r = 1.0f / s;
		*this *= r;
		return *this;
	}

	inline bool operator==(const Vector2& Other) const
	{
		return (x == Other.x && y == Other.y);
	}

	inline bool operator!=(const Vector2& Other) const
	{
		return (x != Other.x || y != Other.y);
	}

	inline bool operator<(const Vector2& Other) const {
		return ((x < Other.x && y <= Other.y) || (x <= Other.x && y < Other.y));
	}

	inline bool operator>(const Vector2& Other) const {
		return ((x > Other.x && y >= Other.y) || (x >= Other.x && y > Other.y));
	}
};


#endif