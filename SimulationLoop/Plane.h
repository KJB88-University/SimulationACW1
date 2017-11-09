#pragma once

#include "Geometry.h"
#include "Vector3f.h"
#include <vector>

using namespace std;

class Plane
{

public:

	Plane(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4);
	~Plane(void);

	vector<Vector3f> GetBounds(void) const;

	void SetupTray(void);
	void ToggleTray(void);
	void Render(void);

private:

	bool hasTray;
	bool trayIn;
	vector<Vector3f> m_Bounds;
	const Vector3f normalDir;
};