/*
#pragma once

#include "ContactManifold.h"
#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>
#include "Geometry.h"

class Cube : public Geometry
{
public:
	Cube(Vector3f pos, float mass);
	~Cube(void);

	Vector3f GetPos() const;

	void Render();

private:
	Vector3f m_pos;
	int m_objectID;
	GLuint m_texture;

	static int countID;

	void CalcFaces();
};
*/