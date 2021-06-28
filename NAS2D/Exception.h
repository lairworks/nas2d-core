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

#include <stdexcept>
#include <string>

namespace NAS2D {

namespace Exception {

class filesystem_backend_init_failure : public std::runtime_error { public: filesystem_backend_init_failure(const std::string& description); filesystem_backend_init_failure(); };
class filesystem_already_initialized : public std::runtime_error { public: filesystem_already_initialized(); };
class filesystem_file_handle_still_open : public std::runtime_error { public: filesystem_file_handle_still_open(const std::string& description); filesystem_file_handle_still_open();};
class font_bad_data : public std::runtime_error { public: font_bad_data(); };
class font_invalid_glyph_map : public std::runtime_error { public: font_invalid_glyph_map(); font_invalid_glyph_map(const std::string& description); };
class image_bad_copy : public std::runtime_error { public: image_bad_copy(); };
class image_bad_data : public std::runtime_error { public: image_bad_data(); };
class image_null_data : public std::runtime_error { public: image_null_data(); };
class image_unsupported_bit_depth : public std::runtime_error { public: image_unsupported_bit_depth(); };
class mixer_backend_init_failure : public std::runtime_error { public: mixer_backend_init_failure(const std::string& description); mixer_backend_init_failure(); };
class renderer_no_glsl : public std::runtime_error { public: renderer_no_glsl(); };
class renderer_backend_init_failure : public std::runtime_error { public: renderer_backend_init_failure(const std::string& description); renderer_backend_init_failure();};
class renderer_window_creation_failure : public std::runtime_error { public: renderer_window_creation_failure(); };
class renderer_opengl_context_failure : public std::runtime_error { public: renderer_opengl_context_failure(); };

} // exception namespace
} // NAS2D namespace
