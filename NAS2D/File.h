// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include <string>
#include <utility>


namespace NAS2D {

class File
{
public:

	using byte = char;
	using const_byte = const char;
	using RawByteStream = const_byte*;

	using iterator = std::string::iterator;
	using reverse_iterator = std::string::reverse_iterator;
	using ByteStream = std::string;


	File()
	{}

	File(ByteStream stream, std::string name) :
		mByteStream(std::move(stream)),
		mFileName(std::move(name))
	{}

	~File()
	{}


	File(const File& _f) :
		mByteStream(_f.mByteStream),
		mFileName(_f.mFileName)
	{}

	File& operator=(const File& _f)
	{
		mByteStream = _f.mByteStream;
		mFileName = _f.mFileName;
		return *this;
	}

	ByteStream& bytes() { return mByteStream; }
	const ByteStream& bytes() const { return mByteStream; }

	RawByteStream raw_bytes() const { return mByteStream.c_str(); }

	std::size_t size() const { return mByteStream.size(); }
	std::size_t length() const { return size(); }

	void resize(std::size_t size) { mByteStream.resize(size); }
	void resize(std::size_t size, byte b) { mByteStream.resize(size, b); }
	bool empty() const { return mByteStream.empty(); }

	iterator begin() { return mByteStream.begin(); }
	iterator end() { return mByteStream.end(); }
	reverse_iterator rbegin() { return mByteStream.rbegin(); }
	reverse_iterator rend() { return mByteStream.rend(); }
	iterator seek(std::ptrdiff_t pos) { iterator it = mByteStream.begin() + pos; return it; }
	reverse_iterator rseek(std::ptrdiff_t pos) { reverse_iterator it = mByteStream.rbegin() + pos; return it; }

	byte& operator[](std::size_t pos) { return mByteStream[pos]; }
	const_byte& operator[](std::size_t pos) const { return mByteStream[pos]; }

	void clear() { mByteStream = ""; mFileName = ""; }
	std::string filename() const { return mFileName; }

private:
	ByteStream mByteStream;
	//TODO: Change type to std::filesystem::path
	std::string mFileName;
};

} // namespace
