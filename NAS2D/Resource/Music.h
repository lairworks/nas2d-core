// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include <string_view>
#include <string>
#include <map>

#if defined(__XCODE_BUILD__)
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif


namespace NAS2D
{

	/**
	 *  Music resource.
	 */
	class Music
	{
	public:
		explicit Music(std::string_view filePath);

		Music(const Music& rhs) = delete;
		Music& operator=(const Music& rhs) = delete;

		~Music();

	protected:
		friend class MixerSDL;
		Mix_Music* music() const;

	private:
		const std::string mBuffer;
		Mix_Music* mMusic{nullptr};
	};

} // namespace
