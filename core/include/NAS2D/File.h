// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_FILE_
#define _NAS_FILE_

#include <string>

/**
 * \class File
 * \brief Represent a File object.
 *
 * The File object represents a file as a stream of bytes.
 */
class File
{
public:
	/**
	 * Default Constructor
	 */
	File()
	{}

	/**
	 * Constructor
	 * 
	 * \param	byteStream	A stream of bytes stored in a \c std::string object.
	 * \param	fileName	The full name of the file including path.
	 */
	File(const std::string& byteStream, const std::string& fileName):	mByteStream(byteStream), mFileName(fileName)
	{}

	~File()
	{}

	typedef char byte;
	typedef const char const_byte;
	typedef const_byte* RawByteStream;

	typedef std::string::iterator iterator;
	typedef std::string::reverse_iterator reverse_iterator;
	typedef std::string ByteStream;

	/**
	 * Gets a reference to the internal ByteStream.
	 *
	 * \note	This gets a \c non- const reference to the internal \c ByteStream
	 *			so that modifications can be made as necessary.
	 */
	ByteStream& bytes() { return mByteStream; }


	/**
	 * Gets a reference to the internal ByteStream.
	 *
	 * \note	This gets a \c const reference to the internal \c ByteStream.
	 */
	const ByteStream& bytes() const { return mByteStream; }


	/**
	 * Gets a raw pointer to a \c const \c byte stream.
	 *
	 * \note	This function is provided as a convenience for
	 *			use with low-level and legacy C libraries that
	 *			need a const char* byte stream.
	 */
	RawByteStream raw_bytes() const { return mByteStream.c_str(); }


	/**
	 * Gets the size, in bytes, of the file.
	 */
	size_t size() const { return mByteStream.size(); }

	bool empty() const { return mByteStream.empty(); }

	iterator begin() { return mByteStream.begin(); }
	iterator end() { return mByteStream.end(); }

	reverse_iterator rbegin() { return mByteStream.rbegin(); }
	reverse_iterator rend() { return mByteStream.rend(); }

	iterator seek(size_t pos) { iterator it = mByteStream.begin() + pos; return it; }
	reverse_iterator rseek(size_t pos) { reverse_iterator it = mByteStream.rbegin() + pos; return it; }

	byte& operator[](size_t pos) { return mByteStream[pos]; }
	const_byte& operator[](size_t pos) const { return mByteStream[pos]; }

	/**
	 * Clears the File and leaves it completely empty.
	 */
	void clear() { mByteStream = ""; mFileName = ""; }


	/**
	 * Gets the file name.
	 *
	 * \note	Filenames include both the individual file's
	 *			name and full directory path.
	 */
	std::string filename() const { return mFileName; }

private:
	ByteStream	mByteStream;	/**< Internal stream of bytes. */
	std::string	mFileName;		/**< Internal filename including directory path. */
};

#endif