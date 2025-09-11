#pragma once
#include "gl_call.hpp"

namespace tr::gfx {
	// GLSL variable types.
	enum class glsl_type {
		UNDEFINED,
		BOOL = GL_BOOL,
		BVEC2 = GL_BOOL_VEC2,
		BVEC3 = GL_BOOL_VEC3,
		BVEC4 = GL_BOOL_VEC4,
		INT = GL_INT,
		IVEC2 = GL_INT_VEC2,
		IVEC3 = GL_INT_VEC3,
		IVEC4 = GL_INT_VEC4,
		UINT = GL_UNSIGNED_INT,
		UVEC2 = GL_UNSIGNED_INT_VEC2,
		UVEC3 = GL_UNSIGNED_INT_VEC3,
		UVEC4 = GL_UNSIGNED_INT_VEC4,
		FLOAT = GL_FLOAT,
		VEC2 = GL_FLOAT_VEC2,
		VEC3 = GL_FLOAT_VEC3,
		VEC4 = GL_FLOAT_VEC4,
		MAT2 = GL_FLOAT_MAT2,
		MAT3 = GL_FLOAT_MAT3,
		MAT4 = GL_FLOAT_MAT4,
		MAT2x3 = GL_FLOAT_MAT2x3,
		MAT2x4 = GL_FLOAT_MAT2x4,
		MAT3x2 = GL_FLOAT_MAT3x2,
		MAT3x4 = GL_FLOAT_MAT3x4,
		MAT4x2 = GL_FLOAT_MAT4x2,
		MAT4x3 = GL_FLOAT_MAT4x3,
		SAMPLER2D = GL_SAMPLER_2D,
	};

	// GLSL variable information.
	struct glsl_variable {
		std::string name;
		glsl_type type;
		int array_size;

		friend constexpr bool operator==(const glsl_variable&, const glsl_variable&) = default;
	};

	template <class T> inline constexpr glsl_type as_glsl_type{glsl_type::UNDEFINED};
	template <> inline constexpr glsl_type as_glsl_type<bool>{glsl_type::BOOL};
	template <> inline constexpr glsl_type as_glsl_type<glm::bvec2>{glsl_type::BVEC2};
	template <> inline constexpr glsl_type as_glsl_type<glm::bvec3>{glsl_type::BVEC3};
	template <> inline constexpr glsl_type as_glsl_type<glm::bvec4>{glsl_type::BVEC4};
	template <> inline constexpr glsl_type as_glsl_type<int>{glsl_type::INT};
	template <> inline constexpr glsl_type as_glsl_type<glm::ivec2>{glsl_type::IVEC2};
	template <> inline constexpr glsl_type as_glsl_type<glm::ivec3>{glsl_type::IVEC3};
	template <> inline constexpr glsl_type as_glsl_type<glm::ivec4>{glsl_type::IVEC4};
	template <> inline constexpr glsl_type as_glsl_type<unsigned int>{glsl_type::UINT};
	template <> inline constexpr glsl_type as_glsl_type<glm::uvec2>{glsl_type::UVEC2};
	template <> inline constexpr glsl_type as_glsl_type<glm::uvec3>{glsl_type::UVEC3};
	template <> inline constexpr glsl_type as_glsl_type<glm::uvec4>{glsl_type::UVEC4};
	template <> inline constexpr glsl_type as_glsl_type<float>{glsl_type::FLOAT};
	template <> inline constexpr glsl_type as_glsl_type<glm::vec2>{glsl_type::VEC2};
	template <> inline constexpr glsl_type as_glsl_type<glm::vec3>{glsl_type::VEC3};
	template <> inline constexpr glsl_type as_glsl_type<glm::vec4>{glsl_type::VEC4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat2>{glsl_type::MAT2};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat3>{glsl_type::MAT3};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat4>{glsl_type::MAT4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat2x3>{glsl_type::MAT2x3};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat2x4>{glsl_type::MAT2x4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat3x2>{glsl_type::MAT3x2};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat3x4>{glsl_type::MAT3x4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat4x2>{glsl_type::MAT4x2};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat4x3>{glsl_type::MAT4x3};
}; // namespace tr::gfx

template <> class TR_FMT::formatter<tr::gfx::glsl_type> : protected TR_FMT::formatter<const char*> {
  public:
	using TR_FMT::formatter<const char*>::parse;

	template <typename FormatContext> constexpr auto format(tr::gfx::glsl_type t, FormatContext& ctx) const
	{
		const char* str;
		switch (t) {
		case tr::gfx::glsl_type::BOOL:
			str = "bool";
			break;
		case tr::gfx::glsl_type::BVEC2:
			str = "bvec2";
			break;
		case tr::gfx::glsl_type::BVEC3:
			str = "bvec3";
			break;
		case tr::gfx::glsl_type::BVEC4:
			str = "bvec4";
			break;
		case tr::gfx::glsl_type::INT:
			str = "int";
			break;
		case tr::gfx::glsl_type::IVEC2:
			str = "ivec2";
			break;
		case tr::gfx::glsl_type::IVEC3:
			str = "ivec3";
			break;
		case tr::gfx::glsl_type::IVEC4:
			str = "ivec4";
			break;
		case tr::gfx::glsl_type::UINT:
			str = "unsigned int";
			break;
		case tr::gfx::glsl_type::UVEC2:
			str = "uvec2";
			break;
		case tr::gfx::glsl_type::UVEC3:
			str = "uvec3";
			break;
		case tr::gfx::glsl_type::UVEC4:
			str = "uvec4";
			break;
		case tr::gfx::glsl_type::FLOAT:
			str = "float";
			break;
		case tr::gfx::glsl_type::VEC2:
			str = "vec2";
			break;
		case tr::gfx::glsl_type::VEC3:
			str = "vec3";
			break;
		case tr::gfx::glsl_type::VEC4:
			str = "vec4";
			break;
		case tr::gfx::glsl_type::MAT2:
			str = "mat2";
			break;
		case tr::gfx::glsl_type::MAT3:
			str = "mat3";
			break;
		case tr::gfx::glsl_type::MAT4:
			str = "mat4";
			break;
		case tr::gfx::glsl_type::MAT2x3:
			str = "mat2x3";
			break;
		case tr::gfx::glsl_type::MAT2x4:
			str = "mat2x4";
			break;
		case tr::gfx::glsl_type::MAT3x2:
			str = "mat3x2";
			break;
		case tr::gfx::glsl_type::MAT3x4:
			str = "mat3x4";
			break;
		case tr::gfx::glsl_type::MAT4x2:
			str = "mat4x2";
			break;
		case tr::gfx::glsl_type::MAT4x3:
			str = "mat4x3";
			break;
		case tr::gfx::glsl_type::SAMPLER2D:
			str = "sampler2D";
			break;
		default:
			str = "<unknown>";
			break;
		};
		ctx.advance_to(TR_FMT::formatter<const char*>::format(str, ctx));
		return ctx.out();
	}
};

template <>
class TR_FMT::formatter<tr::gfx::glsl_variable> : private TR_FMT::formatter<tr::gfx::glsl_type>, private TR_FMT::formatter<int> {
  public:
	using TR_FMT::formatter<const char*>::parse;

	template <typename FormatContext> constexpr auto format(const tr::gfx::glsl_variable& v, FormatContext& ctx) const
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
};