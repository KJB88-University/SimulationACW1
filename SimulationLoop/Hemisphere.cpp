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

void Hemisphere::CalculatePhysics(float dt, double t)
{

}

void Hemisphere::CollisionDetection(Geometry* geometry, ContactManifold* contactManifold)
{

}

void Hemisphere::CollisionResponse(ManifoldPoint &point)
{

}

void Hemisphere::Update(void)
{

}

void Hemisphere::Render() const
{

	/*
	const float PI = 3.141592f;
	GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles     
	x = m_pos.GetX();
	y = m_pos.GetY();
	z = m_pos.GetZ();

	//GLfloat radius = 60.0f;
	int gradation = 60.0f;

	for (alpha = 0.0; alpha < PI; alpha += PI / gradation)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (beta = 0.0; beta < 2.01 * PI; beta += PI / gradation)
		{
			x = m_radius*cos(beta)*sin(alpha);
			y = m_radius*sin(beta)*sin(alpha);
			z = m_radius*cos(alpha);
			glVertex3f(x, y, z);
			x = m_radius*cos(beta)*sin(alpha + PI / gradation);
			y = m_radius*sin(beta)*sin(alpha + PI / gradation);
			z = m_radius*cos(alpha + PI / gradation);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
	*/

	// BOWL
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(m_pos.GetX() , m_pos.GetY(), m_pos.GetZ()); // Change to default position

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Change to line drawing

	glEnable(GL_CLIP_PLANE0);
	glTranslatef(m_pos.GetX(), m_pos.GetY() - 20.0f * Game::scaleTweakable, m_pos.GetZ()); // Move world for clipping plane
	glClipPlane(GL_CLIP_PLANE0, planeEqn); 

	glTranslatef(m_pos.GetX(), m_pos.GetY(), m_pos.GetZ()); // Return world for drawing sphere

	glColor3d(1, 0, 0);
	gluSphere(quadric, m_radius, 16, 16);
	glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	glDisable(GL_CLIP_PLANE0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Revert to fill instead of lines
	glPopMatrix();
}
