#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>
#include "ContactManifold.h"
#include "Geometry.h"

int Geometry::countID = 0;

Geometry::Geometry(Vector3f pos, float mass, bool isStatic) 
	: k_mass(mass), k_objectID(countID), k_isStaticGeometry(isStatic)
{
	m_pos = pos;
	++countID;
}

Geometry::~Geometry(void)
{

}

int Geometry::GetID() const
{
	return k_objectID;
}

Vector3f Geometry::GetPos() const
{
	return m_pos;
}

float Geometry::GetMass() const
{
	return k_mass;
}

void Geometry::SetPos(Vector3f inPos)
{
	m_pos = inPos;
}


