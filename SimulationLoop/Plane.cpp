#include "Plane.h"

Plane::Plane(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4)
{
	//Initialize Bounds[]
	m_Bounds.emplace_back(p1);
	m_Bounds.emplace_back(p2);
	m_Bounds.emplace_back(p3);
	m_Bounds.emplace_back(p4);

}

Plane::~Plane(void)
{
	
}

/*
Plane::SetUpTray(void)
{

}
*/


vector<Vector3f> Plane::GetBounds(void) const
{
	return m_Bounds;
}

void Plane::Render(void)
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

