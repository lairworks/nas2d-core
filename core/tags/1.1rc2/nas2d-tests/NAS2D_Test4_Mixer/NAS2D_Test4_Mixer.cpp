// ==================================================================================
// = NAS2D Test 3: Blending and Blend Modes
// ==================================================================================
// = Provides an example of setting different blend modes and displays the output.
// ==================================================================================

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if !defined(_DEBUG)
#pragma comment( linker, "/subsystem:windows" )
#endif

#endif // #ifdef WIN32

#include "NAS2D/NAS2D.h"
#include "Test4State.h"

#include <string>

int main(int argc, char *argv[])
{
	try
	{
		Game game("NAS2D Test App4: Mixer", argv[0]);
		game.go(new Test4State());
	}
	catch(const Exception &e)
	{
		stringstream errorMessage;
		errorMessage << "Error (" << e.getCode() << "):" << endl << endl << e.getDescription();

		#ifdef WIN32
		MessageBoxA(NULL, errorMessage.str().c_str(), e.getBriefDescription().c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif

		cout << errorMessage << endl;
		return -1;
	}
	catch(const bad_alloc&)
	{
		stringstream errorMessage;
		errorMessage << "An unhandled memory allocation error occured.";

		#ifdef WIN32
		MessageBoxA(NULL, errorMessage.str().c_str(), "Out of Memory", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif

		cout << errorMessage << endl;
		return -2;
	}

	return 0;
}
