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
#pragma region RK4 Integration

void PhysicsManager::CalculatePrePhysics(Sphere* sphere, float t, float dt)
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


void PhysicsManager::RK4Integrate(State* state, float t, float dt)
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

PhysicsManager::Differential PhysicsManager::RK4Evaluate(State* initial, float t, float dt, Differential diff)
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

Vector3f PhysicsManager::ApplyExternalForces(const State* state, const float t)
{
	Vector3f force = Vector3f(0.0f, state->mass * m_gravity, 0.0f);
	Vector3f accel = force / state->mass;
	return accel;
}

#pragma endregion

#pragma region Iterative Collision Detection
void PhysicsManager::CollisionDetection(Geometry* geometry1, Geometry* geometry2, ContactManifold *contactManifold, float t, float dt)
{
	float distance;

	// If the first object is a sphere,
	// (we don't need to check collision from a plane perspective)
	// Additionally avoid checking against the same sphere
	if (geometry1->objType == SPHERE && geometry1->GetID() != geometry2->GetID())
	{
		Sphere* sphere1 = static_cast<Sphere*>(geometry1);

		if (geometry2->objType == PLANE)
		{
			Plane* plane = static_cast<Plane*>(geometry2);
			distance = plane->normal.dot(geometry1->GetPos() - geometry2->GetPos());

			if (distance <= 0.0f)
			{
				if (IterativeCollisionDetectionS2P(sphere1, plane, t, dt))
				{
					ManifoldPoint mp;
					mp.contactID1 = sphere1;
					mp.contactID2 = plane;
					mp.contactNormal = plane->normal;
					contactManifold->Add(mp);
				}
			}
		}
		else if (geometry2->objType == SPHERE)
		{
			Sphere* sphere2 = static_cast<Sphere*>(geometry2);
			distance = sphere1->GetPos().distance(sphere2->GetPos()) - (sphere1->GetRadius() + sphere2->GetRadius());

			if (distance <= 0.0f)
			{
				if (IterativeCollisionDetectionS2S(sphere1, sphere2, t, dt))
				{
					ManifoldPoint mp;
					mp.contactID1 = sphere1;
					mp.contactID2 = sphere2;
					mp.contactNormal = (sphere1->GetPos() - sphere2->GetPos()).normalise();
					contactManifold->Add(mp);
				}
			}
		}
	}

}

/*
void PhysicsManager::IterativeCollisionDetection(Sphere* sphere1, Geometry* geometry, float t, float dt, ContactManifold* contactManifold)
{

	// OBJECTS REQUIRED
	// Sphere1's variables
	Vector3f pos1 = sphere1->GetPos();
	Vector3f vel1 = sphere1->GetVel();
	Vector3f testPos;

	// Sphere2's variables
	Sphere* sphere2;
	Vector3f pos2;

	// Plane variables
	Plane* plane;

	// Safest position
	Vector3f safePos;
	bool collided = false;

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

	// deltaTime checks
	float min = t;
	float max = dt;
	float currentDT = dt;

	// Distance to target from position
	float distance = 0;
	for (int i = 0; i < maxIterations; ++i)
	{
		// Get new position based on current DT value
		testPos = pos1 + (vel1 * currentDT);

		// Sphere-to-sphere radius check
		if (geometry->objType == SPHERE)
		{
			distance = testPos.distance(pos2) - (sphere1->GetRadius() + sphere2->GetRadius());
		}

		// Sphere-to-Plane normal check
		else if (geometry->objType == PLANE)
		{
			distance = plane->normal.dot((testPos - plane->GetPos()));
		}

		// Above plane
		if (distance > 0)
		{
			min = currentDT;
			safePos = testPos;
		}
		else if (distance <= 0)
		{
			max = currentDT;
			collided = true;
		}

		currentDT = (max - min) / 2;
	}

	if (collided == true)
	{
		ManifoldPoint mp;
		sphere1->SetNewPos(testPos);
		mp.contactID1 = sphere1;
		mp.contactID2 = geometry;
		if (geometry->objType == PLANE)
		{
			mp.contactNormal = plane->normal;
		}
		else if (geometry->objType == SPHERE)
		{
			mp.contactNormal = (sphere1->GetPos() - geometry->GetPos()).normalise();
		}

		contactManifold->Add(mp);
	}
}
*/
bool PhysicsManager::IterativeCollisionDetectionS2S(Sphere* sphere1, Sphere* sphere2, float t, float dt)
{
	// Object attributes
	Vector3f pos1 = sphere1->GetPos();
	Vector3f vel1 = sphere1->GetVel();
	float r1 = sphere1->GetRadius();

	Vector3f pos2 = sphere2->GetPos();
	float r2 = sphere2->GetRadius();

	// Binary chop values
	int maxIterations = 5;
	float min = t;
	float max = dt;
	float currentDT = (max - min) / 2;

	// Output values
	Vector3f testPos;
	Vector3f safePos;
	bool collided = false;

	// Distance to target from position
	float distance = 0;
	for (int i = 0; i < maxIterations; ++i)
	{
		// Get new position based on current DT value
		testPos = pos1 + (vel1 * currentDT);

		// S2S distance test
		distance = testPos.distance(pos2) - (sphere1->GetRadius() + sphere2->GetRadius());

		// Above plane
		if (distance > 0)
		{
			min = currentDT;
			safePos = testPos;
		}
		else if (distance <= 0)
		{
			max = currentDT;
			collided = true;
		}

		currentDT = (max - min) / 2;
	}

	if (collided == true)
	{
		sphere1->SetNewPos(safePos);
	}

	return collided;
}

