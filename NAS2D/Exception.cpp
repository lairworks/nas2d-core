// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "Exception.h"

#include <string>


using namespace NAS2D;
using namespace NAS2D::Exception;


mixer_backend_init_failure::mixer_backend_init_failure() : runtime_error("Failure initializing back end audio library.")
{}


mixer_backend_init_failure::mixer_backend_init_failure(const std::string& description) : runtime_error("Failure initializing back end audio library: " + description)
{}


renderer_no_glsl::renderer_no_glsl() : runtime_error("Graphics driver reports no GLSL support.")
{}


renderer_backend_init_failure::renderer_backend_init_failure(const std::string& description) : runtime_error("Failure initializing back end video library: " + description)
{}


renderer_backend_init_failure::renderer_backend_init_failure() : runtime_error("Failure initializing back end video library.")
{}


renderer_window_creation_failure::renderer_window_creation_failure() : runtime_error("Unable to create a Window.")
{}


renderer_opengl_context_failure::renderer_opengl_context_failure() : runtime_error("Unable to create an OpenGL Context.")
{}
