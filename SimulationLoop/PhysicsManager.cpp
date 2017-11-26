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

// RK4 Implementation
// Adapted from Glenn Fiedler's implementation 
// <https://gafferongames.com/post/integration_basics/>
// [1st June 2014],
// Accessed on: 7th November 2017.
#pragma region RK4 Integration Implementation

void PhysicsManager::CalculatePrePhysics(Sphere* sphere, double t, float dt)
{
	// Setup required structs
	State state = State();

	// Assign state with initial values
	state.position = sphere->GetPos();
	state.velocity = sphere->GetVel();
	state.acceleration = sphere->GetAccel();

	// Integrate
	RK4Integrate(&state, t, dt);

	// Push new positions to object
	sphere->SetNewPos(state.position);
	sphere->SetNewVel(state.velocity);
}


void PhysicsManager::RK4Integrate(State* state, double t, float dt)
{
	// Declare required steps
	Differential k1, k2, k3, k4;

	// Calculate RK4 steps
	k1 = RK4Evaluate(state, t, 0.0f, Differential());
	k2 = RK4Evaluate(state, t, dt * 0.5f, k1);
	k3 = RK4Evaluate(state, t, dt * 0.5f, k2);
	k4 = RK4Evaluate(state, t, dt, k3);

	// Apply full RK4 integration
	Vector3f dxdt = 1.0f / 6.0f * (k1.dx + 2.0f * (k2.dx + k3.dx) + k4.dx);
	Vector3f dvdt = 1.0f / 6.0f * (k1.dv + 2.0f * (k2.dv + k3.dv) + k4.dv);

	// Update state with changed values
	state->position = state->position + dxdt * dt;
	state->velocity = state->velocity + dvdt * dt;
}

PhysicsManager::Differential PhysicsManager::RK4Evaluate(State* initial, double t, float dt, Differential diff)
{
	// Create new state
	State newState;

	// Update initial position with the rate of change
	newState.position = initial->position + diff.dx * dt;
	newState.velocity = initial->velocity + diff.dv * dt;

	// Create new derivative
	Differential output;

	// Update derivative with new values
	output.dx = newState.velocity;
	output.dv = ApplyExternalForces(&newState, t); // Apply external forces like gravity, etc.
	return output;
}

Vector3f PhysicsManager::ApplyExternalForces(const State* state, const double t)
{
	Vector3f force = Vector3f(0.0f, state->mass * m_gravity, 0.0f);
	Vector3f accel = force / state->mass;
	return accel;
}

#pragma endregion

void PhysicsManager::CollisionDetection(Geometry* geometry1, Geometry* geometry2, ContactManifold *contactManifold)
{
	// SPHERE TO SPHERE
	if (geometry1->objType == SPHERE && geometry2->objType == SPHERE)
	{
		SphereToSphereCollisionDetection(static_cast<Sphere*>(geometry1), static_cast<Sphere*>(geometry2), contactManifold);
	}

	// SPHERE TO PLANE
	else if (geometry1->objType == SPHERE && geometry2->objType == PLANE)
	{
		SphereToPlaneCollisionDetection(static_cast<Sphere*>(geometry1), static_cast<Plane*>(geometry2), contactManifold);
	}
}

void PhysicsManager::IterativeCollisionDetection(Sphere* sphere1, Geometry* geometry, float dt)
{
#pragma region VAR DELCARATION

	// deltaTime checks
	float currentDT = dt;
	float lastDT;

	// Sphere1's variables
	Vector3f pos1 = sphere1->GetPos();
	Vector3f vel1 = sphere1->GetVel();
	Vector3f testPos;

	// Sphere2's variables
	Sphere* sphere2;
	Vector3f pos2;

	// Plane variables
	Plane* plane;
	Vector3f planeNormal;
	vector<Vector3f> bounds;

	// Safest position
	Vector3f safePosition;

#pragma endregion

#pragma region OBJECT DETERMINATION
	// Object determination
	if (geometry->objType == SPHERE)
	{
		sphere2 = static_cast<Sphere*>(geometry);
		pos2 = sphere2->GetPos();
	}
	else if (geometry->objType == PLANE)
	{
		plane = static_cast<Plane*>(geometry);
		//bounds = plane1->GetBounds();
		//Vector3f planeNormal = plane1->normal;
	}
	
#pragma endregion

#pragma region ITERATIVE COLLISION DETECTION

	// LOOP - Iterative testing for collisions
	for (int i = 0; i < maxIterations; i++)
	{
		// Update positions to test
		testPos = pos1 + (vel1 * currentDT);

		float distance;

		// Sphere-to-sphere radius check
		if (geometry->objType == SPHERE)
		{
			distance = testPos.distance(pos2) - (sphere1->GetRadius() + sphere2->GetRadius());
		}

		// Sphere-to-Plane normal check
		else if (geometry->objType == PLANE)
		{
			Vector3f tempVector3f = testPos - bounds[0];
			distance = tempVector3f.dot(planeNormal);
		}

		// Save previous DT before we change current DT
		lastDT = currentDT;

		// If distance is greater than 0,
		// we're 'above' the plane
		// NO COLLISION
		if (distance > 0)
		{
			safePosition = testPos;
			currentDT = currentDT / 2; // Halve currentDT, to get next DT
		}

		// If distance is less than or equal to 0,
		// we have passed through the plane or are currently intersecting
		// COLLISION DETECTED
		else if (distance <= 0)
		{
			lastDT = currentDT; // Save previousDT
			currentDT = currentDT + lastDT;
		}
	}

#pragma endregion

	// TODO - ADD TO CONTACT MANIFOLD + TIME OF IMPACT
}

void PhysicsManager::SphereToSphereCollisionDetection
(Sphere* sphere1, Sphere* sphere2, ContactManifold *contactManifold)
{
	// TODO - change to reliable Detection
	/*
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
	*/
	
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
	point.contactID1->SetNewVel(
		-1.0f * colNormal * colNormal.dot(point.contactID1->GetVel()));

	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(
		-1.0f * colNormal * colNormal.dot(point.contactID2->GetVel()));
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