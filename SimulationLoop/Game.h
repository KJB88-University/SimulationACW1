#pragma once

#include <Windows.h>
#include "Sphere.h"
#include "ContactManifold.h"
#include "Geometry.h"
#include <vector>
#include "InputManager.h"
#include "PhysicsManager.h"
#include "Plane.h"
#include "Hemisphere.h"

using namespace std;

class Game
{
public:
	// Modify the scale of the simulation by this
	const static float scaleTweakable;

	Game(HDC hdc);
	~Game(void);

	void Update();

private:
	void CheckInput();
	void SimulationLoop();
	void CalculateObjectPhysics();
	void DynamicCollisionDetection();
	void DynamicCollisionResponse();
	void UpdateObjectPhysics();
	void Render();

	//void SetupBalls();

	//void ResetSim();

private:

	PhysicsManager* physicsManager;
	InputManager* inputManager;

	Hemisphere* hemisphere;
	vector<Geometry*> objVector;
	vector<Plane*> boxQuads;


	HDC   m_hdc;

	double gameTime;
	float m_dt;
	float original_dt;
	int	  m_fps;
	float m_previousTime;

	ContactManifold *m_manifold;
	LARGE_INTEGER start, end, frequency;

	// Time Tweakable
	float timeScale = 1.0f;
	float originalTimeScale = 1.0f;
	float timeScaleModifier = 0.05f; // Multiplied by deltaTime

	const float maxTimeClamp = 2.0f;
	const float minTimeClamp = 0.0f;

	// Camera vars
	const float baseSpeed = 15.0f;
	const float shiftMultiplier = 3.0f;

	float pan_x = 0.0f;
	float pan_y = 0.0f;
	float pan_z = 0.0f;

	float rotate_x = 0.0f;
	float rotate_y = 0.0f;

	float zoomFactor;
	const float zoomMin = 3.0f;
	const float zoomMax = 10.0f;

};
