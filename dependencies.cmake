###########################################################################################################################################
##                                                                                                                                       ##
## Automatically handles tr's dependencies.                                                                                              ##
##                                                                                                                                       ##
## If TR_USE_SYSTEM_LIBRARIES evaluates to true, dependencies are taken from the user's system; otherwise download and build them.       ##
##                                                                                                                                       ##
## If TR_HAS_STD_FORMAT ends up not evaluating to true, fmtlib is fetched as a fallback.                                                 ##
##                                                                                                                                       ##
###########################################################################################################################################

include(FetchContent)
include(CheckIncludeFileCXX)

if(TR_USE_SYSTEM_LIBRARIES)
	message("-- tr: Using system dependencies")
	find_package(PkgConfig REQUIRED)
else()
	message("-- tr: Using fetched dependencies")
endif()

################################################################# UTILITY #################################################################

if(TR_USE_SYSTEM_LIBRARIES)
	find_package(glm REQUIRED)
	pkg_check_modules(lz4 REQUIRED IMPORTED_TARGET liblz4)
	add_library(lz4::lz4 ALIAS PkgConfig::lz4)
else()
	set(LZ4_BUILD_CLI OFF)

	FetchContent_Declare(
		glm
		URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
		DOWNLOAD_EXTRACT_TIMESTAMP true
	)
	FetchContent_Declare(
		lz4
		URL https://github.com/lz4/lz4/archive/refs/tags/v1.10.0.zip
		SOURCE_SUBDIR build/cmake
		DOWNLOAD_EXTRACT_TIMESTAMP true
	)
	FetchContent_MakeAvailable(glm lz4)
	add_library(lz4::lz4 ALIAS lz4_static)
endif()

check_include_file_cxx(format TR_HAS_STD_FORMAT)
if(NOT TR_HAS_STD_FORMAT)
	if(TR_USE_SYSTEM_LIBRARIES)
		find_package(fmt REQUIRED)
	else()
		FetchContent_Declare(
			fmt
			URL https://github.com/fmtlib/fmt/releases/download/12.1.0/fmt-12.1.0.zip
			DOWNLOAD_EXTRACT_TIMESTAMP true
		)
		FetchContent_MakeAvailable(fmt)
	endif()
endif()

################################################################## SYSGFX #################################################################

if(TR_BUILD_SYSGFX)
	if(TR_USE_SYSTEM_LIBRARIES)
		find_package(SDL3 REQUIRED)
		find_package(SDL3_ttf REQUIRED)
		find_package(SDL3_image REQUIRED)
	else()
		set(BUILD_SHARED_LIBS OFF)
		set(SDL_STATIC ON)
		set(SDL_SHARED OFF)
		set(SDL_DISABLE_INSTALL ON)
		set(SDL_TEST_LIBRARY OFF)
		set(SDL_AUDIO OFF)
		set(SDL_HAPTIC OFF)
		set(SDL_JOYSTICK OFF)
		set(SDL_SENSOR OFF)
		set(SDL_CAMERA OFF)
		set(SDL_RENDER OFF)
		set(SDL_POWER OFF)
		set(SDL_HIDAPI OFF)
		set(SDL_GPU OFF)
		set(SDL_OPENGLES OFF)
		set(SDL_VULKAN OFF)
		set(SDLTTF_INSTALL OFF)
		set(SDLTTF_VENDORED ON)
		set(SDLIMAGE_INSTALL OFF)
		set(SDLIMAGE_VENDORED ON)
		set(SDLIMAGE_AVIF OFF)
		set(SDLIMAGE_GIF OFF)
		set(SDLIMAGE_JPG OFF)
		set(SDLIMAGE_JXL OFF)
		set(SDLIMAGE_LBM OFF)
		set(SDLIMAGE_PCX OFF)
		set(SDLIMAGE_PNM OFF)
		set(SDLIMAGE_SVG OFF)
		set(SDLIMAGE_TGA OFF)
		set(SDLIMAGE_TIF OFF)
		set(SDLIMAGE_WEBP OFF)
		set(SDLIMAGE_XCF OFF)
		set(SDLIMAGE_XPM OFF)
		set(SDLIMAGE_XV OFF)
		set(SDLIMAGE_PNG ON)
		set(SDLIMAGE_QOI ON)

		FetchContent_Declare(
			sdl3
			URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.4.0.zip
			OVERRIDE_FIND_PACKAGE
			DOWNLOAD_EXTRACT_TIMESTAMP true
		)

		FetchContent_Declare(
			sdl3_ttf
			GIT_REPOSITORY https://github.com/libsdl-org/sdl_ttf.git
			GIT_TAG release-3.2.2
			GIT_SHALLOW true
		)

		FetchContent_Declare(
			sdl3_image
			URL https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-3.2.6.zip
			DOWNLOAD_EXTRACT_TIMESTAMP true
		)

		FetchContent_MakeAvailable(sdl3_ttf sdl3_image sdl3)
	endif()
endif()

################################################################## AUDIO ##################################################################

if(TR_BUILD_AUDIO)
	if(TR_USE_SYSTEM_LIBRARIES)
		find_package(Ogg REQUIRED)
		pkg_check_modules(PC_Vorbis_Vorbis REQUIRED vorbis)
		pkg_check_modules(PC_Vorbis_Enc REQUIRED vorbisenc)
		pkg_check_modules(PC_Vorbis_File REQUIRED vorbisfile)
		find_package(OpenAL REQUIRED)
	else()
		set(ALSOFT_EXAMPLES OFF)
		set(ALSOFT_UTILS OFF)
		set(ALSOFT_NO_CONFIG_UTIL ON)
		set(ALSOFT_INSTALL OFF)
		set(ALSOFT_INSTALL_CONFIG OFF)
		set(ALSOFT_INSTALL_HRTF_DATA OFF)
		set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF)
		set(LIBTYPE "STATIC")

		FetchContent_Declare(
			ogg
			URL https://github.com/xiph/ogg/archive/refs/tags/v1.3.6.zip
			DOWNLOAD_EXTRACT_TIMESTAMP true
		)
		FetchContent_MakeAvailable(ogg)
		set(OGG_INCLUDE_DIR ${ogg_SOURCE_DIR}/include)
		set(OGG_LIBRARY ${ogg_SOURCE_DIR})

		FetchContent_Declare(
			vorbis
			URL https://github.com/xiph/vorbis/archive/refs/tags/v1.3.7.zip
			DOWNLOAD_EXTRACT_TIMESTAMP true
		)

		FetchContent_Declare(
			openal-soft
			URL https://github.com/kcat/openal-soft/archive/refs/tags/1.25.0.zip
			DOWNLOAD_EXTRACT_TIMESTAMP true
		)

		FetchContent_MakeAvailable(openal-soft vorbis)
	endif()
endif()

################################################################## IMGUI ##################################################################

if(TR_BUILD_IMGUI)
	FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG v1.92.5
	)
	FetchContent_MakeAvailable(imgui)

	add_library(imgui STATIC
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp 
		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp
		${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
	)
	target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
	target_link_libraries(imgui PRIVATE SDL3::SDL3)
endif()