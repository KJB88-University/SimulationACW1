#include "Hemisphere.h"
#include "Vector3f.h"
#include "Geometry.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "TextureLoader.h"
#include "Game.h"

#pragma region CONSTRUCTOR_DESTRUCTOR

Hemisphere::Hemisphere(Vector3f pos, float mass, float radius) :
	Geometry(pos, mass, true, BOWL), m_radius(radius)
{
	//m_texture = TextureLoader::LoadBMP("checker.bmp");

	quadric = gluNewQuadric();
}

Hemisphere::~Hemisphere(void)
{

}
#pragma endregion

#pragma region PROPERTIES

// GETS
float Hemisphere::GetRadius() const
{
	return m_radius;
}

#pragma endregion

void Hemisphere::CalculatePhysics(float dt, float t)
{

}

void Hemisphere::CollisionDetection(Geometry* geometry, ContactManifold* contactManifold, float dt, float t)
{

}

void Hemisphere::CollisionResponse(ManifoldPoint &point)
{

}

void Hemisphere::Update(void)
{

}

void Hemisphere::Render()
{

	// BOWL

	glPushMatrix();
		glTranslatef(m_pos.GetX(), m_pos.GetY(), m_pos.GetZ()); // Change to default position
		glColor3f(0.0, 1.0, 0.0);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Change to line drawing

		glEnable(GL_CLIP_PLANE0);
		glTranslatef(m_pos.GetX(), m_pos.GetY() - 22.5f * Game::scaleTweakable, m_pos.GetZ()); // Move world for clipping plane
		glClipPlane(GL_CLIP_PLANE0, planeEqn);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(m_pos.GetX(), m_pos.GetY(), m_pos.GetZ()); // Change to default position
		glColor3d(1, 0, 0);
		gluSphere(quadric, m_radius, 128, 128);
		
		glRotatef(90.0, 1.0f, 0.0f, 0.0f);
		glDisable(GL_CLIP_PLANE0);
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Revert to fill instead of lines

}
