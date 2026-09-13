/// @file
/// @brief Includes all headers from the system and graphics module.
/// @details
/// The system and graphics module provides a system and graphics API based on SDL3 and OpenGL.
///
/// Using this module requires the option TR_BUILD_SYSGFX to be turned on in CMake.
///
/// Most classes and functions in this module cannot be used outside of the callbacks run by `tr::run_main_loops()`. Exceptions to this
/// include the classes and functions in application, dialog.hpp, logger.hpp, and path.hpp.

#pragma once
#include <tr/sysgfx/application.hpp>                         // IWYU pragma: export
#include <tr/sysgfx/bitmap.hpp>                              // IWYU pragma: export
#include <tr/sysgfx/bitmap_atlas.hpp>                        // IWYU pragma: export
#include <tr/sysgfx/bitmap_view.hpp>                         // IWYU pragma: export
#include <tr/sysgfx/blending.hpp>                            // IWYU pragma: export
#include <tr/sysgfx/clipboard.hpp>                           // IWYU pragma: export
#include <tr/sysgfx/cursor.hpp>                              // IWYU pragma: export
#include <tr/sysgfx/dialog.hpp>                              // IWYU pragma: export
#include <tr/sysgfx/display.hpp>                             // IWYU pragma: export
#include <tr/sysgfx/dynamic_atlas.hpp>                       // IWYU pragma: export
#include <tr/sysgfx/dynamic_index_buffer.hpp>                // IWYU pragma: export
#include <tr/sysgfx/dynamic_vertex_buffer.hpp>               // IWYU pragma: export
#include <tr/sysgfx/event.hpp>                               // IWYU pragma: export
#include <tr/sysgfx/framebuffer.hpp>                         // IWYU pragma: export
#include <tr/sysgfx/graphics_benchmark.hpp>                  // IWYU pragma: export
#include <tr/sysgfx/graphics_buffer.hpp>                     // IWYU pragma: export
#include <tr/sysgfx/graphics_buffer_map_access.hpp>          // IWYU pragma: export
#include <tr/sysgfx/graphics_context.hpp>                    // IWYU pragma: export
#include <tr/sysgfx/keyboard.hpp>                            // IWYU pragma: export
#include <tr/sysgfx/log_level.hpp>                           // IWYU pragma: export
#include <tr/sysgfx/log_message.hpp>                         // IWYU pragma: export
#include <tr/sysgfx/log_sink.hpp>                            // IWYU pragma: export
#include <tr/sysgfx/logger.hpp>                              // IWYU pragma: export
#include <tr/sysgfx/mapped_graphics_buffer_object.hpp>       // IWYU pragma: export
#include <tr/sysgfx/mapped_graphics_buffer_span.hpp>         // IWYU pragma: export
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp> // IWYU pragma: export
#include <tr/sysgfx/mouse.hpp>                               // IWYU pragma: export
#include <tr/sysgfx/owning_shader_pipeline.hpp>              // IWYU pragma: export
#include <tr/sysgfx/path.hpp>                                // IWYU pragma: export
#include <tr/sysgfx/ping_pong_target.hpp>                    // IWYU pragma: export
#include <tr/sysgfx/pixel_format.hpp>                        // IWYU pragma: export
#include <tr/sysgfx/pixel_iterator.hpp>                      // IWYU pragma: export
#include <tr/sysgfx/pixel_proxy.hpp>                         // IWYU pragma: export
#include <tr/sysgfx/render_target.hpp>                       // IWYU pragma: export
#include <tr/sysgfx/shader.hpp>                              // IWYU pragma: export
#include <tr/sysgfx/shader_array.hpp>                        // IWYU pragma: export
#include <tr/sysgfx/shader_buffer.hpp>                       // IWYU pragma: export
#include <tr/sysgfx/shader_pipeline.hpp>                     // IWYU pragma: export
#include <tr/sysgfx/static_index_buffer.hpp>                 // IWYU pragma: export
#include <tr/sysgfx/static_vertex_buffer.hpp>                // IWYU pragma: export
#include <tr/sysgfx/sub_bitmap.hpp>                          // IWYU pragma: export
#include <tr/sysgfx/texture.hpp>                             // IWYU pragma: export
#include <tr/sysgfx/texture_target.hpp>                      // IWYU pragma: export
#include <tr/sysgfx/texture_view.hpp>                        // IWYU pragma: export
#include <tr/sysgfx/ttfont.hpp>                              // IWYU pragma: export
#include <tr/sysgfx/uniform_buffer.hpp>                      // IWYU pragma: export
#include <tr/sysgfx/untyped_dynamic_vertex_buffer.hpp>       // IWYU pragma: export
#include <tr/sysgfx/untyped_shader_buffer.hpp>               // IWYU pragma: export
#include <tr/sysgfx/untyped_static_vertex_buffer.hpp>        // IWYU pragma: export
#include <tr/sysgfx/untyped_uniform_buffer.hpp>              // IWYU pragma: export
#include <tr/sysgfx/vertex_attribute.hpp>                    // IWYU pragma: export
#include <tr/sysgfx/vertex_binding.hpp>                      // IWYU pragma: export
#include <tr/sysgfx/vertex_format.hpp>                       // IWYU pragma: export
#include <tr/sysgfx/window.hpp>                              // IWYU pragma: export
#include <tr/sysgfx/window_view.hpp>                         // IWYU pragma: export