/// @file
/// @brief Provides `tr::internal::glsl_type.hpp`.

#pragma once
#include <tr/utility/common.hpp>

//

namespace tr::internal
{
	/// GLSL variable types.
	enum class glsl_type
	{
		/// Undefined variable type.
		undefined,

		/// Boolean variable.
		bool_ = 0x8B56,

		/// 2D boolean vector variable.
		bvec2,

		/// 3D boolean vector variable.
		bvec3,

		/// 4D boolean vector variable.
		bvec4,

		/// Integer variable variable.
		int_ = 0x1404,

		/// 2D integer vector variable.
		ivec2 = 0x8B53,

		/// 3D integer vector variable.
		ivec3,

		/// 4D integer vector variable.
		ivec4,

		/// Unsigned integer variable.
		uint = 0x1405,

		/// 2D unsigned integer vector variable.
		uvec2 = 0x8DC6,

		/// 3D unsigned integer vector variable.
		uvec3,

		/// 4D unsigned integer vector variable.
		uvec4,

		/// Floating-point variable.
		float_ = 0x1406,

		/// 2D floating-point vector variable.
		vec2 = 0x8B50,

		/// 3D floating-point vector variable.
		vec3,

		/// 4D floating-point vector variable.
		vec4,

		/// 2x2 matrix variable.
		mat2 = 0x8B5A,

		/// 3x3 matrix variable.
		mat3,

		/// 4x4 matrix variable.
		mat4,

		/// 2x3 matrix variable.
		mat2x3 = 0x8B65,

		/// 2x4 matrix variable.
		mat2x4,

		/// 3x2 matrix variable.
		mat3x2,

		/// 3x4 matrix variable.
		mat3x4,

		/// 4x2 matrix variable.
		mat4x2,

		/// 4x3 matrix variable.
		mat4x3,

		/// Sampler variable.
		sampler2D = 0x8B5E
	};

	//

	/// Maps a C++ type to a glsl_type enum value.
	/// @tparam T C++ type to get the matching glsl_type value of.
	template <typename T>
	inline constexpr glsl_type as_glsl_type{glsl_type::undefined};

	/// Specialization of `tr::as_glsl_type` for `bool`.
	template <>
	inline constexpr glsl_type as_glsl_type<bool>{glsl_type::bool_};

	/// Specialization of `tr::as_glsl_type` for `bvec2`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::bvec2>{glsl_type::bvec2};

	/// Specialization of `tr::as_glsl_type` for `bvec3`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::bvec3>{glsl_type::bvec3};

	/// Specialization of `tr::as_glsl_type` for `bvec4`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::bvec4>{glsl_type::bvec4};

	/// Specialization of `tr::as_glsl_type` for `int`.
	template <>
	inline constexpr glsl_type as_glsl_type<int>{glsl_type::int_};

	/// Specialization of `tr::as_glsl_type` for `ivec2`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::ivec2>{glsl_type::ivec2};

	/// Specialization of `tr::as_glsl_type` for `ivec3`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::ivec3>{glsl_type::ivec3};

	/// Specialization of `tr::as_glsl_type` for `ivec4`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::ivec4>{glsl_type::ivec4};

	/// Specialization of `tr::as_glsl_type` for `uint`.
	template <>
	inline constexpr glsl_type as_glsl_type<unsigned int>{glsl_type::uint};

	/// Specialization of `tr::as_glsl_type` for `uvec2`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::uvec2>{glsl_type::uvec2};

	/// Specialization of `tr::as_glsl_type` for `uvec3`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::uvec3>{glsl_type::uvec3};

	/// Specialization of `tr::as_glsl_type` for `uvec4`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::uvec4>{glsl_type::uvec4};

	/// Specialization of `tr::as_glsl_type` for `float`.
	template <>
	inline constexpr glsl_type as_glsl_type<float>{glsl_type::float_};

	/// Specialization of `tr::as_glsl_type` for `vec2`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::vec2>{glsl_type::vec2};

	/// Specialization of `tr::as_glsl_type` for `vec3`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::vec3>{glsl_type::vec3};

