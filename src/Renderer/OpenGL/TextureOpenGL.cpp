#include "NAS2D/Renderer/OpenGL/TextureOpenGL.h"

using namespace NAS2D;

TextureOpenGL::TextureOpenGL(unsigned int id)
: Texture()
, _id(id)
{
	/* DO NOTHING */
}

unsigned int NAS2D::TextureOpenGL::Id() const noexcept
{
	return _id;
}
