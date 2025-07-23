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

#include <cstdint>

namespace NAS2D
{

	struct Color
	{
		uint8_t red = 255;
		uint8_t green = 255;
		uint8_t blue = 255;
		uint8_t alpha = 255;


		bool operator==(Color other) const;
		bool operator!=(Color other) const;

		Color alphaFade(uint8_t newAlpha) const;
		Color alphaBlend(Color background) const;


		static const Color Black;
		static const Color Blue;
		static const Color Green;
		static const Color Cyan;
		static const Color DarkGreen;
		static const Color DarkGray;
		static const Color Gray;
		static const Color LightGray;
		static const Color CoolGray;
		static const Color CoolLightGray;
		static const Color CoolDarkGray;
		static const Color WarmGray;
		static const Color WarmLightGray;
		static const Color WarmDarkGray;
		static const Color Magenta;
		static const Color Navy;
		static const Color Orange;
		static const Color Red;
		static const Color Silver;
		static const Color White;
		static const Color Yellow;
		static const Color Normal;
		static const Color NormalZ;
		static const Color NoAlpha;
	};

} // namespace NAS2D
