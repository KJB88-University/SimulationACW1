#include "Game.h"
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\GLU.h>
#include <iostream>
#include "Hemisphere.h"
#include "PlaneRotation.h"

const float Game::scaleTweakable = 10;
float Game::m_dt = 0;

// Constructor
Game::Game(HDC hdc) : m_hdc(hdc), m_previousTime(0)
{

	// Create / Get Managers
	inputManager = InputManager::GetInstance();
	physicsManager = PhysicsManager::GetInstance();

#pragma region TRAYS

	// Bottom Tray
	tray1 = new Plane
	(
		/*
		Vector3f(-5.0f * scaleTweakable, -4.5f * scaleTweakable, 5.0f * scaleTweakable),
		Vector3f(5.0f * scaleTweakable, -4.5f * scaleTweakable, 5.0f * scaleTweakable),
		Vector3f(5.0f * scaleTweakable, -4.5f * scaleTweakable, -5.0f * scaleTweakable),
		Vector3f(-5.0f * scaleTweakable, -4.5f * scaleTweakable, -5.0f * scaleTweakable),
		*/
		Vector3f(0.0f, -4.5f * scaleTweakable, 0.0f),
		5.0f * scaleTweakable,
		5.0f * scaleTweakable,
		0.0f,
		Vector3f(0.0f, -1.0f, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f),
		Vector3f(1.0f, 0.0f, 0.0f)
	);
	tray1->SetupTray();
	objVector.emplace_back
	(
		tray1
	);


	// Mid Tray
	tray2 = new Plane
	(
		Vector3f(0.0f, 0.0f, 0.0f),
		5.0f * scaleTweakable,
		5.0f * scaleTweakable,
		0.0f,
		Vector3f(0.0f, -1.0f, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f),
		Vector3f(1.0f, 0.0f, 0.0f)
	);
	tray2->SetupTray();
	objVector.emplace_back
	(
		tray2
	);

	// Top Tray
	tray3 = new Plane
	(
		Vector3f(0.0f, 4.5f * scaleTweakable, 0.0f),
		5.0f * scaleTweakable,
		5.0f * scaleTweakable,
		0.0f,
		Vector3f(0.0f, -1.0f, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f),
		Vector3f(1.0f, 0.0f, 0.0f)
	);
	tray3->SetupTray();
	objVector.emplace_back
	(
		tray3
	);

#pragma endregion

#pragma region PLANES

	// 'Front' face
	objVector.emplace_back
	(
		new Plane
		(
			Vector3f(0.0f, 0.0f, 5.0f * scaleTweakable),
			5.0f * scaleTweakable,
			0.0f * scaleTweakable,
			7.5f * scaleTweakable,
			Vector3f(0.0f, 0.0f, -1.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(-1.0f, 0.0, 0.0f)
		)
	);

	// 'Back' face
	objVector.emplace_back
	(
		new Plane
		(
			Vector3f(0.0f, 0.0f, -5.0f * scaleTweakable),
			5.0f * scaleTweakable,
			0.0f * scaleTweakable,
			7.5f * scaleTweakable,
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(1.0f, 0.0f, 0.0f)
		)
	);


	// 'Left' face
	objVector.emplace_back
	(
		new Plane
		(
			Vector3f(-5.0f * scaleTweakable, 0.0f, 0.0f),
			0.0f * scaleTweakable,
			5.0f * scaleTweakable,
			7.5f * scaleTweakable,
			Vector3f(1.0f, 0.0f, 0.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 1.0f)
		)
	);

	// 'Right' face
	objVector.emplace_back
	(
		new Plane
		(
			Vector3f(5.0f * scaleTweakable, 0.0f, 0.0f),
			0.0f * scaleTweakable,
			5.0f * scaleTweakable,
			7.5f * scaleTweakable,
			Vector3f(-1.0f, 0.0f, 0.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, -1.0f)
		)
	);



#pragma endregion

#pragma region SPHERES

	objVector.emplace_back
	(
		new Sphere
		(
			Vector3f(0, 0, 0),
			Vector3f(0.5, 0, 0),
			0.02f,
			0.5f * scaleTweakable
		)
	);

	objVector.emplace_back
	(
		new Sphere
		(
			Vector3f(0, -15, 0),
			Vector3f(-1.0, 20, 0),
			0.02f,
			0.5f * scaleTweakable
		)
	);

#pragma endregion

	// Hemisphere
	objVector.emplace_back
	(
		new Hemisphere(Vector3f(0.0f, 7.5f * scaleTweakable, 0.0f), 1.0f, 20.0f * scaleTweakable)
	);

	// Manifold
	m_manifold = new ContactManifold();

	// Perf metrics
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
}

// Deconstructor
Game::~Game(void)
{
	// Delete all elements to avoid memory leaks
	for (int i = 0; i < static_cast<int>(objVector.size()); ++i)
	{
		// Delete each element in turn
		delete objVector[i];
	}

	// Delete contact manifold
	delete m_manifold;
}

void Game::Update()
{
	// The simulation loop should be on its own thread(s)
	CheckInput();
	SimulationLoop();
	Render();
}

void Game::CheckInput()
{

#pragma region Input Hooks

	if (inputManager->CheckKeyPress('1'))
	{
		// TODO - ADD BALL
		objVector.emplace_back(
			new Sphere(
				Vector3f(0, 0, 0),
				Vector3f(0.5, 0, 0),
				75.0f,
				5.0f));
	}

	/*
	if (inputManager->CheckKeyPress('2'))
	{
		// ADV - Cube
	}
	*/

	if (inputManager->CheckKeyPress('3'))
	{
		// TODO - Remove top tray
		tray3->MoveTrayOut();
	}

	if (inputManager->CheckKeyPress('4'))
	{
		// TODO - add top tray
		tray3->MoveTrayIn();
	}

	if (inputManager->CheckKeyPress('5'))
	{
		// TODO - Remove bottom tray
		tray1->MoveTrayOut();
	}

	if (inputManager->CheckKeyPress('6'))
	{
		// TODO - Add bottom tray
		tray1->MoveTrayIn();
	}

	if (inputManager->CheckKeyPress('R'))
	{
		// TODO - REset sim
	}

	/*
	if (inputManager->CheckKeyPress('N'))
	{
		// ADV - Toggle net
	}
	*/

	if (inputManager->CheckKeyPress('P'))
	{
		if (timeScale == 0.0f)
		{
			timeScale = originalTimeScale;
		}
		else
		{
			originalTimeScale = timeScale;
			timeScale = 0.0f;
		}
	}

	// Increase Time Scale
	if (inputManager->CheckKeyPress('U'))
	{
		if (timeScale < maxTimeClamp - 0.05f)
		{
			timeScale += timeScaleModifier;
		}
		else
		{
			timeScale = maxTimeClamp;
		}
	}

	// Decrease Time Scale
	if (inputManager->CheckKeyPress('J'))
	{
		if (timeScale > minTimeClamp + 0.05f)
		{
			timeScale -= timeScaleModifier;
		}
		else
		{
			timeScale = minTimeClamp;
		}
	}

	if (inputManager->CheckKeyPress('I'))
	{
		// TODO - Increase friction magnitude
	}

	if (inputManager->CheckKeyPress('K'))
	{
		// TODO - Decrease friction magnitude
	}

	if (inputManager->CheckKeyPress('O'))
	{
		// TODO - Increase elasticity magnitude
	}

	if (inputManager->CheckKeyPress('L'))
	{
		// TODO - Decrease elasticity magnitude
	}

	// Camera Up
	if (inputManager->CheckKeyHeld('W'))
	{
		if (inputManager->CheckShift())
		{
			pan_y -= baseSpeed * shiftMultiplier * m_dt;
		}
		else
		{
			pan_y -= baseSpeed * m_dt;
		}
	}

	// Camera Down
	if (inputManager->CheckKeyHeld('S'))
	{
		if (inputManager->CheckShift())
		{
			pan_y += baseSpeed * shiftMultiplier * m_dt;
		}
		else
		{
			pan_y += baseSpeed * m_dt;
		}

	}

	// Camera Left
	if (inputManager->CheckKeyHeld('A'))
	{
		if (inputManager->CheckShift())
		{
			pan_x += baseSpeed * shiftMultiplier * m_dt;
		}
		else
		{
			pan_x += baseSpeed * m_dt;
		}
	}

	// Camera Right
	if (inputManager->CheckKeyHeld('D'))
	{
		if (inputManager->CheckShift())
		{
			pan_x -= baseSpeed * shiftMultiplier * m_dt;
		}
		else
		{
			pan_x -= baseSpeed * m_dt;
		}
	}

	// Camera Zoom out
	if (inputManager->CheckDownArrow())
	{
		if (inputManager->CheckShift())
		{
			pan_z -= baseSpeed * shiftMultiplier * m_dt;
		}
		else
		{
			pan_z -= baseSpeed * m_dt;
		}
	}

	// Camera Zoom in
	if (inputManager->CheckUpArrow())
	{
		if (inputManager->CheckShift())
		{
			pan_z += baseSpeed * shiftMultiplier * m_dt;
		}
		else
		{
			pan_z += baseSpeed * m_dt;
		}
	}

	// Camera Y-Left rotation
	if (inputManager->CheckLeftArrow())
	{
		if (inputManager->CheckShift())
		{
			rotate_y -= baseSpeed * shiftMultiplier * m_dt;
		}

		else
		{
			rotate_y -= baseSpeed * m_dt;
		}

	}

	// Camera Y-Right rotation
	if (inputManager->CheckRightArrow())
	{
		if (inputManager->CheckShift())
		{
			rotate_y += baseSpeed * shiftMultiplier * m_dt;
		}

		else
		{
			rotate_y += baseSpeed * m_dt;
		}

	}

	// Camera X-Up rotation
	if (inputManager->CheckPgUp())
	{
		if (inputManager->CheckShift())
		{
			rotate_x += baseSpeed * shiftMultiplier * m_dt;
		}

		else
		{
			rotate_x += baseSpeed * m_dt;

		}
	}

	// Camera X-Down rotation
	if (inputManager->CheckPgDown())
	{
		if (inputManager->CheckShift())
		{
			rotate_x -= baseSpeed * shiftMultiplier * m_dt;
		}

		else
		{
			rotate_x -= baseSpeed * m_dt;
		}
	}
#pragma endregion

}

void Game::SimulationLoop()
{
	// Calculate dt based on the simulation loop rate using a timer
	QueryPerformanceCounter(&end);
	m_dt = static_cast<float>((end.QuadPart - start.QuadPart) / static_cast<double>(frequency.QuadPart));
	start = end;

	m_fps = static_cast<int>(1.0 / m_dt);

	// Apply timescale changes
	m_dt *= timeScale;

	/*
	1. Calc obj physics
	2. Clear manifold for next round of collision detection
	3. Detect collisions
	4. Determine response
	5. Update physics
	*/

	// Calculate the physic calculations on all objects (e.g. new position, velocity, etc)
	CalculateObjectPhysics();

	// Clear the manifold so that we can calculate all collisions for this simulation loop
	m_manifold->Clear();

	// Find dynamic collisions for all objects and add to contact manifold 
	DynamicCollisionDetection();

	// Handle dynamic collision responses using the contact manifold
	DynamicCollisionResponse();

	// Update the physics calculations on all objects (e.g. new position, velocity, etc)
	UpdateObjectPhysics();

	m_previousTime += m_dt;
}


// Update the physics calculations on each object
void Game::CalculateObjectPhysics()
{
	int length = objVector.size();
	for (int i = 0; i < length; i++)
	{
		objVector[i]->CalculatePhysics(m_dt, m_previousTime);
	}
}

// Handle dynamic collisions
void Game::DynamicCollisionDetection()
{
	int length = objVector.size();
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			if (objVector[j]->GetID() != objVector[i]->GetID())
			{
				objVector[i]->CollisionDetection(objVector[j], m_manifold, m_dt, m_previousTime);
			}
		}
	}
}

// Handle dynamic collision responses
void Game::DynamicCollisionResponse()
{

	for (int collision = 0; collision < m_manifold->GetNumPoints(); ++collision)
	{
		ManifoldPoint &point = m_manifold->GetPoint(collision);
		point.contactID1->CollisionResponse(point);
	}

}

// Update the physics calculations on each object
void Game::UpdateObjectPhysics()
{
	// Update objects
	for (int i = 0; i < static_cast<int>(objVector.size()); ++i)
	{
		objVector[i]->Update();
	}
}

// Render and display the scene in OpenGL
void Game::Render()									// Here's Where We Do All The Drawing
{
	// CAMERA SETUP
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();									// Reset The Current Modelview Matrix

	glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
	glTranslatef(pan_x, pan_y, pan_z);

	gluLookAt(0, 50, 100, 0, 0, 0, 0, 1, 0);

	//tray1->Render();
	//tray3->Render();
	// GEOMETRY
	for (int i = 0; i < static_cast<int>(objVector.size()); ++i)
	{
		objVector[i]->Render();
	}

	SwapBuffers(m_hdc);				// Swap Buffers (Double Buffering)
}

