// ==================================================================================
// = Zombie Splash
// = Copyright © 2011, New Age Software
// ==================================================================================
// = 
// ==================================================================================

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if !defined(_DEBUG)
#pragma comment( linker, "/subsystem:windows" )
#endif

#endif // #ifdef WIN32

#include "NAS2D/NAS2D.h"
#include "GameState.h"

#include <string>

/** \todo Find a sane place to define this. */
const std::string REPORT_MESSAGE("\n\nPlease report the error and the steps taken to\nreproduce it to the development team.");
const std::string APPLICATION_TITLE("Blood Bath");

int main(int argc, char *argv[])
{
	try
	{
		Game game(APPLICATION_TITLE, argv[0]);
		game.go(new GameState());
	}
	catch(const Exception &e)
	{
		stringstream errorMessage;
		errorMessage << "Error (" << e.getCode() << "):" << endl << endl << e.getDescription() << REPORT_MESSAGE;

		#ifdef WIN32
		MessageBoxA(NULL, errorMessage.str().c_str(), e.getBriefDescription().c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif

		cout << errorMessage << endl;
		return -1;
	}
	catch(const bad_alloc&)
	{
		stringstream errorMessage;
		errorMessage << "An unhandled memory allocation error occured and " << APPLICATION_TITLE << " needs to close." << REPORT_MESSAGE;

		#ifdef WIN32
		MessageBoxA(NULL, errorMessage.str().c_str(), "Out of Memory", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif

		cout << errorMessage << endl;
		return -2;
	}
	/*
	catch(...)
	{
		stringstream errorMessage;
		errorMessage << "An unexpected error occured and " << APPLICATION_TITLE << " needs to close." << REPORT_MESSAGE;

		#ifdef WIN32
		MessageBoxA(NULL, errorMessage.str().c_str(), "Unexpected Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif

		Logger::log << errorMessage << endl;
		return -1;
	}
	*/

	return 0;
}
