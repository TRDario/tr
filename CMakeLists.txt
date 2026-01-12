cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(tr LANGUAGES CXX VERSION 1.1.1)

option(TR_BUILD_SYSGFX "Build the system and graphics module." ON)
option(TR_BUILD_AUDIO "Build the audio module." ON)
option(TR_BUILD_IMGUI "Build the Dear ImGui integration module." OFF)
option(TR_BUILD_TESTS "Build tests" OFF)

if((TR_BUILD_AUDIO OR TR_BUILD_IMGUI) AND NOT TR_BUILD_SYSGFX)
	message("[tr] Setting 'TR_BUILD_SYSGFX' to 'ON' because another module depends on it.")
	set(TR_BUILD_SYSGFX ON CACHE BOOL "Build the system and graphics module." FORCE)
endif()

include(dependencies.cmake)
include(defines.cmake)
include(generate_embeddable.cmake)
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

if(TR_BUILD_SYSGFX)
	tr_generate_embeddable_string(resources/basic_renderer.vert resources/generated/BASIC_RENDERER_VERT.hpp BASIC_RENDERER_VERT)
	tr_generate_embeddable_string(resources/basic_renderer.frag resources/generated/BASIC_RENDERER_FRAG.hpp BASIC_RENDERER_FRAG)
	tr_generate_embeddable_string(resources/circle_renderer.vert resources/generated/CIRCLE_RENDERER_VERT.hpp CIRCLE_RENDERER_VERT)
	tr_generate_embeddable_string(resources/circle_renderer.frag resources/generated/CIRCLE_RENDERER_FRAG.hpp CIRCLE_RENDERER_FRAG)
	tr_generate_embeddable_string(resources/debug_renderer.vert resources/generated/DEBUG_RENDERER_VERT.hpp DEBUG_RENDERER_VERT)
	tr_generate_embeddable_string(resources/debug_renderer.frag resources/generated/DEBUG_RENDERER_FRAG.hpp DEBUG_RENDERER_FRAG)
	tr_generate_embeddable_binary(resources/debug_font.bmp resources/generated/DEBUG_RENDERER_FONT.hpp DEBUG_RENDERER_FONT)
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
if(TR_BUILD_AUDIO)
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
if(TR_BUILD_IMGUI)
	target_sources(tr PRIVATE src/imgui.cpp)
	target_link_libraries(tr imgui)
endif()
if(TR_BUILD_TESTS)
	add_subdirectory(test)
endif()