// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2014 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/NAS2D.h"

#include "Test2State.h"

using namespace std;

int main(int argc, char *argv[])
{

	try
	{
		Game game("NAS2D Test 2: Renderer Functions", argv[0]);
		game.go(new Test2State());
	}
	catch(Exception e)
	{
		stringstream errorMessage;
		errorMessage << "Error (" << e.getCode() << "):" << endl << endl << e.getDescription();
	
		#if defined(WIN32)
		MessageBoxA(NULL, errorMessage.str().c_str(), e.getBriefDescription().c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#endif
	}
	catch(...)
	{
	}

	return 0;
}
