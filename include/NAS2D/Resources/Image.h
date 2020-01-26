// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "../Renderer/Color.h"
#include "../Renderer/Vector.h"

#include <mutex>
#include <string>
#include <utility>
#include <vector>

namespace NAS2D
{
	/**
 * \class Image
 * \brief Image Class
 *
 * Provides a high level interface for image files. Can load the following formats:
 * - BMP
 * - JPEG
 * - PCX
 * - PNG
 * - TGA
 * - TIFF
 * - WEBP
 * - PSD
 * - GIF (including Animated GIF89a or GIF87a
 * - HDR
 * - PIC
 * - PNM
 *
 * \note	Image currently only supports 24-bit and 32-bit images (true-color with
 *			and without an alpha channel).
 *
 */
	class Image
	{
	  public:
		Image() = default;
		explicit Image(const std::string& filePath, bool openGL_flipOnLoad = false);
		Image(uint8_t* buffer, int width, int height);
		Image(int width, int height);

		Image(Image&& rhs) noexcept;
		Image& operator=(Image&& rhs) noexcept;
		Image(const Image& rhs) = delete;
		Image& operator=(const Image& rhs) = delete;

		~Image() = default;

		Vector<int> size() const;
		int width() const;
		int height() const;

		void setTexelColor(const Vector<int> texelPos, const Color& color);
		Color getTexelColor(const Vector<int> texelPos) const;
		
		const std::string& GetFilepath() const noexcept;

		const uint8_t* GetData() const noexcept;
	  private:
		Vector<int> _size{};
		int _bytesPerTexel{4};
		std::string _filepath{};
		std::vector<uint8_t> _texelBytes{};
		std::mutex _cs{};
	};

	/**
 * \typedef	ImagePtrList
 * \brief	A list of pointers to Image objects.
 */
	using ImagePtrList = std::vector<Image*>;

	/**
 * \typedef	ImageList
 * \brief	A list of Image objects.
 */
	using ImageList = std::vector<Image>;

} // namespace NAS2D
