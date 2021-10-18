#pragma once

class Vector2
{
public:
	Vector2() : Vector2(0.0f, 0.0f) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {} 

	Vector2 operator+(const Vector2& other)
	{
		Vector2 temp(x + other.x, y + other.y);
		return temp;
	}

	Vector2 operator-(const Vector2& other)
	{
		Vector2 temp(x - other.x, y - other.y);
		return temp;
	}

	float x;
	float y;
};

class Vector3
{
public:
	Vector3() : Vector3(0.0f, 0.0f, 0.0f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(int x, int y, int z) : x(static_cast<float>(x)), y(static_cast<float>(y)), z(static_cast<float>(z)) {}

	Vector3 operator+(const Vector3& other)
	{
		Vector3 temp(x + other.x, y + other.y, z + other.z);
		return temp;
	}

	Vector3 operator-(const Vector3& other)
	{
		Vector3 temp(x - other.x, y - other.y, z - other.z);
		return temp;
	}

	float x;
	float y;
	float z;
};