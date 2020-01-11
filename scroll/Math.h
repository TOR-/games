#pragma once

#include <cmath>
#include <memory.h>
#include <limits>

namespace Math
{
	const float PI      = 3.1415926535f;
	const float TWO_PI  = PI * 2.0f;
	const float HALF_PI = PI / 2.0f;
	const float INF     =  std::numeric_limits<float>::infinity();
	const float NEGINF  = -std::numeric_limits<float>::infinity();

	inline float deg_to_rad(float deg) { return deg * PI / 180.0f; }
	inline float rad_to_deg(float rad) { return rad * 180.0f / PI; }
	inline bool near_zero(float f, float err = 0.001f)
	{
		if (fabs(f) <= err)
			return true;
		else
			return false;
	}

	inline float sqrt(float value)
	{
		return sqrtf(value);
	}
}

class Vector2
{
	public:
		float x;
		float y;

		Vector2(): x(0.0f), y(0.0f) {}

		explicit Vector2(float inX, float inY): x(inX), y(inY) {}

		void set(float inX, float inY)
		{
			x = inX;
			y = inY;
		}

		friend Vector2 operator+(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x + b.x, a.y + b.y);
		}

		friend Vector2 operator-(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x - b.x, a.y - b.y);
		}

		// Vector product
		friend Vector2 operator*(const Vector2& a, const Vector2& b)
		{
			return Vector2(a.x * b.x, a.y * b.y);
		}

		// Scalar product
		friend Vector2 operator*(const Vector2& vec, float scalar)
		{
			return Vector2(vec.x * scalar, vec.y * scalar);
		}

		// Scalar product
		friend Vector2 operator*(float scalar, const Vector2& vec)
		{
			return Vector2(vec.x * scalar, vec.y * scalar);
		}

		Vector2& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		Vector2& operator+=(const Vector2& right)
		{
			x += right.x;
			y += right.y;
			return *this;
		}

		Vector2& operator-=(const Vector2& right)
		{
			x -= right.x;
			y -= right.y;
			return *this;
		}
		// Length squared of vector
		float LengthSq() const
		{
			return (x*x + y*y);
		}

		// Length of vector
		float Length() const
		{
			return (Math::sqrt(LengthSq()));
		}

		// Normalize this vector
		void Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
		}

		// Normalize the provided vector
		static Vector2 Normalize(const Vector2& vec)
		{
			Vector2 temp = vec;
			temp.Normalize();
			return temp;
		}

		// Dot product between two vectors (a dot b)
		static float Dot(const Vector2& a, const Vector2& b)
		{
			return (a.x * b.x + a.y * b.y);
		}

		// Lerp from A to B by f
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float f)
		{
			return Vector2(a + f * (b - a));
		}

		// Reflect V about (normalized) N
		static Vector2 Reflect(const Vector2& v, const Vector2& n)
		{
			return v - 2.0f * Vector2::Dot(v, n) * n;
		}

		// Transform vector by matrix
		static Vector2 Transform(const Vector2& vec, const class Matrix3& mat, float w = 1.0f);

		static const Vector2 Zero;
		static const Vector2 UnitX;
		static const Vector2 UnitY;
		static const Vector2 NegUnitX;
		static const Vector2 NegUnitY;
};
