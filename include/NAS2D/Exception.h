// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include <string>

namespace NAS2D {

namespace exception {

/**
 * \class Exception
 * \brief Thrown when Exceptions occur.
 *
 * \deprecated	This class has been deprecated and will be removed in future versions of NAS2D. See
 *				current NAS2D documentation (http://www.lairworks.com/nas2d_doc/) for recommended usage.
 */
class Exception
{
private:
	Exception(int code, const std::string& brief, const std::string& description);
	~Exception();
	
	/**
	 * Gets a brief description of the Exception.
	 */
	const std::string& getBriefDescription() const;

	/**
	 * Gets a full description of the Exception.
	 */
	const std::string& getDescription() const;

	/**
	 * Gets an Exception ID.
	 */
	int getCode() const;

	std::string		mErrorBrief;			/**< Brief description of the error. */
	std::string		mErrorDescription;		/**< Full description of the Error. */
	int				mErrorCode;				/**< Error Code */
};


/**
 * \page Exceptions Exceptions
 * 
 * See <a href="https://github.com/lairworks/nas2d-core/wiki/Exceptions-&-Exception-Handling-in-NAS2D">Wiki Page (Exceptions)</a> for further details.
 */

/**
 * \class filesystem_backend_init_failure
 * 
 * Thrown when the Filesystem is unable to initialize itself.
 */
class filesystem_backend_init_failure : public std::runtime_error { public: filesystem_backend_init_failure(const std::string& description); filesystem_backend_init_failure(); };


/**
 * \class filesystem_not_initialized
 *
 * Thrown when any Filesystem operations are attempted before Filesystem::init() is called.
 */
class filesystem_not_initialized : public std::runtime_error { public: filesystem_not_initialized(); };


/**
 * \class filesystem_already_initialized
 *
 * Thrown when Filesystem::init() is called but the Filesystem is already initialized.
 */
class filesystem_already_initialized : public std::runtime_error { public: filesystem_already_initialized(); };


/**
 * \class filesystem_file_handle_still_open
 *
 * Thrown when the Filesystem is unable to close a file handle.
 */
class filesystem_file_handle_still_open : public std::runtime_error { public: filesystem_file_handle_still_open(const std::string& description); filesystem_file_handle_still_open();};


/**
 * \class image_bad_data
 *
 * Thrown when an Image resource contains an invalid pixel buffer.
 */
class image_bad_data : public std::runtime_error { public: image_bad_data(); };


/**
 * \class image_null_data
 *
 * Thrown when an Image resource contains no data in its pixel buffer.
 */
class image_null_data : public std::runtime_error { public: image_null_data(); };


/**
 * \class image_unsupported_bit_depth
 *
 * Thrown when attempting to load or perform operations on an image file in
 * an 8-bit or 16-bit format. See Image for further details.
 */
class image_unsupported_bit_depth : public std::runtime_error { public: image_unsupported_bit_depth(); };



/**
 * \class mixer_backend_init_failure
 *
 * Thrown when a derived Mixer type is unable to initialize itself.
 */
class mixer_backend_init_failure : public std::runtime_error { public: mixer_backend_init_failure(const std::string& description); mixer_backend_init_failure(); };


/**
* \class renderer_no_glsl
*
* Thrown when the OGL_Renderer determines that no GLSL shading language is available.
*/
class renderer_no_glsl : public std::runtime_error { public: renderer_no_glsl(); };


/**
* \class renderer_backend_init_failure
*
* Thrown when a derived Renderer type is unable to initialize itself.
*/
class renderer_backend_init_failure : public std::runtime_error { public: renderer_backend_init_failure(const std::string& description); renderer_backend_init_failure();};


/**
* \class renderer_window_creation_failure
*
* Thrown when a window context can't be created.
*/
class renderer_window_creation_failure : public std::runtime_error { public: renderer_window_creation_failure(); };


/**
* \class renderer_opengl_context_failure
*
* Thrown when the OGL_Renderer can't create an OpenGL context.
*/
class renderer_opengl_context_failure : public std::runtime_error { public: renderer_opengl_context_failure(); };

} // exception namespace
} // NAS2D namespace
