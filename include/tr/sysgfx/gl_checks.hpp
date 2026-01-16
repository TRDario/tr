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
#include "gl_call.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// GLSL variable types.
	enum class glsl_type {
		undefined,
		bool_ = GL_BOOL,
		bvec2 = GL_BOOL_VEC2,
		bvec3 = GL_BOOL_VEC3,
		bvec4 = GL_BOOL_VEC4,
		int_ = GL_INT,
		ivec2 = GL_INT_VEC2,
		ivec3 = GL_INT_VEC3,
		ivec4 = GL_INT_VEC4,
		uint = GL_UNSIGNED_INT,
		uvec2 = GL_UNSIGNED_INT_VEC2,
		uvec3 = GL_UNSIGNED_INT_VEC3,
		uvec4 = GL_UNSIGNED_INT_VEC4,
		float_ = GL_FLOAT,
		vec2 = GL_FLOAT_VEC2,
		vec3 = GL_FLOAT_VEC3,
		vec4 = GL_FLOAT_VEC4,
		mat2 = GL_FLOAT_MAT2,
		mat3 = GL_FLOAT_MAT3,
		mat4 = GL_FLOAT_MAT4,
		mat2x3 = GL_FLOAT_MAT2x3,
		mat2x4 = GL_FLOAT_MAT2x4,
		mat3x2 = GL_FLOAT_MAT3x2,
		mat3x4 = GL_FLOAT_MAT3x4,
		mat4x2 = GL_FLOAT_MAT4x2,
		mat4x3 = GL_FLOAT_MAT4x3,
		sampler2D = GL_SAMPLER_2D,
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

#include "gl_checks_impl.hpp" // IWYU pragma: keep