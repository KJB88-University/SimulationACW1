#include "Plane.h"

Plane::Plane(Vector3f origin, float width, float length, Vector3f normal, Vector3f right, Vector3f forward, PlaneRotation inRotation) :
	Geometry(origin, 1.0f, true, objType), normal(normal), forward(forward), right(right)
{
	Vector3f tempPos;

	// DEFINE CORNERS
	// Top Left
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + -length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + -width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + -width);
	}
	m_Bounds.emplace_back(tempPos);

	// Top Right
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + -length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + width);
	}
	m_Bounds.emplace_back(tempPos);

	// Bottom Right
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + width);
	}
	m_Bounds.emplace_back(tempPos);

	// Bottom Left
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + -width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + -width);
	}
	m_Bounds.emplace_back(tempPos);

}

Plane::~Plane(void)
{

}

/*
Plane::SetUpTray(void)
{

}
*/


void Plane::Update(void)
{

}

void Plane::CalculatePhysics(float dt, double t)
{

}

void Plane::CollisionDetection(Geometry* geometry, ContactManifold* contactManifold)
{

}

void Plane::CollisionResponse(ManifoldPoint &point)
{

}

vector<Vector3f> Plane::GetBounds(void) const
{
	return m_Bounds;
}

void Plane::Render(void) const
{
	// Draw plane (at y=-20)
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glVertex3d(m_Bounds[0].GetX(), m_Bounds[0].GetY(), m_Bounds[0].GetZ());
	glVertex3d(m_Bounds[1].GetX(), m_Bounds[1].GetY(), m_Bounds[1].GetZ());
	glVertex3d(m_Bounds[2].GetX(), m_Bounds[2].GetY(), m_Bounds[2].GetZ());
	glVertex3d(m_Bounds[3].GetX(), m_Bounds[3].GetY(), m_Bounds[3].GetZ());
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
}

