#include "EnumKeyModifier.h"

#include <type_traits>


namespace NAS2D
{
	KeyModifier& operator|=(KeyModifier& a, const KeyModifier& b) noexcept
	{
		using underlying = std::underlying_type_t<KeyModifier>;
		auto underlying_a = static_cast<underlying>(a);
		auto underlying_b = static_cast<underlying>(b);
		a = static_cast<KeyModifier>(underlying_a | underlying_b);
		return a;
	}

	KeyModifier& operator&=(KeyModifier& a, const KeyModifier& b) noexcept
	{
		using underlying = std::underlying_type_t<KeyModifier>;
		auto underlying_a = static_cast<underlying>(a);
		auto underlying_b = static_cast<underlying>(b);
		a = static_cast<KeyModifier>(underlying_a & underlying_b);
		return a;
	}

	KeyModifier operator|(KeyModifier a, const KeyModifier& b) noexcept
	{
		a |= b;
		return a;
	}

	KeyModifier operator&(KeyModifier a, const KeyModifier& b) noexcept
	{
		a &= b;
		return a;
	}
}
