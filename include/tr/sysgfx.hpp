///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a system and graphics API based on SDL3 and OpenGL.                                                                          //
//                                                                                                                                       //
// Using this module requires the option TR_BUILD_SYSGFX to be turned on in CMake.                                                       //
//                                                                                                                                       //
// When using this module, the standard main entry point is replaced with several custom functions that must be defined, see main.hpp.   //
//                                                                                                                                       //
// Most classes and functions in the tr::sys namespace cannot be used in main::parse_command_line. Exceptions to this include the        //
// functions in dialog.hpp, main.hpp, and path.hpp.                                                                                      //
//                                                                                                                                       //
// Classes and functions in the tr::gfx namespace cannot be used while no window is open, and any objects not persist between window     //
// closings/openings.                                                                                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "sysgfx/atlas.hpp"            // IWYU pragma: export
#include "sysgfx/backbuffer.hpp"       // IWYU pragma: export
#include "sysgfx/basic_renderer.hpp"   // IWYU pragma: export
#include "sysgfx/bitmap.hpp"           // IWYU pragma: export
#include "sysgfx/bitmap_iterators.hpp" // IWYU pragma: export
#include "sysgfx/blending.hpp"         // IWYU pragma: export
#include "sysgfx/buffer_map.hpp"       // IWYU pragma: export
#include "sysgfx/circle_renderer.hpp"  // IWYU pragma: export
#include "sysgfx/cursor.hpp"           // IWYU pragma: export
#include "sysgfx/debug_renderer.hpp"   // IWYU pragma: export
#include "sysgfx/dialog.hpp"           // IWYU pragma: export
#include "sysgfx/display.hpp"          // IWYU pragma: export
#include "sysgfx/event.hpp"            // IWYU pragma: export
#include "sysgfx/gpu_benchmark.hpp"    // IWYU pragma: export
#include "sysgfx/graphics_context.hpp" // IWYU pragma: export
#include "sysgfx/index_buffer.hpp"     // IWYU pragma: export
#include "sysgfx/keyboard.hpp"         // IWYU pragma: export
#include "sysgfx/layered_drawing.hpp"  // IWYU pragma: export
#include "sysgfx/main.hpp"             // IWYU pragma: export
#include "sysgfx/mouse.hpp"            // IWYU pragma: export
#include "sysgfx/path.hpp"             // IWYU pragma: export
#include "sysgfx/render_target.hpp"    // IWYU pragma: export
#include "sysgfx/shader.hpp"           // IWYU pragma: export
#include "sysgfx/shader_buffer.hpp"    // IWYU pragma: export
#include "sysgfx/shader_pipeline.hpp"  // IWYU pragma: export
#include "sysgfx/state_machine.hpp"    // IWYU pragma: export
#include "sysgfx/texture.hpp"          // IWYU pragma: export
#include "sysgfx/ttfont.hpp"           // IWYU pragma: export
#include "sysgfx/uniform_buffer.hpp"   // IWYU pragma: export
#include "sysgfx/vertex_buffer.hpp"    // IWYU pragma: export
#include "sysgfx/vertex_format.hpp"    // IWYU pragma: export
#include "sysgfx/window.hpp"           // IWYU pragma: export