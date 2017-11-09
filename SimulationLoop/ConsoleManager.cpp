#include <iostream>
#include <string>
#include "ConsoleManager.h"

using namespace std;

ConsoleManager* ConsoleManager::instance = 0;

ConsoleManager::ConsoleManager(void)
{
	noOfBalls = 0;
	eMag = 0.0f;
	fMag = 0.0f;
}

ConsoleManager::~ConsoleManager(void)
{

}

ConsoleManager* ConsoleManager::GetInstance(void)
{
	if (instance == 0)
	{
		instance = new ConsoleManager();
	}

	return instance;
}

void ConsoleManager::ClearConsole(void) const
{
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
}

void ConsoleManager::UpdateConsole(int inNoOfBalls, float inEMag, float inFMag)
{
	ClearConsole();
	WriteConsole();
}

void ConsoleManager::WriteConsole(void) const
{
	cout << "No. of Balls: " << noOfBalls << "\n"
		<< "Elasticity Mag: " << eMag << "\n"
		<< "Friction Mag: " << fMag << endl;
}