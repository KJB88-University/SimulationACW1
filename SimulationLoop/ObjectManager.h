/*
#pragma once

#include "Vector3f.h"
#include <vector>
#include "Geometry.h"

using namespace std;

class ObjectManager
{

public:
	// Constructor / Destructor
	ObjectManager(void);
	~ObjectManager(void);

	// Properties
	void RemoveObject(int objID);
	void AddObject(Geometry* obj);
	int GetLength();

	vector<Geometry*> GetObjectList();

private:

	// Manager-specific functionality
	void UpdateLength();

	// Vars
	int vecLength;
	vector<Geometry*> objVector;
};
*/