///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the templated parts of vertex_format.hpp.                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/color.hpp"
#include "vertex_format.hpp"

//////////////////////////////////////////////////////////// VERTEX ATTRIBUTES ////////////////////////////////////////////////////////////

template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::i8>{vertex_attribute_type::i8, 1, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i8vec2>{vertex_attribute_type::i8, 2, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i8vec3>{vertex_attribute_type::i8, 3, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i8vec4>{vertex_attribute_type::i8, 4, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::u8>{vertex_attribute_type::u8, 1, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u8vec2>{vertex_attribute_type::u8, 2, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u8vec3>{vertex_attribute_type::u8, 3, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u8vec4>{vertex_attribute_type::u8, 4, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::i16>{vertex_attribute_type::i16, 1, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i16vec2>{vertex_attribute_type::i16, 2, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i16vec3>{vertex_attribute_type::i16, 3, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i16vec4>{vertex_attribute_type::i16, 4, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::u16>{vertex_attribute_type::u16, 1, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u16vec2>{vertex_attribute_type::u16, 2, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u16vec3>{vertex_attribute_type::u16, 3, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u16vec4>{vertex_attribute_type::u16, 4, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::i32>{vertex_attribute_type::i32, 1, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i32vec2>{vertex_attribute_type::i32, 2, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i32vec3>{vertex_attribute_type::i32, 3, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::i32vec4>{vertex_attribute_type::i32, 4, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::u32>{vertex_attribute_type::u32, 1, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u32vec2>{vertex_attribute_type::u32, 2, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u32vec3>{vertex_attribute_type::u32, 3, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::u32vec4>{vertex_attribute_type::u32, 4, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<float>{vertex_attribute_type::f32, 1, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::vec2>{vertex_attribute_type::f32, 2, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::vec3>{vertex_attribute_type::f32, 3, false};
template <> inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<glm::vec4>{vertex_attribute_type::f32, 4, false};

template <class T>
inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::gfx::normalized<T>>{as_vertex_attribute<T>.type,
																								as_vertex_attribute<T>.elements, true};

template <>
inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::rgb8>{tr::gfx::as_vertex_attribute<normalized<glm::u8vec3>>};
template <>
inline constexpr tr::gfx::vertex_attribute tr::gfx::as_vertex_attribute<tr::rgba8>{tr::gfx::as_vertex_attribute<normalized<glm::u8vec4>>};

////////////////////////////////////////////////////////// VERTEX ATTRIBUTE LIST //////////////////////////////////////////////////////////

namespace tr::gfx {
	template <class T>
	concept convertible_to_vertex_attribute = requires { as_vertex_attribute<T>; };
} // namespace tr::gfx

template <tr::gfx::convertible_to_vertex_attribute... Ts>
inline constexpr std::array tr::gfx::as_vertex_attribute_list<Ts...>{as_vertex_attribute<Ts>...};

template <class T>
	requires(requires {
		{ T::as_vertex_attribute_list } -> tr::cvref_specialization_of_tv<std::array>;
	})
inline constexpr std::array tr::gfx::as_vertex_attribute_list<T>{T::as_vertex_attribute_list};

////////////////////////////////////////////////////////////// VERTEX BINDING /////////////////////////////////////////////////////////////

template <class T> constexpr tr::gfx::vertex_binding tr::gfx::make_vertex_binding(u32 divisor)
{
	return {divisor, as_vertex_attribute_list<T>};
}

//////////////////////////////////////////////////////////////// FORMATTER ////////////////////////////////////////////////////////////////

template <typename FormatContext>
constexpr auto TR_FMT::formatter<tr::gfx::vertex_attribute>::format(tr::gfx::vertex_attribute v, FormatContext& ctx) const
{
	switch (v.type) {
	case tr::gfx::vertex_attribute_type::i8:
		ctx.advance_to(TR_FMT::formatter<const char*>::format("i8", ctx));
		break;
	case tr::gfx::vertex_attribute_type::u8:
		ctx.advance_to(TR_FMT::formatter<const char*>::format("u8", ctx));
		break;
	case tr::gfx::vertex_attribute_type::i16:
		ctx.advance_to(TR_FMT::formatter<const char*>::format("i16", ctx));
		break;
	case tr::gfx::vertex_attribute_type::u16:
		ctx.advance_to(TR_FMT::formatter<const char*>::format("u16", ctx));
		break;
	case tr::gfx::vertex_attribute_type::i32:
		ctx.advance_to(TR_FMT::formatter<const char*>::format("i32", ctx));
		break;
	case tr::gfx::vertex_attribute_type::u32:
		ctx.advance_to(TR_FMT::formatter<const char*>::format("u32", ctx));
		break;
	case tr::gfx::vertex_attribute_type::f32:
		if (v.elements == 1) {
			ctx.advance_to(TR_FMT::formatter<const char*>::format("float", ctx));
		}
		break;
	default:
		ctx.advance_to(TR_FMT::formatter<const char*>::format("<unknown>", ctx));
		return ctx.out();
	}

	if (v.elements != 1) {
		ctx.advance_to(TR_FMT::formatter<const char*>::format("vec", ctx));
		ctx.advance_to(TR_FMT::formatter<tr::u8>::format(v.elements, ctx));
	}

	return ctx.out();
}