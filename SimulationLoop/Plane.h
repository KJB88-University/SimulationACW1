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

	Plane(Vector3f origin, float width, float depth, Vector3f normal, Vector3f right, Vector3f forward, PlaneRotation inRotation);
	Plane(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4, Vector3f normal, Vector3f right, Vector3f forward);
	~Plane(void);

	vector<Vector3f> GetBounds(void) const;
	Vector3f normal;
	Vector3f right;
	Vector3f forward;

	void SetupTray(void);
	void MoveTray(void);

	bool trayMove = false;

	// Simulation Loop Methods
	virtual void CalculatePhysics(float dt, double t);
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold);
	virtual void CollisionResponse(ManifoldPoint &point);
	virtual void Update();

	virtual void Render() const;

private:

	float m_dt;
	Vector3f m_velocity;
	ObjectType objType = PLANE;
	bool hasTray = false;
	bool trayIn = true;
	bool trayLocked = false;
	vector<Vector3f> m_Bounds;
};