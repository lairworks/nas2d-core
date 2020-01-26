#pragma once

#include "NAS2D/Renderer/Texture.h"

namespace NAS2D
{
	class TextureOpenGL : public Texture
	{
	  public:
		TextureOpenGL(unsigned int id);
		virtual ~TextureOpenGL() = default;

		unsigned int Id() const noexcept;
	  protected:
	  private:
		unsigned int _id{0};
	};

} // namespace NAS2D