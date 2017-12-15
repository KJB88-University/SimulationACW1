#pragma once

#include "Geometry.h"
#include "Vector3f.h"
#include <vector>
#include "ObjectType.h"
#include "PlaneRotation.h"
#include "Box.h"

using namespace std;

class Plane : public Geometry
{

public:

	Plane(Vector3f origin, float width, float length, Vector3f normal, Vector3f right, Vector3f forward);
	~Plane(void);

	// Local co-ords
	Vector3f normal;
	Vector3f right;
	Vector3f forward;

	// Vertex Positions
	Vector3f topLeft;
	Vector3f topRight;
	Vector3f botRight;
	Vector3f botLeft;

	void SetupTray(void);
	void MoveTrayIn(void);
	void MoveTrayOut(void);

	void ResetTray();
	bool trayMove = false;

	// Plane dimensions
	float length;
	float width;
	float depth;

	// Friction
	void SetupFriction(void);
	bool friction = false;

	// Simulation Loop Methods
	virtual void CalculatePhysics(float dt, float t) override;
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold, float t, float dt) override;
	virtual void CollisionResponse(ManifoldPoint &point) override;
	virtual void Update() override;

	virtual void Render();

	bool hasTray = false;
private:

	// Copy of DT for movable Trays
	float m_dt;

	// Tray Movement vars
	bool trayIn = true;
	bool trayLocked = false;

	Vector3f m_velocity;
};