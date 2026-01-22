###########################################################################################################################################
##                                                                                                                                       ##
## Primary CMake file for tr.                                                                                                            ##
##                                                                                                                                       ##
## tr is divided into several modules.                                                                                                   ##
## The 'utility' module is always included and cannot be disabled.                                                                       ##
## The system and graphics module is enabled by default, but can be disabled by setting TR_BUILD_SYSGFX to OFF. If the system and        ##
## graphics module is enabled, TR_HAS_SYSGFX will be defined as a macro.                                                                 ##
## The audio module is enabled by default, but can be disabled by setting TR_BUILD_AUDIO to OFF. The audio module depends on the sysgfx  ##
## module and will force-enable it if disabled. If the audio module is enabled, TR_HAS_AUDIO will be defined as a macro.                 ##
## The DearImGui module is disabled by default, but can be enabled by setting TR_BUILD_IMGUI to ON. The imgui module depends on the      ##
## sysgfx module and will force-enable it if disabled. If the imgui module is enabled, TR_HAS_IMGUI will be defined as a macro.          ##
##                                                                                                                                       ##
## The default behavior with respect to dependencies is to fetch them via FetchContent. This can be overriden by setting                 ##
## TR_USE_SYSTEM_LIBRARIES to ON, which will try to find all dependencies except Dear ImGui through find_package or pkg_check_modules.   ##
## This is primarily meant to allow for quicker iteration during development.                                                            ##
##                                                                                                                                       ##
## If the standard library doesn't support std::format, but does support the rest of C++20, tr will try to substitute it with fmtlib.    ##
##                                                                                                                                       ##
## tr uses tr_target_template for compilation specifics, check target_template.cmake for more information.                               ##
##                                                                                                                                       ##
###########################################################################################################################################

cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)
project(tr LANGUAGES CXX VERSION 2.0.0)

################################################################# OPTIONS #################################################################

option(TR_BUILD_SYSGFX "Build the system and graphics module." ON)
option(TR_BUILD_AUDIO "Build the audio module." ON)
option(TR_BUILD_IMGUI "Build the Dear ImGui integration module." OFF)
option(TR_BUILD_TESTS "Build tests." OFF)
option(TR_USE_SYSTEM_LIBRARIES "Use system packages for dependencies instead of downloading them." OFF)

if((TR_BUILD_AUDIO OR TR_BUILD_IMGUI) AND NOT TR_BUILD_SYSGFX)
	message("-- tr: Setting 'TR_BUILD_SYSGFX' to 'ON' because another module depends on it.")
	set(TR_BUILD_SYSGFX ON CACHE BOOL "Build the system and graphics module." FORCE)
endif()

################################################################# INCLUDES ################################################################

include(dependencies.cmake)
include(defines.cmake)
include(generate_embeddable.cmake)
include(target_template.cmake)

################################################################# UTILITY #################################################################

add_library(tr STATIC)
add_library(tr::tr ALIAS tr)
tr_target_template(tr)

target_include_directories(tr PUBLIC include)
target_precompile_headers(tr PUBLIC include/tr/utility.hpp)
target_sources(tr PRIVATE
	src/utility/atlas_packer.cpp
	src/utility/benchmark.cpp
	src/utility/binary_io.cpp
	src/utility/chrono.cpp
	src/utility/cstring_view.cpp
	src/utility/encryption.cpp
	src/utility/exception.cpp
	src/utility/geometry.cpp
	src/utility/iostream.cpp
	src/utility/localization_map.cpp
	src/utility/logger.cpp
	src/utility/mstream.cpp
	src/utility/rng.cpp
	src/utility/stopwatch.cpp
	src/utility/timer.cpp
)
target_link_libraries(tr glm::glm lz4::lz4)
if(NOT TR_HAS_STD_FORMAT)
	target_link_libraries(tr fmt::fmt)
endif()

################################################################## SYSGFX #################################################################

