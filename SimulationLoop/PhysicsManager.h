#pragma once

#include "Vector3f.h"
#include "ContactManifold.h"
#include "Geometry.h"
#include "Sphere.h"
#include "Plane.h"
#include "Hemisphere.h"

class PhysicsManager
{
private:

	struct State
	{
		Vector3f position;
		Vector3f velocity;
		Vector3f acceleration;
		float mass;
	};

	struct Differential
	{
		Vector3f dx; // dx/dt = velocity
		Vector3f dv; // dv/dt = acceleration
	};

public:

	// Get instance of this manager
	static PhysicsManager* GetInstance();

	// ALL SIMULATION STEPS UPDATE OBJECT VIA POINTERS
	// NO RETURN VALUES NECESSARY
	// PRE-COLLISION PHYSICS
	void CalculatePrePhysics(Sphere* sphere, float t, float dt);

	// DETECTION
	void CollisionDetection(Geometry* geometry1, Geometry* geometry2, ContactManifold* contactManifold, float t, float dt);

	// RESPONSE
	void CollisionResponse(ManifoldPoint &point);

	// POST-COLLISION PHYSICS
	void CalculatePostPhysics(Sphere* sphere);

	// ELASTICITY MANIPULATION
	void IncrementElasticity(void);
	void DecrementElasticity(void);

	float GetCoR(void) const;
	void SetCoR(float newCoR);

	// FRICTIONAL FORCE MANIPULATION
	float GetCoF(void) const;
	void SetCoF(float newCoF);

	void IncrementFriction(void);
	void DecrementFriction(void);

	// 0-2 Trays (Bot, Mid, Top)
	// 3-6 Walls (Front, Back, Left, Right);
	//vector<Plane*> planeList;
private:

	// Private constructor to avoid instances being created
	PhysicsManager(void);

	// Instance of this manager
	static PhysicsManager* instance;

	// COLLISION DETECTION
	int maxIterations = 5; // Maximum no. of iterations for precise collision detection

	// Coefficient of Restitution
	float m_CoR;

	// Friction force
	float m_CoF;
	// Acceleration due to Gravity
	const float m_gravity = -9.81f;

	Vector3f CrossProduct(Vector3f vec1, Vector3f vec2);
	Vector3f PlaneProjection(Sphere* sphere1, Plane* plane);

	// COLLISION DETECTION
	bool IterativeCollisionDetectionS2S(Sphere* sphere1, Sphere* sphere2, float t, float dt, Vector3f& outNormal);
	//bool IterativeCollisionDetectionS2B(Sphere* sphere1, Hemisphere* hemisphere1, float t, float dt);
	bool IterativeCollisionDetectionS2P(Sphere* sphere1, Plane* plane1, float t, float dt);
	bool PhysicsManager::IterativeCollisionDetectionS2B(Sphere* sphere1, Hemisphere* hemisphere1, float t, float dt);

		// COLLISION RESPONSE
	void SphereToSphereCollisionResponse(ManifoldPoint &point);
	void SphereToPlaneCollisionResponse(ManifoldPoint &point);

	// RUNGE-KUTTA FOURTH ORDER
	void RK4Integrate(State* state, float t, float dt);
	Differential RK4Evaluate(State* initial, float t, float dt, Differential diff);
	Vector3f ApplyExternalForces(const State* state, const float t);

};