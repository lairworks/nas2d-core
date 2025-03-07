#include "SdlVersionInfo.h"

#include <SDL2/SDL.h>


void printSdlVersionInfo()
{
	SDL_version compiled = SDL_version{ SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL };
	SDL_version linked;

	SDL_GetVersion(&linked);

	SDL_Log("Compiled against SDL version %u.%u.%u.\n", compiled.major, compiled.minor, compiled.patch);
	SDL_Log("Linked against SDL version %u.%u.%u.\n", linked.major, linked.minor, linked.patch);
}
