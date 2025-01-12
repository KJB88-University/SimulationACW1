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
	virtual void CalculatePhysics(float dt, float t) override;
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold, float dt, float t) override;
	virtual void CollisionResponse(ManifoldPoint &point);

	virtual void Update(void) override;

	void Render(void);

private:

	float m_radius;

	// Clip section
	const double planeEqn[4] = { 0.0f, -1.0f, -0.0f, -0.0f };

	GLUquadric* quadric;
};

