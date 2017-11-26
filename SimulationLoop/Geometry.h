#pragma once

#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>
#include "ContactManifold.h"
#include "ObjectType.h"

// Base class 
class Geometry
{
public:
	// Constructors / Destructors
	Geometry(Vector3f pos, float mass, bool isStatic, ObjectType inType);
	~Geometry(void);
	//Geometry(void);

	// Does this geometry move or interact dynamically?
	const bool k_isStaticGeometry;

	// Object type
	const ObjectType objType;

	// 'Properties'
	virtual Vector3f GetPos(void) const;
	virtual float GetMass(void) const;
	virtual int GetID(void) const;

	// Simulation Loop Methods
	virtual void CalculatePhysics(float dt, float t) = 0;
	virtual void CollisionDetection(Geometry* geometry, ContactManifold* contactManifold, float t, float dt) = 0;
	virtual void CollisionResponse(ManifoldPoint &point) = 0;
	virtual void Update(void) = 0;

	virtual void Render(void) const = 0;

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