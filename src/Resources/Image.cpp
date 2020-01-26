// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/Resources/Image.h"

#include "NAS2D/Exception.h"
#include "NAS2D/File.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Resources/ImageInfo.h"
#include "NAS2D/StringUtils.h"
#include "NAS2D/Utility.h"

#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

#include "Thirdparty/stb/stb_image.h"

#include <iostream>
#include <map>
#include <string>

using namespace NAS2D;
using namespace NAS2D::Exception;

/**
 * Loads an Image from disk.
 *
 * \param filePath Path to an image file.
 */
Image::Image(const std::string& filePath, bool openGL_flipOnLoad /*= false*/)
: _filepath(filePath)
{
	auto file = Utility<Filesystem>::get().open(_filepath);
	if(file.empty()) {
		std::cout << "Could not load " << _filepath << std::endl;
		throw std::runtime_error("file not found");
	}
	else
	{
		int comp = 0;
		int req_comp = 4;
		std::vector<uint8_t> buffer(file.bytes().begin(), file.bytes().end());
		file.clear();
		stbi_set_flip_vertically_on_load(openGL_flipOnLoad ? 1 : 0);
		auto texel_bytes = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &_size.x, &_size.y, &comp, req_comp);
		if(!texel_bytes)
		{
			throw NAS2D::Exception::image_bad_data();
		}
		buffer.clear();
		buffer.shrink_to_fit();
		_bytesPerTexel = req_comp;
		_texelBytes = std::vector<uint8_t>(texel_bytes, texel_bytes + (static_cast<std::size_t>(_size.x) * _size.y * _bytesPerTexel));
		stbi_image_free(texel_bytes);
	}
}

/**
 * Create a blank Image of X, Y dimensions.
 *
 * \param	width	Width of the Image.
 * \param	height	Height of the Image.
 */
Image::Image(int width, int height)
: _size{width, height}
, _bytesPerTexel{4}
, _texelBytes(static_cast<std::size_t>(width) * height * _bytesPerTexel, 0u)
{
	/* DO NOTHING */
}

/**
 * Create an Image from a raw data buffer.
 *
 * \param	buffer			Pointer to a data buffer.
 * \param	width			Width of the Image.
 * \param	height			Height of the Image.
 */
Image::Image(uint8_t* buffer, int width, int height)
: _size{width, height}
, _bytesPerTexel(4)
, _texelBytes(buffer, buffer + (static_cast<std::size_t>(width) * height * _bytesPerTexel))
{
	/* DO NOTHING */
}

Image::Image(Image&& rhs) noexcept
: _size(std::move(rhs._size))
, _bytesPerTexel(std::move(rhs._bytesPerTexel))
, _filepath(std::move(rhs._filepath))
{
	std::scoped_lock<std::mutex, std::mutex> lock(_cs, rhs._cs);
	_texelBytes = std::move(rhs._texelBytes);
}

Image& Image::operator=(Image&& rhs) noexcept
{
	std::scoped_lock<std::mutex, std::mutex> lock(_cs, rhs._cs);
	_bytesPerTexel = std::move(rhs._bytesPerTexel);
	_size = std::move(rhs._size);
	_filepath = std::move(rhs._filepath);
	_texelBytes = std::move(rhs._texelBytes);

	rhs._bytesPerTexel = 0;
	rhs._size = Vector<int>{};
	rhs._filepath = std::string{};
	rhs._texelBytes.clear();
	rhs._texelBytes.shrink_to_fit();
	return *this;
}

/**
 * Gets the dimensions in pixels of the image.
 */
Vector<int> Image::size() const
{
	return _size;
}

/**
 * Gets the width in pixels of the image.
 */
int Image::width() const
{
	return _size.x;
}

/**
 * Gets the height in pixels of the image.
 */
int Image::height() const
{
	return _size.y;
}

void Image::setTexelColor(const Vector<int> texelPos, const Color& color)
{
	const std::size_t index = static_cast<std::size_t>(texelPos.x) + texelPos.y * static_cast<std::size_t>(_size.x);
	const std::size_t offset = index * _bytesPerTexel;

	_texelBytes[offset + 0u] = color.red();
	_texelBytes[offset + 1u] = color.green();
	_texelBytes[offset + 2u] = color.blue();
	if(_bytesPerTexel == 4)
	{
		_texelBytes[offset + 3u] = color.alpha();
	}
}

Color Image::getTexelColor(const Vector<int> texelPos) const
{
	const std::size_t index = static_cast<std::size_t>(texelPos.x) + texelPos.y * static_cast<std::size_t>(_size.x);
	const std::size_t offset = index * _bytesPerTexel;
	Color color;

	color.red(_texelBytes[offset + 0u]);
	color.green(_texelBytes[offset + 1u]);
	color.blue(_texelBytes[offset + 2u]);
	if(_bytesPerTexel == 4)
	{
		color.alpha(_texelBytes[offset + 3u]);
	}
	else
	{
		color.alpha(255);
	}
	return color;
}

const std::string& NAS2D::Image::GetFilepath() const noexcept
{
	return _filepath;
}

const uint8_t* NAS2D::Image::GetData() const noexcept
{
	return _texelBytes.data();
}
