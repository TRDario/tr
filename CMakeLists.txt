cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(tr LANGUAGES CXX VERSION 1.0.1)

option(TR_BUILD_SYSGFX "Build the system and graphics module." ON)
option(TR_BUILD_AUDIO "Build the audio module." ON)
option(TR_BUILD_IMGUI "Build the Dear ImGui integration module." OFF)
option(TR_BUILD_TESTS "Build tests" OFF)

if((TR_BUILD_AUDIO OR TR_BUILD_IMGUI) AND NOT TR_BUILD_SYSGFX)
	message("[tr] Setting 'TR_BUILD_SYSGFX' to 'ON' because another module depends on it.")
	set(TR_BUILD_SYSGFX ON CACHE BOOL "Build the system and graphics module." FORCE)
endif()

include(Dependencies.cmake)
include(check_compiler.cmake)
include(target_template.cmake)

add_library(tr STATIC)
add_library(tr::tr ALIAS tr)
tr_target_template(tr)

target_include_directories(tr PUBLIC include)
target_precompile_headers(tr PUBLIC include/tr/utility.hpp)
target_sources(tr PRIVATE
	src/utility/atlas_packer.cpp
	src/utility/benchmark.cpp
	src/utility/binary_io.cpp
	src/utility/encryption.cpp
	src/utility/exception.cpp
	src/utility/geometry.cpp
	src/utility/iostream.cpp
	src/utility/localization_map.cpp
	src/utility/logger.cpp
	src/utility/rng.cpp
	src/utility/stopwatch.cpp
	src/utility/timer.cpp
)
target_link_libraries(tr glm::glm lz4::lz4)
if(NOT TR_HAS_STD_FORMAT)
	target_link_libraries(tr fmt::fmt-header-only)
endif()
if(TR_BUILD_SYSGFX)
	target_sources(tr PRIVATE
		src/sysgfx/gl_call.cpp
		src/sysgfx/glad.cpp
		src/sysgfx/backbuffer.cpp
		src/sysgfx/bitmap_iterators.cpp
		src/sysgfx/bitmap.cpp
		src/sysgfx/cursor.cpp
		src/sysgfx/debug_renderer.cpp
		src/sysgfx/dialog.cpp
		src/sysgfx/display.cpp
		src/sysgfx/event.cpp
		src/sysgfx/graphics_context.cpp
		src/sysgfx/index_buffer.cpp
		src/sysgfx/initialization.cpp
		src/sysgfx/keyboard.cpp
		src/sysgfx/mouse.cpp
		src/sysgfx/path.cpp
		src/sysgfx/render_target.cpp
		src/sysgfx/renderer_2d.cpp
		src/sysgfx/shader_buffer.cpp
		src/sysgfx/shader_pipeline.cpp
		src/sysgfx/shader.cpp
		src/sysgfx/state_manager.cpp
		src/sysgfx/texture_unit.cpp
		src/sysgfx/texture.cpp
		src/sysgfx/vertex_buffer.cpp
		src/sysgfx/vertex_format.cpp
		src/sysgfx/ttfont.cpp
		src/sysgfx/window.cpp
	)
	target_link_libraries(tr SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image)
endif()
if(TR_BUILD_AUDIO)
	target_sources(tr PRIVATE
		src/audio/al_call.cpp
		src/audio/buffer.cpp
		src/audio/source.cpp
		src/audio/stream.cpp
		src/audio/system.cpp
		src/audio/impl.cpp
	)
	target_compile_definitions(tr PUBLIC AL_LIBTYPE_STATIC)
	target_link_libraries(tr OpenAL::OpenAL vorbis vorbisenc vorbisfile)
endif()
if(TR_BUILD_IMGUI)
	target_sources(tr PRIVATE
		src/imgui.cpp
	)
endif()
if (TR_BUILD_TESTS)
	add_subdirectory(test)
endif()