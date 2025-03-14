#include "FilesystemPathParents.h"

#include <algorithm>


namespace
{
	std::size_t parentCount(const NAS2D::FilesystemPath& path)
	{
		const std::size_t emptyRelativePathAdjustment = path.isRelative() ? 1 : 0;
		return path.componentCount() + emptyRelativePathAdjustment;
	}

}


namespace NAS2D
{
	static_assert(std::forward_iterator<FilesystemPathParents::Iterator>);


	FilesystemPathParents::FilesystemPathParents(const FilesystemPath& folder) :
		FilesystemPathParents(folder, parentCount(folder))
	{
	}


	FilesystemPathParents::FilesystemPathParents(const FilesystemPath& folder, std::size_t maxLevels) :
		mPath{folder},
		mMaxLevels{std::min(maxLevels, parentCount(folder))}
	{
	}


	std::size_t FilesystemPathParents::size() const
	{
		return mMaxLevels;
	}


	FilesystemPathParents::Iterator FilesystemPathParents::begin() const
	{
		return {mPath, mMaxLevels};
	}


	FilesystemPathParents::Iterator FilesystemPathParents::end() const
	{
		return {};
	}


	FilesystemPathParents::Iterator::Iterator() :
		FilesystemPathParents::Iterator::Iterator("", 0)
	{
	}


	FilesystemPathParents::Iterator::Iterator(const FilesystemPath& folder, std::size_t levelsRemaining) :
		mPath{folder},
		mLevelsRemaining{levelsRemaining}
	{
	}


	bool FilesystemPathParents::Iterator::operator==(const Iterator& other) const
	{
		return mLevelsRemaining == other.mLevelsRemaining && (mLevelsRemaining == 0 || mPath == other.mPath);
	}


	FilesystemPathParents::Iterator& FilesystemPathParents::Iterator::operator++()
	{
		if (mLevelsRemaining == 0)
		{
			return *this;
		}

		mPath = mPath.parent();
		--mLevelsRemaining;
		if (mLevelsRemaining > 1 && mPath.string().size() <= 1)
		{
			mLevelsRemaining = 1;
		}
		return *this;
	}


	FilesystemPathParents::Iterator FilesystemPathParents::Iterator::operator++(int)
	{
		auto temp = *this;
		++*this;
		return temp;
	}


	const FilesystemPath& FilesystemPathParents::Iterator::operator*() const
	{
		return mPath;
	}
}
