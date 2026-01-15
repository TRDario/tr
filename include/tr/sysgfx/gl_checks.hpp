#pragma once
#include "gl_call.hpp"

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
		std::string name;
		glsl_type type;
		int array_size;
	};

	template <class T> inline constexpr glsl_type as_glsl_type{glsl_type::undefined};
	template <> inline constexpr glsl_type as_glsl_type<bool>{glsl_type::bool_};
	template <> inline constexpr glsl_type as_glsl_type<glm::bvec2>{glsl_type::bvec2};
	template <> inline constexpr glsl_type as_glsl_type<glm::bvec3>{glsl_type::bvec3};
	template <> inline constexpr glsl_type as_glsl_type<glm::bvec4>{glsl_type::bvec4};
	template <> inline constexpr glsl_type as_glsl_type<int>{glsl_type::int_};
	template <> inline constexpr glsl_type as_glsl_type<glm::ivec2>{glsl_type::ivec2};
	template <> inline constexpr glsl_type as_glsl_type<glm::ivec3>{glsl_type::ivec3};
	template <> inline constexpr glsl_type as_glsl_type<glm::ivec4>{glsl_type::ivec4};
	template <> inline constexpr glsl_type as_glsl_type<unsigned int>{glsl_type::uint};
	template <> inline constexpr glsl_type as_glsl_type<glm::uvec2>{glsl_type::uvec2};
	template <> inline constexpr glsl_type as_glsl_type<glm::uvec3>{glsl_type::uvec3};
	template <> inline constexpr glsl_type as_glsl_type<glm::uvec4>{glsl_type::uvec4};
	template <> inline constexpr glsl_type as_glsl_type<float>{glsl_type::float_};
	template <> inline constexpr glsl_type as_glsl_type<glm::vec2>{glsl_type::vec2};
	template <> inline constexpr glsl_type as_glsl_type<glm::vec3>{glsl_type::vec3};
	template <> inline constexpr glsl_type as_glsl_type<glm::vec4>{glsl_type::vec4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat2>{glsl_type::mat2};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat3>{glsl_type::mat3};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat4>{glsl_type::mat4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat2x3>{glsl_type::mat2x3};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat2x4>{glsl_type::mat2x4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat3x2>{glsl_type::mat3x2};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat3x4>{glsl_type::mat3x4};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat4x2>{glsl_type::mat4x2};
	template <> inline constexpr glsl_type as_glsl_type<glm::mat4x3>{glsl_type::mat4x3};
}; // namespace tr::gfx

template <> class TR_FMT::formatter<tr::gfx::glsl_type> : protected TR_FMT::formatter<const char*> {
  public:
	using TR_FMT::formatter<const char*>::parse;

	template <typename FormatContext> constexpr auto format(tr::gfx::glsl_type t, FormatContext& ctx) const
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