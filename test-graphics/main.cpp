// ==================================================================================
// = NAS2D Test App 2
// = Copyright © 2008 - 2019 LairWorks Entertainment
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/NAS2D.h"
#include "TestGraphics.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{

	try
	{
		NAS2D::Game game("NAS2D Graphics Test", "NAS2D_GraphicsTest", "LairWorks", argv[0]);
		game.go(new TestGraphics());
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
		// Recommend for portability and no more direct inclusion of <windows.h>...
		// --------------
		// NAS2D::MessageBox_Okay("Application Error", e.what(), MessageBoxSeverityLevel::Error);
		// --------------
		// Original test used
		// --------------
		// #ifdef WINDOWS
		// MessageBoxA(NULL, "Unknown error occurred.", "Unexpected Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		// #endif
	}
	catch(...)
	{
		std::cout << "Unknown error occurred." << std::endl;
		return 2;
		// Recomment for portability and no more direct inclusion of <windows.h>...
		// --------------
		// NAS2D::MessageBox_Okay("Unexpected Error", "Unknown error occurred", MessageBoxSeverityLevel::Error);
		// --------------
		// Original test used
		// --------------
		// #ifdef WINDOWS
		// MessageBoxA(NULL, "Unknown error occurred.", "Unexpected Error", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		// #endif
	}

	return 0;
}
