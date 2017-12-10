#include "PhysicsManager.h"
#include "Game.h"
#include "Box.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
	Vector3f force = Vector3f(0.0f, state->mass * (m_gravity * Game::scaleTweakable), 0.0f);
	Vector3f accel = force / state->mass;
	return accel;
}

#pragma endregion

#pragma region Iterative Collision Detection
void PhysicsManager::CollisionDetection(Geometry* geometry1, Geometry* geometry2, ContactManifold *contactManifold, float t, float dt)
{
	float distance = 0.0f;

	// If the first object is a sphere,
	// (we don't need to check collision from a plane perspective)
	// Additionally avoid checking against the same sphere
	if (geometry1->objType == SPHERE && geometry1->GetID() != geometry2->GetID())
	{
		Sphere* sphere1 = static_cast<Sphere*>(geometry1);

		// If the geometry is a PLANE
		// Do plane collision detection
		if (geometry2->objType == PLANE)
		{
			Plane* plane = static_cast<Plane*>(geometry2);
			distance = plane->normal.dot(sphere1->GetPos() - plane->GetPos()) - sphere1->GetRadius();
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
					mp.contactNormal = (sphere1->GetNewPos() - sphere2->GetNewPos()).normalise();
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
	Vector3f fPos1 = sphere1->GetNewPos();

	Vector3f pos2 = sphere2->GetPos();
	Vector3f fPos2 = sphere2->GetNewPos();
	float r2 = sphere2->GetRadius();

	// Binary chop values
	float minN = 0.0f;
	float maxN = 1.0f;
	float currentN = 0.5f;

	// Output values
	Vector3f testPos = pos1;
	Vector3f collisionDirection;
	Vector3f safePos;
	bool collided = false;

	// Distance to target from position
	float distance = 0;
	//float time = 0;
	for (int i = 0; i < maxIterations; ++i)
	{
		collisionDirection = (fPos1 - fPos2).normalise();
		// Get new position based on current DT value
		testPos = pos1 + (collisionDirection * currentN);

		/*
		// S2S distance test
		Vector3f lineOfImpact = sphere1->GetNewPos() - pos2;
		float radiiSum = r1 + r2;

		//distance = lineOfImpact.dot(lineOfImpact);

		distance = radiiSum * radiiSum;
		if (distance < 0)
		{
			distance = -1;
		}

		Vector3f relativeVel = vel1 - sphere2->GetVel();
		float b = relativeVel.dot(lineOfImpact);
		float a = relativeVel.dot(relativeVel);
		if (b >= 0)
		{
			distance = -1;
		}

		float d = b * b - a * lineOfImpact.dot(lineOfImpact);
		//distance = b * b - a * vecs.dot(vecs);
		time = (-b - sqrt(distance)) / a;
		*/
		//testPos = pos1 + (vel1 * time);
		//distance = fPos2.distance(testPos) - (sphere1->GetRadius() + sphere2->GetRadius());
		distance = testPos.distance(pos2) - (sphere1->GetRadius() + sphere2->GetRadius());

		// Above plane
		if (distance > 0)
		{
			safePos = testPos;
			minN = currentN;
		}
		else if (distance <= 0)
		{
			maxN = currentN;
			collided = true;
		}

		currentN = (maxN + minN) / 2;
	}

	if (collided == true)
	{
		safePos = testPos;
		//safePos = pos1 + (vel1 * time);
		sphere1->SetNewPos(safePos);
	}

	return collided;
}

bool PhysicsManager::IterativeCollisionDetectionS2P(Sphere* sphere1, Plane* plane1, float t, float dt, Vector3f& outNormal)
{
	// Sphere attributes
	Vector3f pos1 = sphere1->GetPos();
	Vector3f vel1 = sphere1->GetNewVel();
	Vector3f fPos1 = sphere1->GetNewPos();
	float radius1 = sphere1->GetRadius();

	// Plane attributes
	Vector3f pOrigin = plane1->GetPos();
	Vector3f pNormal = plane1->normal;
	Vector3f pForward = plane1->forward;
	Vector3f pRight = plane1->right;
	float length = plane1->length;
	float width = plane1->width;

	Vector3f projection = PlaneProjection(sphere1, plane1);
	// Binary chop values
	float minN = 0.0f;
	float maxN = 1.0f;
	float currentN = 0.5f;

	// Output values
	Vector3f safePos;
	bool collided = false;
	Vector3f testPos;

	// Constraints
	if ((fPos1.GetY() - radius1) < (7.5f * Game::scaleTweakable) && (fPos1.GetY() + radius1) > (-7.5f * Game::scaleTweakable))
	{
		if ((projection.GetX() + radius1) > -width && (projection.GetX() - radius1) < width &&
			(projection.GetZ() + radius1) > -length && (projection.GetZ() - radius1) < length)
		{
			// Distance to target from position
			float distance = 0;
			for (int i = 0; i < maxIterations; ++i)
			{
				// Get new position based on current DT value
				//testPos = pos1 + (vel1 * currentDT);
				testPos = pos1 + (vel1 * currentN);

				// Sphere-to-Plane distance test 
				distance = pNormal.dot((testPos - pOrigin)) - radius1;

				// Above plane
				if (distance > 0)
				{
					safePos = testPos;

					// Minimum separated
					if (distance <= 0.001f)
					{
						break;
					}
					else
					{
						minN = currentN;
					}
				}
				else if (distance <= 0)
				{
					maxN = currentN;
					collided = true;
				}

				currentN = (maxN + minN) / 2;

			}

			if (collided == true)
			{
				safePos = testPos;
				sphere1->SetNewPos(safePos);
			}

			return collided;
		}
	}

	return false;
}

Vector3f PhysicsManager::PlaneProjection(Sphere* sphere1, Plane* plane)
{
	// Project Sphere onto Plane
	Vector3f v = plane->GetPos() - sphere1->GetPos();

	float valueInX = v.dot(plane->right);
	float valueInY = v.dot(plane->forward);

	Vector3f localSpacePos = Vector3f(valueInX, 0.0f, valueInY);
	return localSpacePos;
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
	// Grab required data
	Sphere* sphere2 = static_cast<Sphere*>(point.contactID2);

	Vector3f s1Pos = point.contactID1->GetPos();
	Vector3f s1Vel = point.contactID1->GetVel();
	float s1Mass = point.contactID1->GetMass();

	Vector3f s2Vel = sphere2->GetVel();
	Vector3f s2Pos = sphere2->GetPos();
	float s2Mass = sphere2->GetMass();

	// Collision-related data
	//Vector3f cNormal = (s1Pos - s2Pos).normalise();
	Vector3f cNormal = point.contactNormal;
	//lineOfCentres = cNormal;

	// Calculate the oblique impact formula
	Vector3f VL1 = (((s1Mass - (m_CoR * s2Mass)) * (s1Vel.dot(cNormal) * cNormal)) +
		((s2Mass + (m_CoR * s2Mass)) * (s2Vel.dot(cNormal) * cNormal))) / (s1Mass + s2Mass);
	Vector3f VL2 = (((s1Mass + (m_CoR * s1Mass)) * (s1Vel.dot(cNormal) * cNormal)) +
		((s2Mass - (m_CoR * s1Mass)) * (s2Vel.dot(cNormal) * cNormal))) / (s1Mass + s2Mass);

	// Calculate new vectors based on previous formula
	Vector3f nV1 = (s1Vel - (s1Vel.dot(cNormal) * cNormal)) + VL1;
	Vector3f nV2 = (s2Vel - (s2Vel.dot(cNormal) * cNormal)) + VL2;

	// Apply vectors to spheres
	point.contactID1->SetNewVel(nV1);
	sphere2->SetNewVel(nV2);

	// TODO - Change to realistic Response
	/*
	Vector3f colNormal = point.contactNormal;

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(
		-1.0f * colNormal * colNormal.dot(point.contactID1->GetVel()));

	sphere2->ResetPos();
	sphere2->SetNewVel(
		-1.0f * colNormal * colNormal.dot(sphere2->GetVel()));
		*/



}

void PhysicsManager::SphereToPlaneCollisionResponse
(ManifoldPoint &point)
{
	Plane* plane = static_cast<Plane*>(point.contactID2);

	// TODO VRr = Vi - 2(Vi.dot(N))N
	Vector3f colNormal = plane->normal;

	point.contactID1->ResetPos();

	Vector3f newVel = point.contactID1->GetVel() - (1 + m_CoR)
		* (plane->normal.dot(point.contactID1->GetVel()) * plane->normal);
	/*
	Vector3f newVel = point.contactID1->GetVel() - 2 * (point.contactID1->GetVel().dot(colNormal)) * colNormal;
	*/

	point.contactID1->SetNewVel(newVel);
}

void PhysicsManager::CalculatePostPhysics(Sphere* sphere)
{

	sphere->SetVel(sphere->GetNewVel());
	sphere->SetPos(sphere->GetNewPos());

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

Vector3f PhysicsManager::CrossProduct(Vector3f vec1, Vector3f vec2)
{
	Vector3f vec3;

	vec3.SetX((vec1.GetY() * vec2.GetZ()) - (vec1.GetZ() * vec2.GetY()));
	vec3.SetY((vec1.GetZ() * vec2.GetX()) - (vec1.GetX() * vec2.GetZ()));
	vec3.SetZ((vec1.GetX() * vec2.GetY()) - (vec1.GetY() * vec2.GetX()));
	return vec3;
}