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

	void Dictionary::erase(const std::string& key)
	{
		const auto iterator = mDictionary.find(key);

		if (iterator != mDictionary.end())
		{
			mDictionary.erase(iterator);
		}
	}

	bool Dictionary::has(const std::string& key) const
	{
		return mDictionary.find(key) != mDictionary.end();
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
