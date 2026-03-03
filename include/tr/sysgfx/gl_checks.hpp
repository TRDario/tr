///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides datatypes used for additional OpenGL checks.                                                                                 //
//                                                                                                                                       //
// tr::gfx::glsl_type is an enumerator of GLSL variable types, while tr::gfx::glsl_variable is a structure containing information about  //
// a GLSL variable. C++ types can be mapped to values of tr::gfx::glsl_type using tr::gfx::as_glsl_type:                                 //
//     - tr::gfx::as_glsl_type<glm::mat4> -> tr::gfx::glsl_type::mat4                                                                    //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// GLSL variable types.
	enum class glsl_type {
		undefined,
		bool_ = 0x8B56,
		bvec2,
		bvec3,
		bvec4,
		int_ = 0x1404,
		ivec2 = 0x8B53,
		ivec3,
		ivec4,
		uint = 0x1405,
		uvec2 = 0x8DC6,
		uvec3,
		uvec4,
		float_ = 0x1406,
		vec2 = 0x8B50,
		vec3,
		vec4,
		mat2 = 0x8B5A,
		mat3,
		mat4,
		mat2x3 = 0x8B65,
		mat2x4,
		mat3x2,
		mat3x4,
		mat4x2,
		mat4x3,
		sampler2D = 0x8B5E
	};

	// GLSL variable information.
	struct glsl_variable {
		// The name of the variable.
		std::string name;
		// The type of the variable.
		glsl_type type;
		// The array size of the variable, or 1 if not an array.
		int array_size;
	};

	// Maps a C++ type to a glsl_type enum value.
	template <class T> inline constexpr glsl_type as_glsl_type{glsl_type::undefined};
}; // namespace tr::gfx

// GLSL type formatter.
template <> struct TR_FMT::formatter<tr::gfx::glsl_type> : protected TR_FMT::formatter<const char*> {
	using TR_FMT::formatter<const char*>::parse;
	// Formats a GLSL type value.
	template <typename FormatContext> constexpr auto format(tr::gfx::glsl_type t, FormatContext& ctx) const;
};

// GLSL variable information formatter.
template <>
struct TR_FMT::formatter<tr::gfx::glsl_variable> : private TR_FMT::formatter<tr::gfx::glsl_type>, private TR_FMT::formatter<int> {
	using TR_FMT::formatter<const char*>::parse;
	// Formats GLSL variable information.
	template <typename FormatContext> constexpr auto format(const tr::gfx::glsl_variable& v, FormatContext& ctx) const;
};

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "gl_checks_impl.hpp" // IWYU pragma: export