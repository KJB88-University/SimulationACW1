#pragma once
#include "Geometry.h"
#include "Vector3f.h"
#include "Plane.h"
#include "Tray.h"
#include <vector>

using namespace std;
class Box
{
public:

	Box(void);
	~Box(void);
	/*
	static Plane* leftWall;
	static Plane* rightWall;
	static Plane* frontWall;
	static Plane* backWall;
	static Plane* topTray;
	static Plane* midTray;
	static Plane* botTray;
	*/

	// 0-2 Trays (Bot, Mid, Top)
	// 3-6 (Front, Back, Left, Right)
};