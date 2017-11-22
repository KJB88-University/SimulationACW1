#include "Plane.h"

Plane::Plane(Vector3f origin, float width, float depth, Vector3f normal, Vector3f right, Vector3f forward) :
	Geometry(origin, 1.0f, true, objType), normal(normal), forward(forward), right(right)
{
	Vector3f tempPos;

	// TODO - Spawn correct planes in GAME
	// DEFINE CORNERS
	// Top Left
	tempPos = origin;
	tempPos.SetZ(tempPos.GetZ() + -depth);
	tempPos.SetX(tempPos.GetX() + -width);
	m_Bounds.emplace_back(tempPos);

	// Top Right
	tempPos = origin;
	tempPos.SetZ(tempPos.GetZ() + -depth);
	tempPos.SetX(tempPos.GetX() + width);
	m_Bounds.emplace_back(tempPos);

	// Bottom Right
	tempPos = origin;
	tempPos.SetZ(tempPos.GetZ() + depth);
	tempPos.SetX(tempPos.GetX() + width);
	m_Bounds.emplace_back(tempPos);

	// Bottom Left
	tempPos = origin;
	tempPos.SetZ(tempPos.GetZ() + depth);
	tempPos.SetX(tempPos.GetX() + -width);
	m_Bounds.emplace_back(tempPos);

}

Plane::Plane(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4, Vector3f normal) :
	Geometry((p4 - p2), 1.0f, true, objType), normal(normal)
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

