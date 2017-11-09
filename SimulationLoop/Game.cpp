#include "Game.h"
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\GLU.h>
#include <iostream>
#include "Hemisphere.h"

const float Game::scaleTweakable = 10;


// Constructor
Game::Game(HDC hdc) : m_hdc(hdc), m_previousTime(0)
{
	// Create / Get Managers
	inputManager = InputManager::GetInstance();
	physicsManager = PhysicsManager::GetInstance();

#pragma region PLANES

	// 'Front' face
	boxQuads.emplace_back
	(
		new Plane
		(
			Vector3f(-5 * scaleTweakable, -7.5 * scaleTweakable, -5 * scaleTweakable), // Bottom-left
			Vector3f(5 * scaleTweakable, -7.5 * scaleTweakable, -5 * scaleTweakable), // Bottom-right
			Vector3f(5 * scaleTweakable, 7.5 * scaleTweakable, -5 * scaleTweakable), // Top-right
			Vector3f(-5 * scaleTweakable, 7.5 * scaleTweakable, -5 * scaleTweakable) // Top-left
		)
	);

	// 'Back' face
	boxQuads.emplace_back
	(
		new Plane
		(
			Vector3f(-5 * scaleTweakable, -7.5 * scaleTweakable, 5 * scaleTweakable), // Bottom-left
			Vector3f(5 * scaleTweakable, -7.5 * scaleTweakable, 5 * scaleTweakable), // Bottom-right
			Vector3f(5 * scaleTweakable, 7.5 * scaleTweakable, 5 * scaleTweakable), // Top-right
			Vector3f(-5 * scaleTweakable, 7.5 * scaleTweakable, 5 * scaleTweakable) // Top-left
		)
	);

	// 'Left' face
	boxQuads.emplace_back
	(
		new Plane
		(
			Vector3f(-5 * scaleTweakable, -7.5 * scaleTweakable, 5 * scaleTweakable), // Bottom-left
			Vector3f(-5 * scaleTweakable, -7.5 * scaleTweakable, -5 * scaleTweakable), // Bottom-right
			Vector3f(-5 * scaleTweakable, 7.5 * scaleTweakable, -5 * scaleTweakable), // Top-right
			Vector3f(-5 * scaleTweakable, 7.5 * scaleTweakable, 5 * scaleTweakable) // Top-left
		)
	);

	// 'Right' face
	boxQuads.emplace_back
	(
		new Plane
		(
			Vector3f(5 * scaleTweakable, -7.5 * scaleTweakable, -5 * scaleTweakable), // Bottom-left
			Vector3f(5 * scaleTweakable, -7.5 * scaleTweakable, 5 * scaleTweakable), // Bottom-right
			Vector3f(5 * scaleTweakable, 7.5 * scaleTweakable, 5 * scaleTweakable), // Top-right
			Vector3f(5 * scaleTweakable, 7.5 * scaleTweakable, -5 * scaleTweakable) // Top-left
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
			0.5f * scaleTweakable,
			false
		)
	);

	objVector.emplace_back
	(
		new Sphere
		(
			Vector3f(0, -15, 0),
			Vector3f(-1.0, 20, 0),
			0.02f,
			0.5f * scaleTweakable,
			false
		)
	);

#pragma endregion

	hemisphere = new Hemisphere(Vector3f(0.0f, 7.5f * scaleTweakable, 0.0f), 1.0f, 20.0f * scaleTweakable);

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
		objVector.emplace_back(new Sphere(
			Vector3f(0, 0, 0),
			Vector3f(0.5, 0, 0),
			75.0f,
			5.0f,
			false));
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
	}

	if (inputManager->CheckKeyPress('4'))
	{
		// TODO - add bottom tray
	}

	if (inputManager->CheckKeyPress('5'))
	{
		// TODO - Remove bottom tray
	}

	if (inputManager->CheckKeyPress('6'))
	{
		// TODO - Add bottom tray
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

	// Camera Zoom out
	if (inputManager->CheckDownArrow())
	{
		if (inputManager->CheckShift())
		{
			pan_z -= baseSpeed * shiftMultipler * m_dt;
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
			pan_z += baseSpeed * shiftMultipler * m_dt;
		}
		else
		{
			pan_z += baseSpeed * m_dt;
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
			pan_y -= baseSpeed * shiftMultipler * m_dt;
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
			pan_y += baseSpeed * shiftMultipler * m_dt;
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
			pan_x += baseSpeed * shiftMultipler * m_dt;
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
			pan_x -= baseSpeed * shiftMultipler * m_dt;
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
			pan_z -= baseSpeed * shiftMultipler * m_dt;
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
			pan_z += baseSpeed * shiftMultipler * m_dt;
		}
		else
		{
			pan_z += baseSpeed * m_dt;
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
}


// Update the physics calculations on each object
void Game::CalculateObjectPhysics()
{
	int length = objVector.size();
	for (int i = 0; i < length; i++)
	{
		objVector[i]->CalculatePhysics(m_dt);
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
				objVector[i]->CollisionDetection(objVector[j], m_manifold);
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
	glTranslatef(pan_x, pan_y, pan_z);
	glRotatef(rotate_x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotate_y, 0.0f, 1.0f, 0.0f);
	gluLookAt(0, 50, 100, 0, 0, 0, 0, 1, 0);

	//// Draw plane (at y=-20)
	//glBegin(GL_QUADS);
	//glColor3d(1, 1, 1);
	//glVertex3d(-50, -20, -50);
	//glVertex3d(50, -20, -50);
	//glVertex3d(50, -20, 50);
	//glVertex3d(-50, -20, 50);
	//glEnd();

	// BOWL
	hemisphere->Render();

	// BOX
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < static_cast<int>(boxQuads.size()); ++i)
	{
		boxQuads[i]->Render();
	}


	// SPHERES
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < static_cast<int>(objVector.size()); ++i)
	{
		objVector[i]->Render();
	}
	SwapBuffers(m_hdc);				// Swap Buffers (Double Buffering)
}

