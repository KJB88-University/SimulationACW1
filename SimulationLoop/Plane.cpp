#include "Plane.h"
#include "Game.h"

Plane::Plane(Vector3f origin, float width, float depth, float height, Vector3f normal, Vector3f right, Vector3f forward) :
	Geometry(origin, 1.0f, true, objType), normal(normal), forward(forward), right(right), width(width), depth(depth), height(height)
{
	/*
	Vector3f tempPos;

	// DEFINE CORNERS
	// Top Left
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + -length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + -width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + -width);
	}
	m_Bounds.emplace_back(tempPos);

	// Top Right
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + -length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + width);
	}
	m_Bounds.emplace_back(tempPos);

	// Bottom Right
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + width);
	}
	m_Bounds.emplace_back(tempPos);

	// Bottom Left
	tempPos = origin;
	tempPos.SetY(tempPos.GetY() + length);
	if (inRotation == Z_AXIS)
	{
		tempPos.SetX(tempPos.GetX() + -width);
	}
	else if (inRotation == X_AXIS)
	{
		tempPos.SetZ(tempPos.GetZ() + -width);
	}
	m_Bounds.emplace_back(tempPos);
	*/

}

/*
Plane::Plane(Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4, Vector3f normal, Vector3f right, Vector3f forward) :
	Geometry((p4 - p2), 1.0f, false, PLANE), normal(normal), forward(forward), right(right)
{
	m_Bounds.emplace_back(p1);
	m_Bounds.emplace_back(p2);
	m_Bounds.emplace_back(p3);
	m_Bounds.emplace_back(p4);

	// Tray Control
	hasTray = false;

	//depth = p4.distance(p3);
}
*/
Plane::~Plane(void)
{

}


void Plane::SetupTray(void)
{
	hasTray = true;
	m_velocity = Vector3f(1.0f * Game::scaleTweakable, 0.0f, 0.0f);
}

void Plane::MoveTray(void)
{
	trayMove = true;
}

void Plane::Update(void)
{
	// If we have a tray
	if (hasTray)
	{
		/*
		// And it's not current moving
		if (!trayLocked)
		{
			// Set the velocity of the tray to move out
			if (trayMove && trayIn)
			{
				trayLocked = true;
				m_velocity = Vector3f(1.0f * Game::scaleTweakable * m_dt, 0.0f, 0.0f);

			}
			// Set the velocity of the tray to move in
			else if (trayMove && !trayIn)
			{
				trayLocked = true;
				m_velocity = Vector3f(-1.0f * Game::scaleTweakable * m_dt, 0.0f, 0.0f);
			}

		}

		// If we've reached maximum distance, cancel moving and lock
		if (m_pos.GetX() >= (10.0f * Game::scaleTweakable))
		{
			trayMove = false;
			trayLocked = false;
		}
		// If we've reached minimum distance, cancel moving and lock
		else if (m_pos.GetX() <= 0.0f)
		{
			trayMove = false;
			trayLocked = false;
		}
	}
	*/
		m_velocity = m_velocity;
		m_pos = m_pos + m_velocity * m_dt;
	}
	//m_pos = m_pos + m_velocity;
	// TODO - if plane reaches point, stop plane
}

void Plane::CalculatePhysics(float dt, double t)
{
	m_dt = dt;
}

void Plane::CollisionDetection(Geometry* geometry, ContactManifold* contactManifold)
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


	/*
	// Standard box plane
	else if (!hasTray)
	{
		if (rotationInAxis == Z_AXIS)
		{
			// Top Left
			glVertex3d(m_pos.GetX() + -width, m_pos.GetY(), m_pos.GetZ() + length);

			// Top Right
			glVertex3d(m_pos.GetX() + width, m_pos.GetY(), m_pos.GetZ() + length);

			// Bottom Right
			glVertex3d(m_pos.GetX() + width, m_pos.GetY(), m_pos.GetZ() + -length);

			// Bottom Left
			glVertex3d(m_pos.GetX() + -width, m_pos.GetY(), m_pos.GetZ() + -length);
		}
		else if (rotationInAxis == X_AXIS)
		{
			// Top Left
			glVertex3d(m_pos.GetX() + -width, m_pos.GetY(), m_pos.GetZ() + length);

			// Top Right
			glVertex3d(m_pos.GetX() + width, m_pos.GetY(), m_pos.GetZ() + length);

			// Bottom Right
			glVertex3d(m_pos.GetX() + width, m_pos.GetY(), m_pos.GetZ() + -length);

			// Bottom Left
			glVertex3d(m_pos.GetX() + -width, m_pos.GetY(), m_pos.GetZ() + -length);
		}
	}
	*/
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);
}

