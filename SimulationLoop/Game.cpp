#include "Game.h"
#include <gl\gl.h>                                // Header File For The OpenGL32 Library
#include <gl\GLU.h>
#include <iostream>
#include "Hemisphere.h"
#include "PlaneRotation.h"
#include <thread>
#include "ConsoleManager.h"

using namespace std;

class Plane;
class ConsoleManager;

int Game::noOfBalls = 0;
const float Game::scaleTweakable = 10;
float Game::m_dt = 0;

// Constructor
Game::Game(HDC hdc) : m_hdc(hdc), m_previousTime(0)
{

	// Create / Get Managers
	inputManager = InputManager::GetInstance();
	physicsManager = PhysicsManager::GetInstance();
	consoleManager = ConsoleManager::GetInstance();
	consoleManager->InitConsoleThread();

//#pragma region TRAYS
	
	// Bottom Tray
	tray1 = new Plane
	(
		Vector3f(0.0f, -4.5f * scaleTweakable, 0.0f),
		5.0f * scaleTweakable,
		5.0f * scaleTweakable,
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(1.0f, 0.0f, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f)
	);
	tray1->SetupTray();
	tray1->SetupFriction();
	objVector.emplace_back(tray1);

	/*
	// Mid Tray
	tray2 = new Plane
	(
		Vector3f(0.0f, 0.0f, 0.0f),
		5.0f * scaleTweakable,
		5.0f * scaleTweakable,
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(1.0f, 0.0f, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f)
	);
	tray2->SetupTray();
	objVector.emplace_back(tray2);
	// Add to Box container
	//physicsManager->planeList.emplace_back(tray2);
	*/

	// Top Tray
	tray3 = new Plane
	(
		Vector3f(0.0f, 4.5f * scaleTweakable, 0.0f),
		5.0f * scaleTweakable,
		5.0f * scaleTweakable,
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(1.0f, 0.0f, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f)
	);
	tray3->SetupTray();
	objVector.emplace_back(tray3);
#pragma endregion

#pragma region PLANES
	
	
	// 'Front' face
	Plane* plane = new Plane
		(
			Vector3f(0.0f, 0.0f, 5.0f * scaleTweakable),
			7.5f * scaleTweakable,
			5.0f * scaleTweakable,
			Vector3f(0.0f, 0.0f, -1.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(-1.0f, 0.0, 0.0f)
		);
	objVector.emplace_back(plane);

	// 'Back' face
	plane = new Plane
		(
			Vector3f(0.0f, 0.0f, -5.0f * scaleTweakable),
			7.5f * scaleTweakable,
			5.0f * scaleTweakable,
			Vector3f(0.0f, 0.0f, 1.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(1.0f, 0.0f, 0.0f)
		);
	objVector.emplace_back(plane);

	// 'Left' face
	plane = new Plane		
	(
			Vector3f(-5.0f * scaleTweakable, 0.0f, 0.0f),
			7.5f * scaleTweakable,
			5.0f * scaleTweakable,
			Vector3f(1.0f, 0.0f, 0.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, 1.0f)
		);
	objVector.emplace_back(plane);

	// 'Right' face
	plane = new Plane
		(
			Vector3f(5.0f * scaleTweakable, 0.0f, 0.0f),
			7.5f * scaleTweakable,
			5.0f * scaleTweakable,
			Vector3f(-1.0f, 0.0f, 0.0f),
			Vector3f(0.0f, 1.0f, 0.0f),
			Vector3f(0.0f, 0.0f, -1.0f)
		);
	objVector.emplace_back(plane);
	
	
#pragma endregion
	
	// Hemisphere
	objVector.emplace_back
	(
		new Hemisphere(
			Vector3f(0.0f, 7.5f * scaleTweakable, 0.0f),
			1.0f,
			20.0f * scaleTweakable)
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

	consoleManager->StopUpdating();
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

	// Add a sphere (no detection atm) TODO
	if (inputManager->CheckKeyPress('1'))
	{

		objVector.emplace_back(
			new Sphere(
				Vector3f(0.0f, 11.5f * scaleTweakable, 0.0f),
				Vector3f(1.25f * scaleTweakable, 0.0f, -0.5f * scaleTweakable),
				0.2f * scaleTweakable,
				0.5f * scaleTweakable));

		noOfBalls++;
		consoleManager->UpdateBalls();
	}


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

	// Change timescale
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

	// Friction control
	if (inputManager->CheckKeyPress('I'))
	{
		physicsManager->IncrementFriction();
		consoleManager->UpdateCoF();
	}

	if (inputManager->CheckKeyPress('K'))
	{
		physicsManager->DecrementFriction();
		consoleManager->UpdateCoF();
	}

	// Elasticity control
	if (inputManager->CheckKeyPress('O'))
	{
		physicsManager->IncrementElasticity();
		consoleManager->UpdateCoR();
	}

	if (inputManager->CheckKeyPress('L'))
	{
		physicsManager->DecrementElasticity();
		consoleManager->UpdateCoR();
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

