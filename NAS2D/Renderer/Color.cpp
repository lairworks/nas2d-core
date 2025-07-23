// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Color.h"

using namespace NAS2D;

const Color Color::Black{0, 0, 0};
const Color Color::Blue{0, 0, 255};
const Color Color::Green{0, 255, 0};
const Color Color::Cyan{0, 255, 255};
const Color Color::DarkGreen{0, 128, 0};
const Color Color::DarkGray{64, 64, 64};
const Color Color::Gray{128, 128, 128};
const Color Color::LightGray{192, 192, 192};
const Color Color::CoolGray{128, 128, 143};
const Color Color::CoolLightGray{192, 192, 207};
const Color Color::CoolDarkGray{64, 64, 79};
const Color Color::WarmGray{143, 143, 128};
const Color Color::WarmLightGray{207, 207, 192};
const Color Color::WarmDarkGray{79, 79, 64};
const Color Color::Magenta{255, 0, 255};
const Color Color::Navy{35, 60, 85};
const Color Color::Orange{255, 128, 0};
const Color Color::Red{255, 0, 0};
const Color Color::Silver{192, 192, 192};
const Color Color::White{255, 255, 255};
const Color Color::Yellow{255, 255, 0};
const Color Color::Normal{255, 255, 255};
const Color Color::NormalZ{128, 128, 255};
const Color Color::NoAlpha{0, 0, 0, 0};


bool Color::operator==(Color other) const
{
	return (red == other.red) && (green == other.green) && (blue == other.blue) && (alpha == other.alpha);
}


bool Color::operator!=(Color other) const
{
	return !(*this == other);
}

Color Color::alphaFade(uint8_t newAlpha) const
{
	return {red, green, blue, newAlpha};
}

Color Color::alphaBlend(Color background) const
{
	return {
		static_cast<uint8_t>((red * alpha + background.red * (255 - alpha)) / 255),
		static_cast<uint8_t>((green * alpha + background.green * (255 - alpha)) / 255),
		static_cast<uint8_t>((blue * alpha + background.blue * (255 - alpha)) / 255),
		static_cast<uint8_t>((alpha * alpha + background.alpha * (255 - alpha)) / 255),
	};
}
