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

#pragma endregion

#pragma region SIMULATION_LOOP
void Sphere::CalculatePhysics(float dt, float t)
{
	// Calcuate total force for this sphere, e.g. F = F1+F2+F3+...
	//m_force = Vector3f(0.0f, -9.81f * k_mass, 0.0f);

	// Calculate acceleration
	//m_acceleration = m_force / k_mass;

	// Integrate accel to get the new velocity (using Euler)
	//m_newVelocity = m_velocity + (m_acceleration * dt);

	// Integrate old velocity to get the new position (using Euler)
	//m_newPos = m_pos + (m_velocity * dt);

	physics->CalculatePrePhysics(this, t, dt);

	// TODO - Bad plane detection
	if (m_newPos.GetY() < -20.0f + m_radius)
	{
		m_newPos.Set(m_newPos.GetX(), -20.0f + m_radius, 0.0f);
		m_newVelocity.Set(m_newVelocity.GetX(), 0.0f, 0.0f);
	}
	
}

void Sphere::CollisionDetection(Geometry* geometry2, ContactManifold *contactManifold, float t, float dt)
{
	physics->CollisionDetection(this, static_cast<Sphere*>(geometry2), contactManifold, t, dt);
}

void Sphere::CollisionResponse(ManifoldPoint &point)
{
	// Replace with better response code for Spheres

	/*
	// SPHERES FLY OFF IF COR IS LOW
	// Normal of the collision
	Vector3f colNormal = point.contactNormal;

	// Get properties of objects
	Vector3f sVel1 = point.contactID1->GetVel();
	Vector3f sVel2 = point.contactID2->GetVel();
	float sMass1 = point.contactID1->GetMass();
	float sMass2 = point.contactID2->GetMass();

	// Velocities for both spheres
	Vector3f newVel1 = sVel1 - sVel1.dot(colNormal) * colNormal;
	Vector3f newVel2 = sVel2 - sVel2.dot((colNormal * -1)) * (colNormal * -1);

	// Velocities for both spheres based on mass
	Vector3f massVel1 = (sMass1 * sVel1 + sMass2 * sVel2 + CoR * sMass2 * (sVel2 - sVel1)) / (sMass1 + sMass2);
	Vector3f massVel2 = (sMass2 * sVel2 + sMass1 * sVel1 + CoR * sMass1 * (sVel1 - sVel2)) / (sMass2 + sMass1);

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(newVel1 + massVel1);

	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(newVel2 + massVel2);
	*/

	physics->SphereToSphereCollisionResponse(point);
}

void Sphere::Update()
{
	m_velocity = m_newVelocity;
	m_pos = m_newPos;

	m_oldPosition = m_pos;
	m_oldVelocity = m_velocity;
}
#pragma endregion

void Sphere::ResetPos()
{
	m_newPos = m_pos;
}

void Sphere::Render() const									
{
	
	glPushMatrix();
		glTranslatef(m_pos.GetX(), m_pos.GetY(), 0);
		glColor3d(1, 0, 0);
		glBindTexture(GL_TEXTURE_2D, m_texture);               // Select Our Texture
		gluQuadricDrawStyle(quadric, GLU_FILL);
		gluQuadricTexture(quadric, GL_TRUE);
		gluQuadricNormals(quadric, GLU_SMOOTH);
		gluSphere(quadric, m_radius, 20, 20);
	glPopMatrix();
}
