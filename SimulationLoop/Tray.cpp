#include "Tray.h"

Tray::Tray(Vector3f origin, float width, float length, Vector3f normal, Vector3f forward, Vector3f right) :
	Geometry(origin, 1.0f, false, TRAY), normal(normal), forward(forward), right(right)
{

	Vector3f tempPos;

	// DEFINE CORNERS
	// Top Left
	tempPos = origin;
	tempPos.SetX(tempPos.GetX() + -length);
	tempPos.SetZ(tempPos.GetZ() + width);
	m_Bounds.emplace_back(tempPos);

	// Top Right
	tempPos = origin;
	tempPos.SetX(tempPos.GetX() + length);
	tempPos.SetZ(tempPos.GetZ() + width);
	m_Bounds.emplace_back(tempPos);

	// Bottom Right
	tempPos = origin;
	tempPos.SetX(tempPos.GetX() + length);
	tempPos.SetZ(tempPos.GetZ() + -width);
	m_Bounds.emplace_back(tempPos);

	// Bottom Left
	tempPos = origin;
	tempPos.SetX(tempPos.GetX() + -length);
	tempPos.SetZ(tempPos.GetZ() + -width);
	m_Bounds.emplace_back(tempPos);
}

void Tray::CalculatePhysics(float dt, double t)
{

}

void Tray::CollisionDetection(Geometry* geometry, ContactManifold* contactManifold)
{

}

void Tray::CollisionResponse(ManifoldPoint &point)
{

}

void Tray::Update(void)
{

}
void Tray::Render() const
{
	// Draw plane (at y=-20)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glVertex3d(m_Bounds[0].GetX(), m_Bounds[0].GetY(), m_Bounds[0].GetZ());
	glVertex3d(m_Bounds[1].GetX(), m_Bounds[1].GetY(), m_Bounds[1].GetZ());
	glVertex3d(m_Bounds[2].GetX(), m_Bounds[2].GetY(), m_Bounds[2].GetZ());
	glVertex3d(m_Bounds[3].GetX(), m_Bounds[3].GetY(), m_Bounds[3].GetZ());
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}