#pragma once


namespace NAS2D
{
	template <typename T> inline constexpr bool isLvalueRef = false;
	template <typename T> inline constexpr bool isLvalueRef<T&> = true;


	template <typename T> struct removeReference { using type = T; };
	template <typename T> struct removeReference<T&> { using type = T; };
	template <typename T> struct removeReference<T&&> { using type = T; };


	template <typename T> [[nodiscard]] inline constexpr T&& forward(typename removeReference<T>::type& value) noexcept
	{
		return static_cast<T&&>(value);
	}

	template <typename T> [[nodiscard]] inline constexpr T&& forward(typename removeReference<T>::type&& value) noexcept
	{
		static_assert(!isLvalueRef<T>, "Bad use of foward to convert rvalue to lvalue");
		return static_cast<T&&>(value);
	}
}
