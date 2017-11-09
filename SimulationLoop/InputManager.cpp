#include "InputManager.h"


using namespace std;
InputManager* InputManager::instance = 0;

// Classic Singleton setup
InputManager* InputManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new InputManager();
	}

	return instance;
}

// CONSTRUCTOR
InputManager::InputManager(void)
{
	// Fill keys with false values for default
	std::fill(std::begin(keys), std::end(keys), false);
	std::fill(std::begin(oldKeys), std::end(oldKeys), false);
	shiftToggle = false;
}

// DESTRUCTOR
InputManager::~InputManager(void)
{

}

// Return false if key is up,
// Return true is key is down,
// Return false is key is not recognised
bool InputManager::CheckKeyPress(char key)
{
	for (int i = 0; i < k_arraySize; ++i)
	{
		if (key == relevantKeys[i])
		{
			// Key pressed
			if (keys[i] == true && oldKeys[i] == false)
			{
				oldKeys[i] = keys[i];
				return keys[i];
			}
			// Key released
			else if (keys[i] == false && oldKeys[i] == true)
			{
				oldKeys[i] = keys[i];
				return keys[i];
			}
		}
	}

	return false;
}

bool InputManager::CheckKeyHeld(char key)
{
	for (int i = 0; i < k_arraySize; ++i)
	{
		if (key == relevantKeys[i])
		{
			if (keys[i] == true)
			{
				return keys[i];
			}
		}
	}

	return false;

}
// Set status of key to released (false)
void InputManager::KeyUp(char key)
{
	for (int i = 0; i < k_arraySize; ++i)
	{
		if (key == relevantKeys[i])
		{
			keys[i] = false;
		}
	}
}

// Set status of key to released (true)
void InputManager::KeyDown(char key)
{
	for (int i = 0; i < k_arraySize; ++i)
	{
		if (key == relevantKeys[i])
		{
			keys[i] = true;
		}
	}
}

// ARROW KEY
void InputManager::UpArrowDown()
{
	upArrow = true;
}

void InputManager::UpArrowUp()
{
	upArrow = false;
}

void InputManager::DownArrowDown()
{
	downArrow = true;
}

void InputManager::DownArrowUp()
{
	downArrow = false;
}

bool InputManager::CheckDownArrow()
{
	return downArrow;
}

bool InputManager::CheckUpArrow()
{
	return upArrow;
}

// SHIFT TOGGLE
void InputManager::ShiftDown()
{
	shiftToggle = true;
}

void InputManager::ShiftUp()
{
	shiftToggle = false;
}

bool InputManager::CheckShift()
{
	return shiftToggle;
}