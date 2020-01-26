#include "NAS2D/Renderer/Texture.h"

using namespace NAS2D;

const Vector<int>& Texture::GetDimensions() const noexcept
{
	return mDimensions;
}

const std::string& NAS2D::Texture::GetFilepath() const noexcept
{
	return mFilepath;
}