bool PhysicsManager::IterativeCollisionDetectionS2P(Sphere* sphere1, Plane* plane1, float t, float dt)
{
	// Object attributes
	Vector3f pos1 = sphere1->GetPos();
	Vector3f vel1 = sphere1->GetVel();

	Vector3f pOrigin = plane1->GetPos();
	Vector3f pNormal = plane1->normal;
	Vector3f pForward = plane1->forward;
	Vector3f pRight = plane1->right;

	// Binary chop values
	int maxIterations = 5;
	float min = t;
	float max = dt;
	float currentDT = (max - min) / 2;

	// Output values
	Vector3f safePos;
	bool collided = false;
	Vector3f testPos;

	// Distance to target from position
	float distance = 0;
	for (int i = 0; i < maxIterations; ++i)
	{
		// Get new position based on current DT value
		testPos = pos1 + (vel1 * currentDT);

		// TODO - Plane bounds check!
		
		// Sphere-to-Plane distance test
		distance = plane1->normal.dot((testPos - pOrigin));

		// Above plane
		if (distance > 0)
		{
			min = currentDT;
			safePos = testPos;
		}
		else if (distance <= 0)
		{
			max = currentDT;
			collided = true;
		}

		currentDT = (max - min) / 2;
	}

	if (collided == true)
	{
		sphere1->SetNewPos(safePos);
	}

	return collided;
}
/*
bool PhysicsManager::IterativeCollisionDetectionS2B(Sphere* sphere1, Hemisphere* hemisphere1, float t, float dt)
{
	// Object attributes
	Vector3f pos1 = sphere1->GetPos();
	Vector3f vel1 = sphere1->GetVel();
	float r1 = sphere1->GetRadius();
	Vector3f pos2 = hemisphere1->GetPos();
	float r2 = hemisphere1->GetRadius();

	// Binary chop values
	int maxIterations = 5;
	float min = t;
	float max = dt;
	float currentDT = (max - min) / 2;

	// Output values
	Vector3f safePos;
	bool collided = false;

	return false;
}
*/
#pragma endregion
/*
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
*/

void PhysicsManager::CollisionResponse(ManifoldPoint &point)
{

	if (point.contactID2->objType == SPHERE)
	{
		SphereToSphereCollisionResponse(point);
	}
	else if (point.contactID2->objType == PLANE)
	{
		SphereToPlaneCollisionResponse(point);
	}
}

void PhysicsManager::SphereToSphereCollisionResponse
(ManifoldPoint &point)
{
	Sphere* sphere2 = static_cast<Sphere*>(point.contactID2);
	// TODO - Change to realistic Response
	Vector3f colNormal = point.contactNormal;

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(
		-1.0f * colNormal * colNormal.dot(point.contactID1->GetVel()));

	sphere2->ResetPos();
	sphere2->SetNewVel(
		-1.0f * colNormal * colNormal.dot(sphere2->GetVel()));
}

void PhysicsManager::SphereToPlaneCollisionResponse
(ManifoldPoint &point)
{
	Plane* plane = static_cast<Plane*>(point.contactID2);

	// TODO VRr = Vi - 2(Vi.dot(N))N
	Vector3f colNormal = plane->normal;

	point.contactID1->ResetPos();

	Vector3f newVel = point.contactID1->GetVel() - 2 * (point.contactID1->GetVel().dot(colNormal)) * colNormal;
	point.contactID1->SetNewVel(newVel);
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