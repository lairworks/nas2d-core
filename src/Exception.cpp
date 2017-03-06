// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/Exception.h"

#include <string>


using namespace NAS2D;
using namespace NAS2D::exception;

filesystem_already_initialized::filesystem_already_initialized() : runtime_error("Filesystem is already initialized.")
{}


filesystem_backend_init_failure::filesystem_backend_init_failure() : runtime_error("Unable to start virtual filesystem.")
{}


filesystem_backend_init_failure::filesystem_backend_init_failure(const std::string& description) : runtime_error("Unable to start virtual filesystem: " + description)
{}


filesystem_not_initialized::filesystem_not_initialized() : runtime_error("Fileystem is not initialized.")
{}


filesystem_file_handle_still_open::filesystem_file_handle_still_open() : runtime_error("Unable to close file handle.")
{}


filesystem_file_handle_still_open::filesystem_file_handle_still_open(const std::string& description) : runtime_error("Unable to close file handle: " + description)
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


mixer_backend_init_failure::mixer_backend_init_failure() : runtime_error("Failure initializing back end audio library.")
{}


mixer_backend_init_failure::mixer_backend_init_failure(const std::string& description) : runtime_error("Failure initializing back end audio library: " + description)
{}


image_bad_data::image_bad_data() : runtime_error("Image contains invalid data.")
{}


image_null_data::image_null_data() : runtime_error("Image operation performed on a NULL buffer.")
{}


image_unsupported_bit_depth::image_unsupported_bit_depth() : runtime_error("Unsupported bit depth encountered when loading an image. NAS2D only supports 24- and 32-bit images.")
{}
