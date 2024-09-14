#pragma once

#include <cmath>
#include "VectorOperatorMixin.h"

/*!
 * @namespace Maths
 * @brief Contains many mathamatical quantities like vectors, matrices, etc.
 * @details Maths is a namespace that contains many mathamatical quantities like vectors, matrices, etc.
 * These quantities are used in many places in the engine.
 */
namespace Revid::Maths
{
	/*!
	 * @struct Vec4
	 * @brief Contains a 4D vector.
	 * @details Vec4 is a struct that contains a 4D vector. It contains x, y, z, and w values.
	 * @tparam T the type of the data stored. T should have a scalar multiplication and addition defined with float.
	 * This can be used for many purposes like position, color, etc. And supports many operations like addition, multiplication, etc.
	 */
	struct Vec4 : VectorOperationMixin<Vec4>
	{
		float x, y, z, w;

		Vec4()
		{
			Vec4(0.0);
		}

		Vec4(float x)
			: x(x)
			, y(x)
			, z(x)
			, w(x)
		{
		}

		Vec4(float x, float y, float z, float w)
		    : x(x)
		    , y(y)
		    , z(z)
		    , w(w)
		{
		}

		friend Vec4 operator*(const Vec4& a, float scalar) { return Vec4(a.x * scalar, a.y * scalar, a.z * scalar, a.w * scalar); }
		friend Vec4 operator+(const Vec4& a, const Vec4& other) { return Vec4(a.x + other.x, a.y + other.y, a.z + other.z, a.w + other.w); }
	};

	/*!
	 * @struct Vec3
	 * @brief Contains a 3D vector.
	 * @details Vec3 is a struct that contains a 3D vector. It contains x, y, and z values.
	 * This can be used for many purposes like position, color, etc. And supports many operations like addition, multiplication, etc.
	 */
	struct Vec3 : VectorOperationMixin<Vec3>
	{
		float x, y, z;

		Vec3()
		{
			Vec3(0.0f);
		}

		Vec3(float x)
			: x(x)
			, y(x)
			, z(x)
		{
		}

		Vec3(float x, float y, float z)
		    : x(x)
		    , y(y)
		    , z(z)
		{
		}

		Vec3(const Vec3& other)
		    : x(other.x)
		    , y(other.y)
		    , z(other.z)
		{
		}

		friend Vec3 operator*(const Vec3& a, float scalar) { return Vec3(a.x * scalar, a.y * scalar, a.z * scalar); }
		friend Vec3 operator+(const Vec3& a, const Vec3& other) { return Vec3(a.x + other.x, a.y + other.y, a.z + other.z); }
	};

	/*!
	 * @struct Vec2
	 * @brief Contains a 2D vector.
	 * @details Vec2 is a struct that contains a 2D vector. It contains x and y values.
	 * This can be used for many purposes like texture coordinates, 2D coordinates etc. And supports many operations like addition, multiplication, etc.
	 */
	struct Vec2 : VectorOperationMixin<Vec2>
	{
		float x, y;

		Vec2()
		    : x(0.0f)
		    , y(0.0f)
		{
		}

		Vec2(float x, float y)
		    : x(x)
		    , y(y)
		{
		}

		friend Vec2 operator*(const Vec2& curr, float scalar) { return Vec2(curr.x * scalar, curr.y * scalar); }
		friend Vec2 operator+(const Vec2& curr , const Vec2& other) { return Vec2(curr.x + other.x, curr.y + other.y); }

		Vec2 Perpendicular() { return Vec2(-y, x); }

		Vec2 Normalize()
		{
			float length = sqrt(x * x + y * y);
			return Vec2(x / length, y / length);
		}
	};
}