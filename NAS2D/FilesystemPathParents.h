#pragma once

#include "FilesystemPath.h"


namespace NAS2D
{
	class FilesystemPathParents
	{
	public:
		class Iterator;

		FilesystemPathParents(const FilesystemPath& folder);
		FilesystemPathParents(const FilesystemPath& folder, std::size_t maxLevels);

		std::size_t size() const;

		Iterator begin() const;
		Iterator end() const;

	private:
		FilesystemPath mPath;
		std::size_t mMaxLevels;


	public:
		class Iterator
		{
		public:
			using value_type = FilesystemPath;
			using difference_type = std::ptrdiff_t;

			Iterator();
			Iterator(const FilesystemPath& folder, std::size_t levelsRemaining);

			bool operator==(const Iterator& other) const;

			Iterator& operator++();
			Iterator operator++(int);

			const FilesystemPath& operator*() const;

		private:
			FilesystemPath mPath;
			std::size_t mLevelsRemaining;
		};
	};
}
