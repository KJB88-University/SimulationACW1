#include "Sphere.h"
#include "Vector3f.h"
#include "Geometry.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "TextureLoader.h"
#include "PhysicsManager.h"
#include "Game.h"

#pragma region CONSTRUCTOR_DESTRUCTOR

Sphere::Sphere(Vector3f pos, Vector3f vel, float mass, float radius)
	: Geometry::Geometry(pos, mass, false, SPHERE), m_radius(radius), m_velocity(vel)
{
	m_texture = TextureLoader::LoadBMP("checker.bmp");
	physics = PhysicsManager::GetInstance();

	quadric = gluNewQuadric();
}

Sphere::~Sphere(void)
{

}
#pragma endregion

#pragma region PROPERTIES

// GETS
Vector3f Sphere::GetNewPos() const
{
	return m_newPos;
}

Vector3f Sphere::GetVel() const
{
	return m_velocity;
}

Vector3f Sphere::GetNewVel() const
{
	return m_newVelocity;
}

float Sphere::GetRadius() const
{
	return m_radius;
}

// SETS
void Sphere::SetNewPos(Vector3f pos)
{
	m_newPos = pos;
}

void Sphere::SetVel(float x, float y, float z)
{
	m_velocity.Set(x, y, z);
}

void Sphere::SetNewVel(Vector3f vel)
{
	m_newVelocity = vel;
}

void Sphere::SetForce(Vector3f force)
{
	m_force = force;
}

Vector3f Sphere::GetForce() const
{
	return m_force;
}

void Sphere::SetAccel(Vector3f accel)
{
	m_acceleration = accel;
}

Vector3f Sphere::GetAccel() const
{
	return m_acceleration;
}

void Sphere::SetVel(Vector3f vel)
{
	m_velocity = vel;
}

void Sphere::SetRotation(Vector3f rollAxis, float angle)
{
	m_rollAxis = rollAxis;
	m_angle = angle * Game::scaleTweakable;
}
#pragma endregion

#pragma region SIMULATION_LOOP
void Sphere::CalculatePhysics(float dt, float t)
{
	physics->CalculatePrePhysics(this, t, dt);

}

void Sphere::CollisionDetection(Geometry* geometry2, ContactManifold *contactManifold, float t, float dt)
{
	physics->CollisionDetection(this, static_cast<Sphere*>(geometry2), contactManifold, t, dt);
}

void Sphere::CollisionResponse(ManifoldPoint &point)
{
	physics->CollisionResponse(point);
}

void Sphere::Update()
{
	physics->CalculatePostPhysics(this);
}
#pragma endregion

void Sphere::ResetPos()
{
	m_newPos = m_pos;
}

void Sphere::Render()
{

	glPushMatrix();
	glTranslatef(m_pos.GetX(), m_pos.GetY(), m_pos.GetZ());

	// Apply fake rotation
	glRotatef(m_angle, m_rollAxis.GetX(), m_rollAxis.GetY(), m_rollAxis.GetZ());


	//glRotatef(m_angVel.GetX(), 0.0f, 0.0f, 1.0f);
	//glRotatef(m_angVel.GetZ(), 1.0f, 0.0f, 0.0f);

	if (m_velocity.length() > 0.25f)
	{
		glColor3d(1, 0, 0);
	}
	else
	{
		glColor3d(0, 0, 1);
	}
	glBindTexture(GL_TEXTURE_2D, m_texture);               // Select Our Texture
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, m_radius, 20, 20);
	glPopMatrix();

}
