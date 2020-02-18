#include "NAS2D/DataUtils.h"

#include "NAS2D/StringUtils.h"

#include <stdexcept>

bool NAS2D::Dictionary::value_or_helper(const std::string& key, const bool& defaultValue) const
{
	// invalid key returns empty std::string
	// value may still be invalid out of range.
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	valueAsStr = NAS2D::trimWhitespace(NAS2D::toLowercase(valueAsStr));
	if (valueAsStr == "true" || valueAsStr == "false")
	{
		return valueAsStr == "true";
	}
	const auto valueAsBool = std::stoi(valueAsStr) != 0;
	return valueAsBool;
}


char NAS2D::Dictionary::value_or_helper(const std::string& key, const char& defaultValue) const noexcept
{
	// invalid key returns empty std::string
	const auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	return valueAsStr[0];
}

signed char NAS2D::Dictionary::value_or_helper(const std::string& key, const signed char& defaultValue) const
{
	// invalid key returns empty std::string
	const auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsSChar = static_cast<signed char>(std::stoi(valueAsStr));
	return valueAsSChar;
}

unsigned char NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned char& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUChar = static_cast<unsigned char>(std::stoul(valueAsStr));
	return valueAsUChar;
}

short NAS2D::Dictionary::value_or_helper(const std::string& key, const short& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsShort = static_cast<short>(std::stoi(valueAsStr));
	return valueAsShort;
}

unsigned short NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned short& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUShort = static_cast<unsigned short>(std::stoul(valueAsStr));
	return valueAsUShort;
}


int NAS2D::Dictionary::value_or_helper(const std::string& key, const int& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsInt = std::stoi(valueAsStr);
	return valueAsInt;
}

unsigned int NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned int& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUInt = static_cast<unsigned int>(std::stoul(valueAsStr));
	return valueAsUInt;
}


long NAS2D::Dictionary::value_or_helper(const std::string& key, const long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsLong = std::stol(valueAsStr);
	return valueAsLong;
}

unsigned long NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUL = std::stoul(valueAsStr);
	return valueAsUL;
}

long long NAS2D::Dictionary::value_or_helper(const std::string& key, const long long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsLL = std::stoll(valueAsStr);
	return valueAsLL;
}

unsigned long long NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned long long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsULongLong = std::stoull(valueAsStr);
	return valueAsULongLong;
}


float NAS2D::Dictionary::value_or_helper(const std::string& key, const float& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsFloat = std::stof(valueAsStr);
	return valueAsFloat;
}

double NAS2D::Dictionary::value_or_helper(const std::string& key, const double& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsDouble = std::stod(valueAsStr);
	return valueAsDouble;
}

long double NAS2D::Dictionary::value_or_helper(const std::string& key, const long double& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsLongDouble = std::stold(valueAsStr);
	return valueAsLongDouble;
}

