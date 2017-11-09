#pragma once

#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>
#include "ContactManifold.h"

// Base class 
class Geometry
{
public:
	// Constructors / Destructors
	Geometry(Vector3f pos, float mass, bool isStatic);
	~Geometry(void);
	Geometry(void);

	// Does this geometry move or interact dynamically?
	const bool k_isStaticGeometry;

	// 'Properties'
	virtual Vector3f GetPos() const;
	virtual float GetMass() const;
	virtual int GetID() const;

	// Simulation Loop Methods
	virtual void CalculatePhysics(float dt) = 0;
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold) = 0;
	virtual void CollisionResponse(ManifoldPoint &point) = 0;
	virtual void Update() = 0;

	virtual void Render() const = 0;

protected:
	// Static counter for objID
	static int countID;

	// Data Members
	const float k_mass;
	const int k_objectID;

	Vector3f m_pos;
	GLuint m_texture;

	// 'Properties'
	virtual void SetPos(Vector3f inPos);
};