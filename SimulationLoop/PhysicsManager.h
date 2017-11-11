#pragma once

#include "Vector3f.h"
#include "ContactManifold.h"
#include "Geometry.h"
#include "Sphere.h"

class PhysicsManager
{
public:

	// Get instance of this manager
	static PhysicsManager* GetInstance();

	// PRE-COLLISION PHYSICS
	Vector3f CalculatePrePhysics(Sphere* sphere, float dt);

	// DETECTION
	void SphereToSphereCollisionDetection(Sphere* sphere1, Sphere* sphere2, ContactManifold *contactManifold);
	void SphereToPlaneCollisionDetection(Sphere* sphere, Geometry* plane, ContactManifold *contactManifold);

	// RESPONSE
	void SphereToSphereCollisionResponse(ManifoldPoint &point);
	void SphereToPlaneCollisionResponse(ManifoldPoint &point);

	// POST-COLLISION PHYSICS
	Vector3f CalculatePostPhysics(Sphere* sphere);

	// ELASTICITY MANIPULATION
	void IncrementElasticity(void);
	void DecrementElasticity(void);

	float GetCoR(void) const;
	void SetCoR(float newCoR);

private:

	struct State
	{
		Vector3f position;
		Vector3f velocity;
	};

	struct Differential
	{
		Vector3f dx; // dx/dt = velocity
		Vector3f dv; // dv/dt = acceleration
	};

	// RUNGE-KUTTA FOURTH ORDER
	Vector3f RK4Integrate(State* state, double t, float dt);
	Vector3f RK4Evaluate(State initial, double t, float dt, Differential diff);
	Vector3f ApplyExternalForces(Vector3f velocity);


	// Private constructor to avoid instances being created
	PhysicsManager(void);

	// Instance of this manager
	static PhysicsManager* instance;

	// Coefficient of Restitution
	float m_CoR;

	// Acceleration due to Gravity
	const float m_gravity = -9.81f;
};