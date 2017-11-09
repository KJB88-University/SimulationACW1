#pragma once
#include <Windows.h>
#include <conio.h>
#include <map>

using namespace std;

class InputManager
{
public:
	// Get instance of this manager
	static InputManager* GetInstance();

	// Check the status of a key
	bool CheckKeyPress(char);
	bool CheckKeyHeld(char);
	bool CheckDownArrow(void);
	bool CheckUpArrow(void);
	bool CheckShift(void);

	// Change the value of a key
	void KeyUp(char);
	void KeyDown(char);

	// Special cases
	void UpArrowDown(void);
	void UpArrowUp(void);

	void DownArrowDown(void);
	void DownArrowUp(void);
	
	void ShiftUp(void);
	void ShiftDown(void);

private:
	// Private constructor to avoid instances being created
	InputManager(void);

	~InputManager(void);

	// Instance of this manager
	static InputManager* instance;
	const static int k_arraySize = 19;

	bool upArrow = false;
	bool downArrow = false;
	bool shiftToggle = false;

	char relevantKeys[k_arraySize] =
	{
		'1', '2', '3', '4', '5', '6', 'R', 'N', 'P',
		'U', 'J', 'I', 'K', 'O', 'L', 'W', 'A', 'S', 'D'
	};

	bool keys[k_arraySize];
	bool oldKeys[k_arraySize];


};