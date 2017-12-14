#pragma once

#include "Vector3f.h"

class Sphere;
class Geometry;

struct ManifoldPoint 
{
	Sphere *contactID1;
	Geometry *contactID2;
	Vector3f contactNormal;
	float collisionTime;
};

class ContactManifold
{
public:
	ContactManifold(void);
	~ContactManifold(void);

	void Add(ManifoldPoint point);
	void Clear();
	int GetNumPoints() const;
	ManifoldPoint& GetPoint(int index);

private:
	// ******************************************************************************************
	// The max number of points in the manifold is not verified in the code, so you need to test!
	// Maybe it would be safer to use a dyamic data sturcture (although this may be too slow)?
	// ******************************************************************************************
	ManifoldPoint m_points[1000];
	int m_numOfPoints;
};

