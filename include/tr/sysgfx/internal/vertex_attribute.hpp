/// @file
/// @brief Implements specializations from vertex_attribute.hpp.

#pragma once
#include <tr/sysgfx/vertex_attribute.hpp>
#include <tr/utility/color.hpp>
#include <tr/utility/specialization_of.hpp>

//

namespace tr
{
	/// Specialization of `as_vertex_attribute` for `i8`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<i8>{vertex_attribute_type::i8, 1, false};

	/// Specialization of `as_vertex_attribute` for `i8vec2`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec2>{vertex_attribute_type::i8, 2, false};

	/// Specialization of `as_vertex_attribute` for `i8vec3`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec3>{vertex_attribute_type::i8, 3, false};

	/// Specialization of `as_vertex_attribute` for `i8vec4`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec4>{vertex_attribute_type::i8, 4, false};

	/// Specialization of `as_vertex_attribute` for `u8`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<u8>{vertex_attribute_type::u8, 1, false};

	/// Specialization of `as_vertex_attribute` for `u8vec2`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec2>{vertex_attribute_type::u8, 2, false};

	/// Specialization of `as_vertex_attribute` for `u8vec3`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec3>{vertex_attribute_type::u8, 3, false};

	/// Specialization of `as_vertex_attribute` for `u8vec4`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec4>{vertex_attribute_type::u8, 4, false};

	/// Specialization of `as_vertex_attribute` for `i16`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<i16>{vertex_attribute_type::i16, 1, false};

	/// Specialization of `as_vertex_attribute` for `i16vec2`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec2>{vertex_attribute_type::i16, 2, false};

	/// Specialization of `as_vertex_attribute` for `i16vec3`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec3>{vertex_attribute_type::i16, 3, false};

	/// Specialization of `as_vertex_attribute` for `i16vec4`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec4>{vertex_attribute_type::i16, 4, false};

	/// Specialization of `as_vertex_attribute` for `u16`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<u16>{vertex_attribute_type::u16, 1, false};

	/// Specialization of `as_vertex_attribute` for `u16vec2`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec2>{vertex_attribute_type::u16, 2, false};

	/// Specialization of `as_vertex_attribute` for `u16vec3`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec3>{vertex_attribute_type::u16, 3, false};

	/// Specialization of `as_vertex_attribute` for `u16vec4`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec4>{vertex_attribute_type::u16, 4, false};

	/// Specialization of `as_vertex_attribute` for `i32`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<i32>{vertex_attribute_type::i32, 1, false};

	/// Specialization of `as_vertex_attribute` for `i32vec2`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec2>{vertex_attribute_type::i32, 2, false};

	/// Specialization of `as_vertex_attribute` for `i32vec3`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec3>{vertex_attribute_type::i32, 3, false};

	/// Specialization of `as_vertex_attribute` for `i32vec4`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec4>{vertex_attribute_type::i32, 4, false};

	/// Specialization of `as_vertex_attribute` for `u32`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<u32>{vertex_attribute_type::u32, 1, false};

	/// Specialization of `as_vertex_attribute` for `u32vec2`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec2>{vertex_attribute_type::u32, 2, false};

	/// Specialization of `as_vertex_attribute` for `u32vec3`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec3>{vertex_attribute_type::u32, 3, false};

	/// Specialization of `as_vertex_attribute` for `u32vec4`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec4>{vertex_attribute_type::u32, 4, false};

	/// Specialization of `as_vertex_attribute` for `float`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<float>{vertex_attribute_type::f32, 1, false};

	/// Specialization of `as_vertex_attribute` for `vec2`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::vec2>{vertex_attribute_type::f32, 2, false};

	/// Specialization of `as_vertex_attribute` for `vec3`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::vec3>{vertex_attribute_type::f32, 3, false};

	/// Specialization of `as_vertex_attribute` for `vec4`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<glm::vec4>{vertex_attribute_type::f32, 4, false};

	/// Specialization of `as_vertex_attribute` for normalized integers.
	template <typename T>
	inline constexpr vertex_attribute as_vertex_attribute<normalized<T>>{
		as_vertex_attribute<T>.type,
		as_vertex_attribute<T>.elements,
		true,
	};

	/// Specialization of `as_vertex_attribute` for `rgb8`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<rgb8>{as_vertex_attribute<normalized<glm::u8vec3>>};

	/// Specialization of `as_vertex_attribute` for `rgba8`.
	template <>
	inline constexpr vertex_attribute as_vertex_attribute<rgba8>{as_vertex_attribute<normalized<glm::u8vec4>>};

	//

	/// Specialization of `as_vertex_attribute_list` for multiple vertex attribute types.
	template <typename... Ts>
		requires requires { (as_vertex_attribute<Ts>, ...); }
	inline constexpr std::array as_vertex_attribute_list<Ts...>{as_vertex_attribute<Ts>...};

	/// Specialization of `as_vertex_attribute_list` for types with a static `::as_vertex_attribute_list` member.
	template <typename T>
		requires(requires {
			{ T::as_vertex_attribute_list } -> cvref_specialization_of_tv<std::array>;
		})
	inline constexpr std::array as_vertex_attribute_list<T>{T::as_vertex_attribute_list};
} // namespace tr