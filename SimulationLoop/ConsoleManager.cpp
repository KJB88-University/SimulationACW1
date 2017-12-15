#include "ConsoleManager.h"
#include <iostream>
#include "Game.h"
using namespace std;

ConsoleManager* ConsoleManager::instance = 0;

ConsoleManager* ConsoleManager::GetInstance(void)
{
	if (instance == 0)
	{
		instance = new ConsoleManager();
	}

	return instance;
}

ConsoleManager::ConsoleManager(void)
{
	CoR = 0.5f;
	CoF = 0.5f;
	noOfBalls = 0;
	timeScale = 1.0f;

	physics = PhysicsManager::GetInstance();
}

ConsoleManager::~ConsoleManager(void)
{
}
void ConsoleManager::UpdateCoR(void)
{
	CoR = physics->GetCoR();
	PromptUpdate();
}

void ConsoleManager::UpdateCoF(void)
{
	CoF = physics->GetCoF();
	PromptUpdate();
}

void ConsoleManager::UpdateBalls(void)
{
	noOfBalls = Game::noOfBalls;
	PromptUpdate();
}

void ConsoleManager::UpdateTimeScale(float ts)
{
	timeScale = ts;
	PromptUpdate();
}

void ConsoleManager::InitConsoleThread(void)
{
	thread consoleThread = thread(&ConsoleManager::ThreadedConsoleUpdate, this);
	consoleThread.join();
}

void ConsoleManager::PromptUpdate(void)
{
	updateNow = true;
	thread consoleThread = thread(&ConsoleManager::ThreadedConsoleUpdate, this);
	consoleThread.join();
}

void ConsoleManager::StopUpdating(void)
{
	stopConsole = true;
}
void ConsoleManager::ThreadedConsoleUpdate(void)
{
	//system("CLS"); // This makes me sad

		if (stopConsole)
		{
			return;
		}

		if (updateNow)
		{
			
			cout << "CoR Magnitude: " << CoR << "\n" <<
				"Friction Magnitude: " << CoF << "\n" <<
				"No. of Balls: " << noOfBalls << "\n" <<
				"Time Scale: " << timeScale << endl;
			cout << "------------------------------------" << endl;
			//cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // Clear console

			updateNow = false;
		}

}

