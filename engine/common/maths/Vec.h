#pragma once

#include <cmath>
#include <glm/glm.hpp>

/*!
 * @namespace Maths
 * @brief Contains many mathamatical quantities like vectors, matrices, etc.
 * @details Maths is a namespace that contains many mathamatical quantities like vectors, matrices, etc.
 * These quantities are used in many places in the engine.
 */
namespace Vivid::Maths
{
	/*!
	 * @struct Vec4
	 * @brief Contains a 4D vector.
	 * @details Vec4 is a struct that contains a 4D vector. It contains x, y, z, and w values.
	 * This can be used for many purposes like position, color, etc. And supports many operations like addition, multiplication, etc.
	 */
	struct Vec4
	{
		float x, y, z, w;

		Vec4()
		    : x(0.0f)
		    , y(0.0f)
		    , z(0.0f)
		    , w(0.0f)
		{
		}

		Vec4(float x, float y, float z, float w)
		    : x(x)
		    , y(y)
		    , z(z)
		    , w(w)
		{
		}

		Vec4 operator*(float scalar) { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }
		Vec4 operator+(Vec4 other) { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
	};

	/*!
	 * @struct Vec3
	 * @brief Contains a 3D vector.
	 * @details Vec3 is a struct that contains a 3D vector. It contains x, y, and z values.
	 * This can be used for many purposes like position, color, etc. And supports many operations like addition, multiplication, etc.
	 */
	struct Vec3
	{
		float x, y, z;

		Vec3()
		    : x(0.0f)
		    , y(0.0f)
		    , z(0.0f)
		{
		}

		Vec3(float x)
		{
			this->x = x;
			this->y = x;
			this->z = x;
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

		Vec3 operator*(float scalar) { return Vec3(x * scalar, y * scalar, z * scalar); }
		Vec3 operator+(Vec3 other) { return Vec3(x + other.x, y + other.y, z + other.z); }
		Vec3 operator-(Vec3 other) { return Vec3(x - other.x, y - other.y, z - other.z); }
		void operator+=(Vec3 other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
		}
		void operator-=(Vec3 other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
		}

		glm::vec3 ToGLM() { return glm::vec3(x, y, z); }
	};

	/*!
	 * @struct Vec2
	 * @brief Contains a 2D vector.
	 * @details Vec2 is a struct that contains a 2D vector. It contains x and y values.
	 * This can be used for many purposes like texture coordinates, 2D coordinates etc. And supports many operations like addition, multiplication, etc.
	 */
	struct Vec2
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

		Vec2 operator*(float scalar) { return Vec2(x * scalar, y * scalar); }

		Vec2 operator*(double scalar) { return Vec2(x * scalar, y * scalar); }

		Vec2 operator+(Vec2 other) { return Vec2(x + other.x, y + other.y); }

		Vec2 operator-(Vec2 other) { return Vec2(x - other.x, y - other.y); }

		Vec2 Perpendicular() { return Vec2(-y, x); }

		Vec2 Normalize()
		{
			float length = sqrt(x * x + y * y);
			return Vec2(x / length, y / length);
		}
	};
}
