// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/Exception.h"

#include <string>


using namespace NAS2D;
using namespace NAS2D::Exception;

filesystem_already_initialized::filesystem_already_initialized() : runtime_error("Filesystem is already initialized.")
{}


filesystem_backend_init_failure::filesystem_backend_init_failure() : runtime_error("Unable to start virtual filesystem.")
{}


filesystem_backend_init_failure::filesystem_backend_init_failure(const std::string& description) : runtime_error("Unable to start virtual filesystem: " + description)
{}


filesystem_file_handle_still_open::filesystem_file_handle_still_open() : runtime_error("Unable to close file handle.")
{}


filesystem_file_handle_still_open::filesystem_file_handle_still_open(const std::string& description) : runtime_error("Unable to close file handle: " + description)
{}


font_bad_data::font_bad_data() : runtime_error("Font contains invalid data.")
{}


font_invalid_glyph_map::font_invalid_glyph_map() : runtime_error("Bitmap font specified but image doesn't conform to expected parameters.")
{}


font_invalid_glyph_map::font_invalid_glyph_map(const std::string& description) : runtime_error("Bitmap font specified but image doesn't conform to expected parameters: " + description)
{}


image_bad_copy::image_bad_copy() : runtime_error("Attempted to copy an Image in an invalid load state.")
{}


image_bad_data::image_bad_data() : runtime_error("Image contains invalid data.")
{}


image_null_data::image_null_data() : runtime_error("Image operation performed on a NULL buffer.")
{}


image_unsupported_bit_depth::image_unsupported_bit_depth() : runtime_error("Unsupported bit depth encountered when loading an image. NAS2D only supports 24- and 32-bit images.")
{}


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