if(TR_BUILD_SYSGFX)
	message("-- tr: Building system and graphics module")
	tr_generate_embeddable_string(resources/basic_renderer.vert resources/generated/basic_renderer_vert.hpp basic_renderer_vert)
	tr_generate_embeddable_string(resources/basic_renderer.frag resources/generated/basic_renderer_frag.hpp basic_renderer_frag)
	tr_generate_embeddable_string(resources/circle_renderer.vert resources/generated/circle_renderer_vert.hpp circle_renderer_vert)
	tr_generate_embeddable_string(resources/circle_renderer.frag resources/generated/circle_renderer_frag.hpp circle_renderer_frag)
	tr_generate_embeddable_string(resources/debug_renderer.vert resources/generated/debug_renderer_vert.hpp debug_renderer_vert)
	tr_generate_embeddable_string(resources/debug_renderer.frag resources/generated/debug_renderer_frag.hpp debug_renderer_frag)
	tr_generate_embeddable_binary(resources/debug_font.bmp resources/generated/debug_renderer_font.hpp debug_renderer_font)
	target_sources(tr PRIVATE
		src/sysgfx/gl_call.cpp
		src/sysgfx/glad.cpp
		src/sysgfx/backbuffer.cpp
		src/sysgfx/basic_renderer.cpp
		src/sysgfx/basic_renderer_staggered_draw_manager.cpp
		src/sysgfx/bitmap_iterators.cpp
		src/sysgfx/bitmap.cpp
		src/sysgfx/buffer_map.cpp
		src/sysgfx/circle_renderer.cpp
		src/sysgfx/circle_renderer_staggered_draw_manager.cpp
		src/sysgfx/cursor.cpp
		src/sysgfx/debug_renderer.cpp
		src/sysgfx/dialog.cpp
		src/sysgfx/display.cpp
		src/sysgfx/event.cpp
		src/sysgfx/graphics_context.cpp
		src/sysgfx/impl.cpp
		src/sysgfx/gpu_benchmark.cpp
		src/sysgfx/index_buffer.cpp
		src/sysgfx/keyboard.cpp
		src/sysgfx/main.cpp
		src/sysgfx/mouse.cpp
		src/sysgfx/path.cpp
		src/sysgfx/render_target.cpp
		src/sysgfx/shader_buffer.cpp
		src/sysgfx/shader_pipeline.cpp
		src/sysgfx/shader.cpp
		src/sysgfx/state_machine.cpp
		src/sysgfx/texture.cpp
		src/sysgfx/uniform_buffer.cpp
		src/sysgfx/vertex_buffer.cpp
		src/sysgfx/vertex_format.cpp
		src/sysgfx/ttfont.cpp
		src/sysgfx/window.cpp
	)
	target_compile_definitions(tr PUBLIC TR_HAS_SYSGFX)
	target_link_libraries(tr SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)
endif()

################################################################## AUDIO ##################################################################

if(TR_BUILD_AUDIO)
	message("-- tr: Building audio module")
	target_sources(tr PRIVATE
		src/audio/al_call.cpp
		src/audio/buffer.cpp
		src/audio/impl.cpp
		src/audio/listener.cpp
		src/audio/source.cpp
		src/audio/stream.cpp
	)
	target_compile_definitions(tr PUBLIC AL_LIBTYPE_STATIC TR_HAS_AUDIO)
	target_link_libraries(tr OpenAL::OpenAL vorbis vorbisenc vorbisfile)
endif()

################################################################## IMGUI ##################################################################

if(TR_BUILD_IMGUI)
	message("-- tr: Building Dear ImGui module")
	target_sources(tr PRIVATE src/imgui.cpp)
	target_compile_definitions(tr PUBLIC AL_LIBTYPE_STATIC TR_HAS_IMGUI)
	target_link_libraries(tr imgui)
endif()

################################################################## TESTS ##################################################################

if(TR_BUILD_TESTS)
	add_subdirectory(test)
endif()