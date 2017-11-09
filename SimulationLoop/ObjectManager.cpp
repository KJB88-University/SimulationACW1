/*
#include "Vector3f.h"
#include <vector>
#include "Geometry.h"
#include "ObjectManager.h"

using namespace std;

// Constructor
ObjectManager::ObjectManager(void)
{

}

// Destructor
ObjectManager::~ObjectManager(void)
{
	UpdateLength();

	// Delete all elements to avoid memory leaks
	for (int i = 0; i < vecLength; ++i)
	{
		// Delete each element in turn
		delete objVector[i];
	}
}

// Remove an object from the vector at the given ID
void ObjectManager::RemoveObject(int objID)
{
	UpdateLength();

	// Check for illegal memory access
	if (objID < vecLength)
	{
		delete objVector[objID];
	}

	// Provides no feedback on success or failure!
}

// Add an object to the vector
void ObjectManager::AddObject(Geometry* obj)
{
	objVector.emplace_back(obj);
}

// Return vector of objects
vector<Geometry*> ObjectManager::GetObjectList()
{
	return objVector;
}

// Update length of vector before performing any
// length-dependent fuctions
void ObjectManager::UpdateLength()
{
	vecLength = objVector.size();
}

// Get the length (for external use)
int ObjectManager::GetLength()
{
	UpdateLength();
	return vecLength;
}
*/