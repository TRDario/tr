include(FetchContent)

set(LZ4_BUILD_CLI OFF)

if(TR_BUILD_SYSGFX)
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
endif()

if(TR_BUILD_AUDIO)
	set(ALSOFT_EXAMPLES OFF)
	set(ALSOFT_UTILS OFF)
	set(ALSOFT_NO_CONFIG_UTIL ON)
	set(ALSOFT_INSTALL OFF)
	set(ALSOFT_INSTALL_CONFIG OFF)
	set(ALSOFT_INSTALL_HRTF_DATA OFF)
	set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF)
endif()

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG			55f22ef736ba6783e95351bc71a40a48ba305afc # 1.0.2
)
FetchContent_Declare(
	lz4
	GIT_REPOSITORY	https://github.com/lz4/lz4.git
	GIT_TAG			v1.10.0
	SOURCE_SUBDIR   build/cmake
)
FetchContent_MakeAvailable(glm lz4)
add_library(lz4::lz4 ALIAS lz4_static)

if(TR_BUILD_SYSGFX)
FetchContent_Declare(
	sdl3
		GIT_REPOSITORY	https://github.com/libsdl-org/SDL.git
		GIT_TAG			release-3.2.16
		OVERRIDE_FIND_PACKAGE 
	)
	FetchContent_Declare(
		sdl3_ttf
		GIT_REPOSITORY	https://github.com/libsdl-org/SDL_ttf.git
		GIT_TAG			release-3.2.2
	)
	FetchContent_Declare(
		sdl3_image
		GIT_REPOSITORY	https://github.com/libsdl-org/SDL_image.git
		GIT_TAG			release-3.2.4
	)
	FetchContent_MakeAvailable(sdl3 sdl3_ttf sdl3_image)
endif()

if(TR_BUILD_AUDIO)
	FetchContent_Declare(
		ogg
		GIT_REPOSITORY	https://github.com/xiph/ogg.git
		GIT_TAG			v1.3.6
	)
	set(OGG_INCLUDE_DIR ${ogg_SOURCE_DIR}/include)
	set(OGG_LIBRARY ${ogg_BINARY_DIR})
	FetchContent_Declare(
		vorbis
		GIT_REPOSITORY	https://github.com/xiph/vorbis.git
		GIT_TAG			v1.3.7
	)
	FetchContent_Declare(
		openal-soft
		GIT_REPOSITORY	https://github.com/kcat/openal-soft.git
		GIT_TAG			1.24.3
	)
	FetchContent_MakeAvailable(ogg vorbis openal-soft)
endif()