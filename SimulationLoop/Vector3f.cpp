#include "Vector3f.h"
#define _USE_MATH_DEFINES
#include <math.h>

Vector3f::Vector3f(void)
{
}

Vector3f::Vector3f(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

Vector3f::~Vector3f(void)
{
}

void Vector3f::Set(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

float Vector3f::GetX() const
{
	return m_x;
}

float Vector3f::GetY() const
{
	return m_y;
}

float Vector3f::GetZ() const
{
	return m_z;
}

void Vector3f::SetX(float X)
{
	m_x = X;
}

void Vector3f::SetY(float Y)
{
	m_y = Y;
}

void Vector3f::SetZ(float Z)
{
	m_z = Z;
}

Vector3f Vector3f::add(const Vector3f &vec) const
{
	return Vector3f(m_x + vec.GetX(), m_y + vec.GetY(), m_z + vec.GetZ());
}

Vector3f Vector3f::subtract(const Vector3f &vec) const
{
	return Vector3f(m_x - vec.GetX(), m_y - vec.GetY(), m_z - vec.GetZ());
}

Vector3f Vector3f::mult(float n) const
{
	return Vector3f(m_x*n, m_y*n, m_z*n);
}

Vector3f Vector3f::divide(float n) const
{
	return Vector3f(m_x/n, m_y/n, m_z/n);
}

float Vector3f::dot(const Vector3f &vec) const
{
	return m_x*vec.GetX() + m_y*vec.GetY();
}

float Vector3f::length() const
{
	return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

float Vector3f::distance(const Vector3f &vec) const
{
	return subtract(vec).length();
}

Vector3f Vector3f::normalise()
{
	float len = length();
	Set(m_x/len, m_y/len, m_z/len);
	return *this;
}


Vector3f operator+ (const Vector3f &lhs, const Vector3f &rhs)
{
	return lhs.add(rhs);
}

Vector3f operator- (const Vector3f &lhs, const Vector3f &rhs)
{
	return lhs.subtract(rhs);
}

Vector3f operator* (const Vector3f &lhs, float n)
{
	return lhs.mult(n);
}

Vector3f operator* (float n, const Vector3f &rhs)
{
	return rhs.mult(n);
}

Vector3f operator/ (const Vector3f &lhs, float n)
{
	return lhs.divide(n);
}