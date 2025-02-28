#include "SdlVersionInfo.h"

#include <SDL2/SDL.h>


void printSdlVersionInfo()
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	SDL_Log("Compiled against SDL version %u.%u.%u.\n", compiled.major, compiled.minor, compiled.patch);
	SDL_Log("Linked against SDL version %u.%u.%u.\n", linked.major, linked.minor, linked.patch);
}
