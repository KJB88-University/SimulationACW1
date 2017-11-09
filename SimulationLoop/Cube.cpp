/*

#include "Cube.h"
#include "Vector3f.h"
#include <Windows.h>
#include <gl\gl.h>  
#include <gl\GLU.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "TextureLoader.h"
int Cube::countID = 0;

int cubeVerts[16];

// Pos represents the CENTERPOINT of the cube
Cube::Cube(Vector3f pos, float mass) 
	: Geometry::Geometry(pos, mass)
{
	m_texture = TextureLoader::LoadBMP("check.bmp");
}

Cube::~Cube(void)
{
	// Front-Face

}

Vector3f Cube::GetPos() const
{
	return Vector3f();
}

void Cube::CalcFaces()
{

}

void Cube::Render()
{
	glPushMatrix();

	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);
	glVertex3d(-50, -20, -50);
	glVertex3d(50, -20, -50);
	glVertex3d(50, -20, 50);
	glVertex3d(-50, -20, 50);
	glEnd();

	glPopMatrix();
}
*/