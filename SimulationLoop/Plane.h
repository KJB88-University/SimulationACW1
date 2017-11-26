#pragma once

#include "Geometry.h"
#include "Vector3f.h"
#include <vector>
#include "ObjectType.h"
#include "PlaneRotation.h"

using namespace std;

class Plane : public Geometry
{

public:

	Plane(Vector3f origin, float width, float depth, float height, Vector3f normal, Vector3f right, Vector3f forward);
	~Plane(void);

	// Obj Type
	const ObjectType objType = PLANE;

	// Local co-ords
	Vector3f normal;
	Vector3f right;
	Vector3f forward;

	void SetupTray(void);
	void Plane::MoveTrayIn(void);
	void Plane::MoveTrayOut(void);

	bool trayMove = false;

	// Simulation Loop Methods
	virtual void CalculatePhysics(float dt, float t) override;
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold, float t, float dt) override;
	virtual void CollisionResponse(ManifoldPoint &point) override;
	virtual void Update() override;

	virtual void Render() const;

private:

	// Plane dimensions
	float height;
	float width;
	float depth;

	// Copy of DT for movable Trays
	float m_dt;

	// Tray Movement vars
	bool hasTray = false;
	bool trayIn = true;
	bool trayLocked = false;

	Vector3f m_velocity;
};