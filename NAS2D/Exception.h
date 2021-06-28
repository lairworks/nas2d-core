// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include <stdexcept>
#include <string>

namespace NAS2D {

	namespace Exception {

		/**
		 * \page Exceptions Exceptions
		 *
		 * See <a href="https://github.com/lairworks/nas2d-core/wiki/Exceptions-&-Exception-Handling-in-NAS2D">Wiki Page (Exceptions)</a> for further details.
		 */


		/**
		 * Thrown when a derived Mixer type is unable to initialize itself.
		 */
		class mixer_backend_init_failure : public std::runtime_error { public: mixer_backend_init_failure(const std::string& description); mixer_backend_init_failure(); };


		/**
		 * Thrown when the RendererOpenGL determines that no GLSL shading language is available.
		 */
		class renderer_no_glsl : public std::runtime_error { public: renderer_no_glsl(); };


		/**
		 * Thrown when a derived Renderer type is unable to initialize itself.
		 */
		class renderer_backend_init_failure : public std::runtime_error { public: renderer_backend_init_failure(const std::string& description); renderer_backend_init_failure();};


		/**
		 * Thrown when a window context can't be created.
		 */
		class renderer_window_creation_failure : public std::runtime_error { public: renderer_window_creation_failure(); };


		/**
		 * Thrown when the RendererOpenGL can't create an OpenGL context.
		 */
		class renderer_opengl_context_failure : public std::runtime_error { public: renderer_opengl_context_failure(); };

	} // exception namespace
} // NAS2D namespace
