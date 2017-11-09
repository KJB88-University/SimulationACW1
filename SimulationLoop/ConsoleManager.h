#pragma once
#include <iostream>

class ConsoleManager
{

public:

	// Get instance of this manager
	static ConsoleManager* GetInstance();

	void UpdateConsole(int, float, float);
private:

	// Values to be displayed
	int noOfBalls;
	float eMag;
	float fMag;

	// Private constructor to avoid instances being created
	ConsoleManager(void);
	~ConsoleManager(void);

	void WriteConsole(void) const;
	void ClearConsole(void) const;

	// Instance of this manager
	static ConsoleManager* instance;
};