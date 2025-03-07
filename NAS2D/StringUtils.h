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

#include <string>
#include <string_view>
#include <vector>


namespace NAS2D
{
	std::string toLowercase(std::string string);
	std::string toUppercase(std::string string);
	std::size_t countDelimiters(const std::string& string, char delimiter = ',');
	std::vector<std::string> split(const std::string& string, char delimiter = ',');
	std::pair<std::string, std::string> splitOnFirst(const std::string& string, char delimiter);
	std::pair<std::string, std::string> splitOnLast(const std::string& string, char delimiter);
	std::string join(const std::vector<std::string>& strings, std::string_view delimiter = {});
	std::string trimWhitespace(std::string_view string);
	bool startsWith(std::string_view string, std::string_view start) noexcept;
	bool endsWith(std::string_view string, std::string_view end) noexcept;
	bool startsWith(std::string_view string, char start) noexcept;
	bool endsWith(std::string_view string, char end) noexcept;
} // namespace NAS2D
