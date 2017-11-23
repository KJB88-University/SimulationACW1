#pragma once

#include "ContactManifold.h"
#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>  
#include "Geometry.h"
#include <gl\GLU.h>
#include "ObjectType.h"

class PhysicsManager;

class Sphere : public Geometry
{
public:
	// Constructor / Destructor
	Sphere(Vector3f pos, Vector3f vel, float mass, float radius);
	~Sphere(void);

	// Simulation Loop (from base/super)
	virtual void CalculatePhysics(float dt, double t) override;
	virtual void CollisionDetection(Geometry* sphere, ContactManifold *contactManifold) override;
	virtual void Update() override;
	virtual void CollisionResponse(ManifoldPoint &point) override;

	// Sphere-specific Properties
	void SetVel(float x, float y, float z);
	void SetNewPos(Vector3f pos);
	void SetNewVel(Vector3f vel);
	void SetForce(Vector3f force);
	void SetAccel(Vector3f accel);
	Vector3f GetAccel() const;
	Vector3f GetForce() const;
	Vector3f GetNewPos() const;
	Vector3f GetVel() const;
	Vector3f GetNewVel() const;
	float GetRadius() const;

	void ResetPos();

	void Render() const;

private:
	
	// Sphere-specific data members
	float m_radius;
	Vector3f m_newPos;
	Vector3f m_velocity;
	Vector3f m_newVelocity;
	Vector3f m_oldVelocity;
	Vector3f m_oldPosition;

	Vector3f m_force;
	Vector3f m_acceleration;

	PhysicsManager* physics;
	GLUquadric* quadric;
};

