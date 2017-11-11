#include "PhysicsManager.h"
#include "Game.h"

// Provide default value to instance
// to avoid undefined behaviour
PhysicsManager* PhysicsManager::instance = 0;

PhysicsManager* PhysicsManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new PhysicsManager();
	}

	return instance;
}

// CONSTRUCTOR
PhysicsManager::PhysicsManager()
{
	m_CoR = 0.5f;
}


Vector3f PhysicsManager::CalculatePrePhysics(Sphere* sphere, float dt)
{
	// Setup required structs
	State state = State();

	// Get initial values
	Vector3f initialPos = sphere->GetPos();
	Vector3f initialVel = sphere->GetVel();

	state.position = initialPos;
	state.velocity = initialVel;

	Vector3f dV = (sphere->GetForce() / sphere->GetMass()) * dt;
	// TODO

	// Integrate
}


Vector3f PhysicsManager::RK4Integrate(State* state, double t, float dt)
{
	Differential k1, k2, k3, k4;

	return 

}

Vector3f PhysicsManager::RK4Evaluate(State initial, double t, float dt, Differential diff)
{
	State newState;
	newState.position = initial.position + diff.dx * dt;
	newState.velocity = initial.velocity + diff.dv * dt;

	Differential output;
	output.dx = newState.velocity;
	output.dv = newState.velocity * m_gravity;
}


Vector3f PhysicsManager::ApplyExternalForces(Vector3f velocity)
{
	velocity.SetY(velocity.GetY() * m_gravity);
	return velocity;
}

void PhysicsManager::SphereToSphereCollisionDetection
(Sphere* sphere1, Sphere* sphere2, ContactManifold *contactManifold)
{
	// TODO - change to reliable Detection
	Vector3f pos1 = sphere1->GetNewPos();
	Vector3f pos2 = sphere2->GetNewPos();
	float dist = pos1.distance(pos2) - (sphere1->GetRadius() + sphere2->GetRadius());
	if (dist < 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = sphere1;
		mp.contactID2 = sphere2;
		mp.contactNormal = (pos2 - pos1).normalise();
		contactManifold->Add(mp);
	}
}

void PhysicsManager::SphereToPlaneCollisionDetection
(Sphere* sphere, Geometry* plane, ContactManifold *contactManifold)
{
	// TODO
}

void PhysicsManager::SphereToSphereCollisionResponse
(ManifoldPoint &point)
{
	// TODO - Change to realistic Response
	Vector3f colNormal = point.contactNormal;

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(-1.0f*colNormal*colNormal.dot(point.contactID1->GetVel()));

	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(-1.0f*colNormal*colNormal.dot(point.contactID2->GetVel()));
}

void PhysicsManager::SphereToPlaneCollisionResponse
(ManifoldPoint &point)
{
	// TODO
}

void PhysicsManager::SetCoR(float newCoR)
{
	m_CoR = newCoR;
}

void PhysicsManager::IncrementElasticity(void)
{
	if (m_CoR + 0.1f != 1.0f)
	{
		m_CoR++;
	}
}

void PhysicsManager::DecrementElasticity(void)
{
	if (m_CoR - 0.1f >= 0.1f)
	{
		m_CoR--;
	}
}

float PhysicsManager::GetCoR(void) const
{
	return m_CoR;
}