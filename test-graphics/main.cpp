// ==================================================================================
// = NAS2D Test App 2
// = Copyright © 2008 - 2019 LairWorks Entertainment
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "TestGraphics.h"

#include <NAS2D/Game.h>
#include <NAS2D/Utility.h>
#include <NAS2D/Configuration.h>
#include <NAS2D/Dictionary.h>

#include <iostream>
#include <string>


int main()
{
	try
	{
		NAS2D::Game game("NAS2D Graphics Test", "NAS2D_GraphicsTest", "LairWorks");
		game.go(new TestGraphics());
	}
	catch(std::exception& e)
	{
		// For portable modal popups (once Renderer is initialized), use:
		// window.doModalError("Application Error", e.what());
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}
