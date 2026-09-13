/// @file
/// @brief Includes all headers from the system and graphics module.
/// @details
/// The system and graphics module provides a system and graphics API based on SDL3 and OpenGL.
///
/// Using this module requires the option TR_BUILD_SYSGFX to be turned on in CMake.
///
/// Most classes and functions in this module cannot be used outside of the callbacks run by `tr::run_main_loops()`. Exceptions to this
/// include the classes and functions in application, dialog.hpp , logger.hpp, and path.hpp.

#pragma once
#include "sysgfx/application.hpp"         // IWYU pragma: export
#include "sysgfx/atlas.hpp"               // IWYU pragma: export
#include "sysgfx/bitmap.hpp"              // IWYU pragma: export
#include "sysgfx/bitmap_iterators.hpp"    // IWYU pragma: export
#include "sysgfx/blending.hpp"            // IWYU pragma: export
#include "sysgfx/clipboard.hpp"           // IWYU pragma: export
#include "sysgfx/cursor.hpp"              // IWYU pragma: export
#include "sysgfx/dialog.hpp"              // IWYU pragma: export
#include "sysgfx/display.hpp"             // IWYU pragma: export
#include "sysgfx/event.hpp"               // IWYU pragma: export
#include "sysgfx/graphics_benchmark.hpp"  // IWYU pragma: export
#include "sysgfx/graphics_buffer.hpp"     // IWYU pragma: export
#include "sysgfx/graphics_buffer_map.hpp" // IWYU pragma: export
#include "sysgfx/graphics_context.hpp"    // IWYU pragma: export
#include "sysgfx/index_buffer.hpp"        // IWYU pragma: export
#include "sysgfx/keyboard.hpp"            // IWYU pragma: export
#include "sysgfx/mouse.hpp"               // IWYU pragma: export
#include "sysgfx/path.hpp"                // IWYU pragma: export
#include "sysgfx/render_target.hpp"       // IWYU pragma: export
#include "sysgfx/shader.hpp"              // IWYU pragma: export
#include "sysgfx/shader_buffer.hpp"       // IWYU pragma: export
#include "sysgfx/shader_pipeline.hpp"     // IWYU pragma: export
#include "sysgfx/texture.hpp"             // IWYU pragma: export
#include "sysgfx/texture_target.hpp"      // IWYU pragma: export
#include "sysgfx/texture_view.hpp"        // IWYU pragma: export
#include "sysgfx/ttfont.hpp"              // IWYU pragma: export
#include "sysgfx/uniform_buffer.hpp"      // IWYU pragma: export
#include "sysgfx/vertex_buffer.hpp"       // IWYU pragma: export
#include "sysgfx/vertex_format.hpp"       // IWYU pragma: export
#include "sysgfx/window.hpp"              // IWYU pragma: export
#include "sysgfx/window_view.hpp"         // IWYU pragma: export
#include <tr/sysgfx/log_level.hpp>
#include <tr/sysgfx/log_message.hpp>
#include <tr/sysgfx/log_sink.hpp>
#include <tr/sysgfx/logger.hpp>