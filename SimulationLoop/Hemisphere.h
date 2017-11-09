#pragma once

#include "ContactManifold.h"
#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>  
#include "Geometry.h"
#include <gl\GLU.h>

class Hemisphere
{
public:
	// Constructor / Destructor
	Hemisphere(Vector3f pos, float mass, float radius);
	~Hemisphere(void);

	// Hemisphere-specific Properties
	float GetRadius(void) const;
	Vector3f GetPos(void) const;
	float GetMass(void) const;

	void ResetPos(void);

	void Render(void) const;

private:

	Vector3f m_pos;
	float m_radius;
	float m_mass;

	// Clip section
	const double planeEqn[4] = { 0.0f, -1.0f * 10.0f, -0.0f, -0.0f };

	GLUquadric* quadric;
};

