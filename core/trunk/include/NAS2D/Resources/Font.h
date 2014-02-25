// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_FONTRESOURCE_
#define _NAS_FONTRESOURCE_

#include "Resource.h"

#ifdef __APPLE__
#include "SDL2_ttf/SDL_ttf.h"
#elif _WIN32
#include "GLee.h"
#define NO_SDL_GLEXT
#include "SDL/SDL_ttf.h"
#else
#include "SDL2/SDL_ttf.h"
#endif

#include <map>

namespace NAS2D {

/**
 *  \class Font
 *  \brief Font Resource.
 *
 *  Implements a Font resource.
 */
class Font: public Resource
{
public:
	Font();
	Font(const std::string& filePath, int ptSize = 12);
	Font(const std::string& filePath, int glyphWidth, int glyphHeight, int glyphSpace);
    Font(const Font& font);
    Font& operator=(const Font& font);
	~Font();

	int width(const std::string& str) const;
	int height() const;
	int ascent() const;

	int ptSize() const;

	const std::string& typefaceName() const;

	void bold();
	void italic();
	void underline();
	void normal();

    struct GlyphMetrics
    {
        float uvX, uvY;	// Texture coordinates.
        float uvW, uvH; // Texture coordinates.
        int minX, minY;
        int maxX, maxY;
        int advance;
    };

    const int glyphCellWidth() const;
    const int glyphCellHeight() const;
    const Font::GlyphMetrics& glyphMetrics(int glyph) const;
    unsigned int texture_id() const;

protected:
	friend class Renderer;
    friend class OGL_Renderer;

private:
	struct FontInfo
	{
		FontInfo(): textureId(0), fontSize(0) {}
		FontInfo(unsigned int id, unsigned int fontSize): textureId(id), fontSize(fontSize) {}
		
		void operator()(unsigned int id, unsigned int size) { textureId = id; fontSize = size; }

		unsigned int textureId;
		unsigned int fontSize;
		int ref_count;
	};

	typedef std::map<std::string, FontInfo> FontMap;
    typedef std::vector<GlyphMetrics> GlyphMetricsList;

	void load();
	void loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace);

	void generateGlyphMap(TTF_Font* ft);
	void generateTexture(SDL_Surface *src);

	int					mHeight;			/**< Font Height. */
	int					mAscent;			/**< Height of each glyph relative to the baseline. */
	int					mPtSize;			/**< Point Size to load the Font in. */

	Point_2d			mGlyphCellSize;		/**< Size in pixels of each cell that glyphs occupy. */

	unsigned int		mTextureId;			/**< OpenGL Texture ID. */

	std::string			mFontName;			/**< Full typeface name. */

	GlyphMetricsList	mGlyphMetrics;		/**< Metrics for each glyph. */

	static FontMap		_FontMap;			/*< Lookup table for OpenGL Texture ID's. */
};

} // namespace

#endif

