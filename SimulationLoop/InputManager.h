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
	const bool CheckKeyPress(char);
	const bool CheckKeyHeld(char) const;
	const bool CheckDownArrow(void) const;
	const bool CheckUpArrow(void) const;
	const bool CheckShift(void) const;
	const bool CheckLeftArrow(void) const;
	const bool CheckRightArrow(void) const;
	const bool CheckPgUp(void) const;
	const bool CheckPgDown(void) const;

	// Change the value of a key
	void KeyUp(char);
	void KeyDown(char);

	// Special cases
	void UpArrowDown(void);
	void UpArrowUp(void);

	void DownArrowDown(void);
	void DownArrowUp(void);

	void LeftArrowDown(void);
	void LeftArrowUp(void);
	
	void RightArrowDown(void);
	void RightArrowUp(void);
	
	void ShiftUp(void);
	void ShiftDown(void);

	void PgUpDown(void);
	void PgUpUp(void);

	void PgDownDown(void);
	void PgDownUp(void);

private:
	// Private constructor to avoid instances being created
	InputManager(void);

	~InputManager(void);

	// Instance of this manager
	static InputManager* instance;
	const static int k_arraySize = 19;

	bool upArrow = false;
	bool downArrow = false;
	bool leftArrow = false;
	bool rightArrow = false;
	bool shiftToggle = false;
	bool pgDown = false;
	bool pgUp = false;

	char relevantKeys[k_arraySize] =
	{
		'1', '2', '3', '4', '5', '6', 'R', 'N', 'P',
		'U', 'J', 'I', 'K', 'O', 'L', 'W', 'A', 'S', 'D'
	};

	bool keys[k_arraySize];
	bool oldKeys[k_arraySize];


};