#pragma once
#include <thread>
#include <istream>
#include <iostream>
#include "PhysicsManager.h"

class ConsoleManager
{
public:
	static ConsoleManager* GetInstance(void);

	// Update info
	void UpdateCoF(void);
	void UpdateCoR(void);
	void UpdateBalls(void);
	void UpdateTimeScale(float timeScale);

	// Initialize the thread
	void InitConsoleThread(void);

	// Prompt a console update
	void PromptUpdate(void);

	// End updating loop
	void StopUpdating(void);

private:

	ConsoleManager(void);
	~ConsoleManager(void);

	float CoR = 0;
	float CoF = 0;
	int noOfBalls = 0;
	float timeScale = 0;

	bool stopConsole = false;
	bool updateNow = true;
	static ConsoleManager* instance;

	PhysicsManager* physics;
	void ThreadedConsoleUpdate(void);
};