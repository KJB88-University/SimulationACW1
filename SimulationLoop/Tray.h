#pragma once

#include "Geometry.h"
#include <vector>

using namespace std;

class Tray : public Geometry
{
public:

	Tray(Vector3f origin, float width, float length, Vector3f normal, Vector3f forward, Vector3f right);
	~Tray(void);

	Vector3f normal;
	Vector3f forward;
	Vector3f right;

	// Simulation Loop Methods
	virtual void CalculatePhysics(float dt, double t);
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold);
	virtual void CollisionResponse(ManifoldPoint &point);
	virtual void Update(void);

	virtual void Render(void) const;

private:

	vector<Vector3f> m_Bounds;
};