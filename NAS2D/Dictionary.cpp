#include "Dictionary.h"
#include "ContainerUtils.h"


namespace NAS2D {
	Dictionary& Dictionary::operator+=(const Dictionary& other)
	{
		for (const auto& [key, value] : other.mDictionary)
		{
			mDictionary[key] = value;
		}
		return *this;
	}

	std::vector<std::string> Dictionary::keys() const
	{
		return getKeys(mDictionary);
	}


	Dictionary operator+(Dictionary lhs, const Dictionary& rhs)
	{
		return lhs += rhs;
	}
}
