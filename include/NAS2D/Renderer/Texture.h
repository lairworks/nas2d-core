#pragma once

#include "Vector.h"

#include <string>

namespace NAS2D
{
	class Texture
	{
	  public:
		virtual ~Texture() = 0 {};

		const Vector<int>& GetDimensions() const noexcept;
		const std::string& GetFilepath() const noexcept;

	  protected:
		Vector<int> mDimensions{};
		std::string mFilepath{};
	  private:
	};

} // namespace NAS2D