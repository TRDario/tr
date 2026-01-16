///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements gl_checks.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "gl_checks.hpp"

////////////////////////////////////////////////////// AS GLSL TYPE SPECIALIZATIONS ///////////////////////////////////////////////////////

template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<bool>{glsl_type::bool_};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::bvec2>{glsl_type::bvec2};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::bvec3>{glsl_type::bvec3};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::bvec4>{glsl_type::bvec4};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<int>{glsl_type::int_};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::ivec2>{glsl_type::ivec2};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::ivec3>{glsl_type::ivec3};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::ivec4>{glsl_type::ivec4};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<unsigned int>{glsl_type::uint};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::uvec2>{glsl_type::uvec2};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::uvec3>{glsl_type::uvec3};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::uvec4>{glsl_type::uvec4};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<float>{glsl_type::float_};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::vec2>{glsl_type::vec2};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::vec3>{glsl_type::vec3};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::vec4>{glsl_type::vec4};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat2>{glsl_type::mat2};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat3>{glsl_type::mat3};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat4>{glsl_type::mat4};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat2x3>{glsl_type::mat2x3};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat2x4>{glsl_type::mat2x4};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat3x2>{glsl_type::mat3x2};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat3x4>{glsl_type::mat3x4};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat4x2>{glsl_type::mat4x2};
template <> inline constexpr tr::gfx::glsl_type tr::gfx::as_glsl_type<glm::mat4x3>{glsl_type::mat4x3};

//////////////////////////////////////////////////////////////// FORMATTERS ///////////////////////////////////////////////////////////////

template <typename FormatContext>
constexpr auto TR_FMT::formatter<tr::gfx::glsl_type>::format(tr::gfx::glsl_type t, FormatContext& ctx) const
{
	const char* str;
	switch (t) {
	case tr::gfx::glsl_type::bool_:
		str = "bool";
		break;
	case tr::gfx::glsl_type::bvec2:
		str = "bvec2";
		break;
	case tr::gfx::glsl_type::bvec3:
		str = "bvec3";
		break;
	case tr::gfx::glsl_type::bvec4:
		str = "bvec4";
		break;
	case tr::gfx::glsl_type::int_:
		str = "int";
		break;
	case tr::gfx::glsl_type::ivec2:
		str = "ivec2";
		break;
	case tr::gfx::glsl_type::ivec3:
		str = "ivec3";
		break;
	case tr::gfx::glsl_type::ivec4:
		str = "ivec4";
		break;
	case tr::gfx::glsl_type::uint:
		str = "unsigned int";
		break;
	case tr::gfx::glsl_type::uvec2:
		str = "uvec2";
		break;
	case tr::gfx::glsl_type::uvec3:
		str = "uvec3";
		break;
	case tr::gfx::glsl_type::uvec4:
		str = "uvec4";
		break;
	case tr::gfx::glsl_type::float_:
		str = "float";
		break;
	case tr::gfx::glsl_type::vec2:
		str = "vec2";
		break;
	case tr::gfx::glsl_type::vec3:
		str = "vec3";
		break;
	case tr::gfx::glsl_type::vec4:
		str = "vec4";
		break;
	case tr::gfx::glsl_type::mat2:
		str = "mat2";
		break;
	case tr::gfx::glsl_type::mat3:
		str = "mat3";
		break;
	case tr::gfx::glsl_type::mat4:
		str = "mat4";
		break;
	case tr::gfx::glsl_type::mat2x3:
		str = "mat2x3";
		break;
	case tr::gfx::glsl_type::mat2x4:
		str = "mat2x4";
		break;
	case tr::gfx::glsl_type::mat3x2:
		str = "mat3x2";
		break;
	case tr::gfx::glsl_type::mat3x4:
		str = "mat3x4";
		break;
	case tr::gfx::glsl_type::mat4x2:
		str = "mat4x2";
		break;
	case tr::gfx::glsl_type::mat4x3:
		str = "mat4x3";
		break;
	case tr::gfx::glsl_type::sampler2D:
		str = "sampler2D";
		break;
	default:
		str = "<unknown>";
		break;
	};
	ctx.advance_to(TR_FMT::formatter<const char*>::format(str, ctx));
	return ctx.out();
}

template <typename FormatContext>
constexpr auto TR_FMT::formatter<tr::gfx::glsl_variable>::format(const tr::gfx::glsl_variable& v, FormatContext& ctx) const
{
	ctx.advance_to(TR_FMT::formatter<tr::gfx::glsl_type>::format(v.type, ctx));
	if (v.array_size > 1) {
		++*ctx.out() = '[';
		ctx.advance_to(TR_FMT::formatter<int>::format(v.array_size, ctx));
		++*ctx.out() = ']';
	}
	++*ctx.out() = ' ';
	ctx.advance_to(TR_FMT::formatter<const char*>::format(v.name.c_str(), ctx));
	return ctx.out();
}