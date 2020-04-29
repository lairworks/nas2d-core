#include "Dictionary.h"


namespace NAS2D {
	Dictionary& Dictionary::operator+=(const Dictionary& other)
	{
		for (const auto& [key, value] : other.mDictionary)
		{
			mDictionary[key] = value;
		}
		return *this;
	}


	Dictionary operator+(Dictionary lhs, const Dictionary& rhs)
	{
		return lhs += rhs;
	}
}
