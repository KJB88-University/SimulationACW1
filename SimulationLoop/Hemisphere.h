#pragma once

#include "ContactManifold.h"
#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>  
#include "Geometry.h"
#include <gl\GLU.h>

class Hemisphere : public Geometry
{
public:
	// Constructor / Destructor
	Hemisphere(Vector3f pos, float mass, float radius);
	~Hemisphere(void);

	// Hemisphere-specific Properties
	float GetRadius(void) const;

	// Simulation Loop Methods
	virtual void CalculatePhysics(float dt, double t);
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold);
	virtual void CollisionResponse(ManifoldPoint &point);

	virtual void Update(void);

	void Render(void) const;

private:

	float m_radius;

	// Clip section
	const double planeEqn[4] = { 0.0f, -1.0f, -0.0f, -0.0f };

	GLUquadric* quadric;
};