	/// Specialization of `tr::as_glsl_type` for `vec4`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::vec4>{glsl_type::vec4};

	/// Specialization of `tr::as_glsl_type` for `mat2`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat2>{glsl_type::mat2};

	/// Specialization of `tr::as_glsl_type` for `mat3`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat3>{glsl_type::mat3};

	/// Specialization of `tr::as_glsl_type` for `mat4`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat4>{glsl_type::mat4};

	/// Specialization of `tr::as_glsl_type` for `mat2x3`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat2x3>{glsl_type::mat2x3};

	/// Specialization of `tr::as_glsl_type` for `mat2x4`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat2x4>{glsl_type::mat2x4};

	/// Specialization of `tr::as_glsl_type` for `mat3x2`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat3x2>{glsl_type::mat3x2};

	/// Specialization of `tr::as_glsl_type` for `mat3x4`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat3x4>{glsl_type::mat3x4};

	/// Specialization of `tr::as_glsl_type` for `mat4x2`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat4x2>{glsl_type::mat4x2};

	/// Specialization of `tr::as_glsl_type` for `mat4x3`.
	template <>
	inline constexpr glsl_type as_glsl_type<glm::mat4x3>{glsl_type::mat4x3};
} // namespace tr::internal

//

/// GLSL type formatter.
template <>
class std::formatter<tr::internal::glsl_type>
{
  public:
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid GLSL type format specification."};
		}
		return context.begin();
	}

	/// Formats a GLSL type.
	/// @tparam FormatContext Formatting context type.
	/// @param type GLSL type to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	constexpr FormatContext::iterator format(tr::internal::glsl_type type, FormatContext& context) const
	{
		std::string_view string;
		switch (type) {
		case tr::internal::glsl_type::bool_:
			string = "bool";
			break;
		case tr::internal::glsl_type::bvec2:
			string = "bvec2";
			break;
		case tr::internal::glsl_type::bvec3:
			string = "bvec3";
			break;
		case tr::internal::glsl_type::bvec4:
			string = "bvec4";
			break;
		case tr::internal::glsl_type::int_:
			string = "int";
			break;
		case tr::internal::glsl_type::ivec2:
			string = "ivec2";
			break;
		case tr::internal::glsl_type::ivec3:
			string = "ivec3";
			break;
		case tr::internal::glsl_type::ivec4:
			string = "ivec4";
			break;
		case tr::internal::glsl_type::uint:
			string = "unsigned int";
			break;
		case tr::internal::glsl_type::uvec2:
			string = "uvec2";
			break;
		case tr::internal::glsl_type::uvec3:
			string = "uvec3";
			break;
		case tr::internal::glsl_type::uvec4:
			string = "uvec4";
			break;
		case tr::internal::glsl_type::float_:
			string = "float";
			break;
		case tr::internal::glsl_type::vec2:
			string = "vec2";
			break;
		case tr::internal::glsl_type::vec3:
			string = "vec3";
			break;
		case tr::internal::glsl_type::vec4:
			string = "vec4";
			break;
		case tr::internal::glsl_type::mat2:
			string = "mat2";
			break;
		case tr::internal::glsl_type::mat3:
			string = "mat3";
			break;
		case tr::internal::glsl_type::mat4:
			string = "mat4";
			break;
		case tr::internal::glsl_type::mat2x3:
			string = "mat2x3";
			break;
		case tr::internal::glsl_type::mat2x4:
			string = "mat2x4";
			break;
		case tr::internal::glsl_type::mat3x2:
			string = "mat3x2";
			break;
		case tr::internal::glsl_type::mat3x4:
			string = "mat3x4";
			break;
		case tr::internal::glsl_type::mat4x2:
			string = "mat4x2";
			break;
		case tr::internal::glsl_type::mat4x3:
			string = "mat4x3";
			break;
		case tr::internal::glsl_type::sampler2D:
			string = "sampler2D";
			break;
		default:
			string = "<unknown>";
			break;
		};
		return std::ranges::copy(string, context.out()).out;
	}
};