/// @file
/// @brief Provides utilities relating to `tr::vertex_attribute`.

#pragma once
#include <tr/utility/integer.hpp>
#include <tr/utility/macro.hpp>

//

namespace tr
{
	/// Variable type that can be passed as a vertex attribute.
	enum class vertex_attribute_type : u16
	{
		/// Unknown vertex attribute.
		unknown,

		/// Signed 8-bit integer type.
		i8 = 0x1400,

		/// Unsigned 8-bit integer type.
		u8,

		/// Signed 16-bit integer type.
		i16,

		/// Unsigned 16-bit integer type.
		u16,

		/// Signed 32-bit integer type.
		i32,

		/// Unsigned 32-bit integer type.
		u32,

		/// 32-bit floating-point type.
		f32 = 0x1406,
	};

	//

	/// Information about a vertex attribute.
	struct vertex_attribute
	{
		/// Base type of the attribute.
		vertex_attribute_type type{vertex_attribute_type::unknown};

		/// Number of elements in the attribute.
		u8 elements{0};

		/// Whether the attribute is normalized (if integral).
		bool normalized{false};
	};

	/// Tag to signify an integral vertex attribute should be normalized.
	/// @tparam Target Type to signify is normalized.
	template <typename Target>
	struct normalized
	{
	};

	/// Maps a C++ type to a vertex attribute.
	/// @note Mappings for 32-bit or smaller integers (wrapped in normalized or not) and floats, vectors of such, and colors are predefined.
	/// @tparam Target Type to get the vertex attribute for.
	/// @hideinitializer
	template <typename Target>
	inline constexpr vertex_attribute as_vertex_attribute{
		TR_UNSPECIALIZED_VARIABLE_TEMPLATE(T, vertex_attribute, "Type is not convertible to vertex_attribute!"),
	};

	/// Maps a C++ type or a list of C++ types to a list of vertex attributes.
	/// @note If the list is composed entirely of types convertible to vertex attributes, they will be grouped into a list.
	/// @note If the type has as_vertex_attribute_list as a public static array of vertex attributes, that list will be used.
	/// @note Otherwise, the type must be manually specialized.
	/// @tparam Targets Types to get the vertex attribute list for.
	/// @hideinitializer
	template <typename... Targets>
	inline constexpr std::array as_vertex_attribute_list{
		TR_UNSPECIALIZED_VARIABLE_TEMPLATE(Targets..., TR_MACRO_COMMA_GUARD(std::array<vertex_attribute, 0>),
										   "Type(s) is/are not convertible to vertex attribute list!"),
	};
} // namespace tr

//

/// Vertex attribute formatter.
template <>
class std::formatter<tr::vertex_attribute>
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
			throw std::format_error{"Invalid vertex attribute format specification."};
		}
		return context.begin();
	}

	/// Formats a vertex attribute.
	/// @tparam FormatContext Formatting context type.
	/// @param attribute Vertex attribute to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::vertex_attribute& attribute, FormatContext& context) const
	{
		switch (attribute.type) {
		case tr::vertex_attribute_type::i8:
			context.advance_to(std::format_to(context.out(), "i8"));
			break;
		case tr::vertex_attribute_type::u8:
			context.advance_to(std::format_to(context.out(), "u8"));
			break;
		case tr::vertex_attribute_type::i16:
			context.advance_to(std::format_to(context.out(), "i16"));
			break;
		case tr::vertex_attribute_type::u16:
			context.advance_to(std::format_to(context.out(), "u16"));
			break;
		case tr::vertex_attribute_type::i32:
			context.advance_to(std::format_to(context.out(), "i32"));
			break;
		case tr::vertex_attribute_type::u32:
			context.advance_to(std::format_to(context.out(), "u32"));
			break;
		case tr::vertex_attribute_type::f32:
			if (attribute.elements == 1) {
				return std::format_to(context.out(), "float");
			}
			break;
		default:
			return std::format_to(context.out(), "<unknown>");
		}

		if (attribute.elements != 1) {
			return std::format_to(context.out(), "vec{}", attribute.elements);
		}
		else {
			return context.out();
		}
	}
};

#include "internal/vertex_attribute.hpp" // IWYU pragma: export