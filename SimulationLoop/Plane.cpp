#include "Plane.h"
#include "Game.h"

Plane::Plane(Vector3f origin, float width, float depth, float height, Vector3f normal, Vector3f right, Vector3f forward) :
	Geometry(origin, 1.0f, true, PLANE), normal(normal), forward(forward), right(right), width(width), depth(depth), height(height)
{
}

Plane::~Plane(void)
{

}


void Plane::SetupTray(void)
{
	hasTray = true;
	m_velocity = Vector3f(0.0f, 0.0f, 0.0f);
	//m_velocity = Vector3f(1.0f * Game::scaleTweakable, 0.0f, 0.0f);
}

void Plane::MoveTrayIn(void)
{
	if (trayIn == true)
	{
		trayMove = true;
	}
}

void Plane::MoveTrayOut(void)
{
	if (trayIn == false)
	{
		trayMove = true;
	}
}

void Plane::Update(void)
{
	// If we have a tray
	if (hasTray)
	{
		// And it's not current moving
		if (!trayLocked)
		{
			// Set the velocity of the tray to move out
			if (trayMove && trayIn)
			{
				trayLocked = true; // Stop further input until transition is complete

				// Give velocity to move
				m_velocity = Vector3f(1.0f * Game::scaleTweakable, 0.0f, 0.0f);
			}
			// Set the velocity of the tray to move in
			else if (trayMove && !trayIn)
			{
				trayLocked = true; // Stop further input until transition is complete

				// Give velocity to move
				m_velocity = Vector3f(-1.0f * Game::scaleTweakable * 2.0f, 0.0f, 0.0f);
			}
		}

		// If the tray has received input and should be moving,
		// and if the tray is not receiving more input
		if (trayMove && trayLocked)
		{
			// If we've reached maximum distance, cancel moving and lock
			if (trayIn && m_pos.GetX() >= (10.0f * Game::scaleTweakable))
			{
				// Set vel to zero
				m_velocity = Vector3f(0.0f, 0.0f, 0.0f);

				trayMove = false; // We are no longer wanting to move
				trayLocked = false; // The plane can now receive new input
				trayIn = false; // The tray is outside the box
			}
			// If we've reached minimum distance, cancel moving and lock
			else if (!trayIn && m_pos.GetX() <= 0.0f)
			{
				// Set vel to zero
				m_velocity = Vector3f(0.0f, 0.0f, 0.0f);

				trayMove = false; // We are no longer wanting to move
				trayLocked = false; // The plane can now receive new input
				trayIn = true; // The tray is inside the box
			}
		}

		//m_velocity = m_velocity;

		// Update position
		m_pos = m_pos + m_velocity * m_dt;
	}
	//m_pos = m_pos + m_velocity;
	// TODO - if plane reaches point, stop plane
}

void Plane::CalculatePhysics(float dt, float t)
{
	m_dt = dt;
}

void Plane::CollisionDetection(Geometry* geometry, ContactManifold* contactManifold, float t, float dt)
{

}

void Plane::CollisionResponse(ManifoldPoint &point)
{

}

/*
vector<Vector3f> Plane::GetBounds(void) const
{
	return m_Bounds;
}
*/

void Plane::Render(void) const
{
	// Draw plane (at y=-20)
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);
	glColor3d(1, 1, 1);

	if (!hasTray)
	{
		// Top Left
		glVertex3d(m_pos.GetX() + -width, m_pos.GetY() + height, m_pos.GetZ() + depth);

		// Top Right
		glVertex3d(m_pos.GetX() + width, m_pos.GetY() + height, m_pos.GetZ() + -depth);

		// Bottom Right
		glVertex3d(m_pos.GetX() + width, m_pos.GetY() + -height, m_pos.GetZ() + -depth);

		// Bottom Left
		glVertex3d(m_pos.GetX() + -width, m_pos.GetY() + -height, m_pos.GetZ() + depth);
	}
	else
	{
		// Top Left
		glVertex3d(m_pos.GetX() + -width, m_pos.GetY() + height, m_pos.GetZ() + -depth);

		// Top Right
		glVertex3d(m_pos.GetX() + width, m_pos.GetY() + height, m_pos.GetZ() + -depth);

		// Bottom Right
		glVertex3d(m_pos.GetX() + width, m_pos.GetY() + -height, m_pos.GetZ() + depth);

		// Bottom Left
		glVertex3d(m_pos.GetX() + -width, m_pos.GetY() + -height, m_pos.GetZ() + depth);
	}

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
}

