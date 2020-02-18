#include "NAS2D/DataUtils.h"

#include "NAS2D/StringUtils.h"

#include <stdexcept>

bool NAS2D::Dictionary::value_helper(const std::string& key, bool) const
{
	auto valueAsStr = value<std::string>(key);
	valueAsStr = NAS2D::trimWhitespace(NAS2D::toLowercase(valueAsStr));
	if (valueAsStr == "true" || valueAsStr == "false")
	{
		return valueAsStr == "true";
	}
	else
	{
		const auto valueAsBool = std::stoi(valueAsStr) != 0;
		return valueAsBool;
	}
}

char NAS2D::Dictionary::value_helper(const std::string& key, char) const
{
	const auto valueAsStr = value<std::string>(key);
	return valueAsStr.at(0);
}

signed char NAS2D::Dictionary::value_helper(const std::string& key, signed char) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsSChar = static_cast<signed char>(std::stoi(valueAsStr));
	return valueAsSChar;
}

unsigned char NAS2D::Dictionary::value_helper(const std::string& key, unsigned char) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsUChar = static_cast<unsigned char>(std::stoul(valueAsStr));
	return valueAsUChar;
}

short NAS2D::Dictionary::value_helper(const std::string& key, short) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsShort = static_cast<short>(std::stoi(valueAsStr));
	return valueAsShort;
}

unsigned short NAS2D::Dictionary::value_helper(const std::string& key, unsigned short) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsUShort = static_cast<unsigned short>(std::stoul(valueAsStr));
	return valueAsUShort;
}

int NAS2D::Dictionary::value_helper(const std::string& key, int) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsInt = std::stoi(valueAsStr);
	return valueAsInt;
}

unsigned int NAS2D::Dictionary::value_helper(const std::string& key, unsigned int) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsUInt = static_cast<unsigned int>(std::stoul(valueAsStr));
	return valueAsUInt;
}

long NAS2D::Dictionary::value_helper(const std::string& key, long) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsLong = std::stol(valueAsStr);
	return valueAsLong;
}

unsigned long NAS2D::Dictionary::value_helper(const std::string& key, unsigned long) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsULong = std::stoul(valueAsStr);
	return valueAsULong;
}

long long NAS2D::Dictionary::value_helper(const std::string& key, long long) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsLongLong = std::stoll(valueAsStr);
	return valueAsLongLong;
}

unsigned long long NAS2D::Dictionary::value_helper(const std::string& key, unsigned long long) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsULongLong = std::stoull(valueAsStr);
	return valueAsULongLong;
}

float NAS2D::Dictionary::value_helper(const std::string& key, float) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsFloat = std::stof(valueAsStr);
	return valueAsFloat;
}

double NAS2D::Dictionary::value_helper(const std::string& key, double) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsDouble = std::stod(valueAsStr);
	return valueAsDouble;
}

long double NAS2D::Dictionary::value_helper(const std::string& key, long double) const
{
	const auto valueAsStr = value<std::string>(key);
	const auto valueAsLongDouble = std::stold(valueAsStr);
	return valueAsLongDouble;
}

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
	const auto valueAsBool = [&valueAsStr]() -> bool {
		try
		{
			return std::stoi(valueAsStr) != 0;
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
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

	const auto valueAsSChar = [&valueAsStr]() {
		try
		{
			return static_cast<signed char>(std::stoi(valueAsStr));
		}
		catch(const std::invalid_argument&)
		{
			throw;
		}
		catch(const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsSChar;
}

unsigned char NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned char& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUChar = [&valueAsStr]() {
		try
		{
			return static_cast<unsigned char>(std::stoul(valueAsStr));
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsUChar;
}

short NAS2D::Dictionary::value_or_helper(const std::string& key, const short& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsShort = [&valueAsStr]() {
		try
		{
			return static_cast<short>(std::stoi(valueAsStr));
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsShort;
}

unsigned short NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned short& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUShort = [&valueAsStr]() {
		try
		{
			return static_cast<unsigned short>(std::stoul(valueAsStr));
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsUShort;
}


int NAS2D::Dictionary::value_or_helper(const std::string& key, const int& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsInt = [&valueAsStr]() {
		try
		{
			return std::stoi(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsInt;
}

unsigned int NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned int& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUInt = [&valueAsStr]() {
		try
		{
			return static_cast<unsigned int>(std::stoul(valueAsStr));
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsUInt;
}


long NAS2D::Dictionary::value_or_helper(const std::string& key, const long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsLong = [&valueAsStr]() {
		try
		{
			return std::stol(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsLong;
}

unsigned long NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsUL = [&valueAsStr]() {
		try
		{
			return std::stoul(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsUL;
}

long long NAS2D::Dictionary::value_or_helper(const std::string& key, const long long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsLL = [&valueAsStr]() {
		try
		{
			return std::stoll(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsLL;
}

unsigned long long NAS2D::Dictionary::value_or_helper(const std::string& key, const unsigned long long& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsULongLong = [&valueAsStr]() {
		try
		{
			return std::stoull(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsULongLong;
}


float NAS2D::Dictionary::value_or_helper(const std::string& key, const float& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsFloat = [&valueAsStr]() {
		try
		{
			return std::stof(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsFloat;
}

double NAS2D::Dictionary::value_or_helper(const std::string& key, const double& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsDouble = [&valueAsStr]() {
		try
		{
			return std::stod(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsDouble;
}

long double NAS2D::Dictionary::value_or_helper(const std::string& key, const long double& defaultValue) const
{
	// invalid key returns empty std::string
	auto valueAsStr = value_or<std::string>(key, std::string{});
	if (valueAsStr.empty()) { return defaultValue; }

	const auto valueAsLongDouble = [&valueAsStr]() {
		try
		{
			return std::stold(valueAsStr);
		}
		catch (const std::invalid_argument&)
		{
			throw;
		}
		catch (const std::out_of_range&)
		{
			throw;
		}
	}(); // IILE
	return valueAsLongDouble;
}

