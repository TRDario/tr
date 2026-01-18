///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a vertex format class and related functionality.                                                                             //
//                                                                                                                                       //
// Vertex formats are an abstraction over OpenGL VAOs.                                                                                   //
//                                                                                                                                       //
// Vertex formats are composed of a list of vertex bindings (points to which vertex buffers are bound), which in turn are composed of a  //
// divisor for instanced attributes (or tr::gfx::not_instanced) and a list of vertex attributes. tr::gfx::as_vertex_attribute<T> is used //
// to map C++ types to vertex attributes; it is defined for primitives and vectors, as well as tr colors, and may be specialized for     //
// custom types:                                                                                                                         //
//     - tr::gfx::as_vertex_attribute<tr::rgba8> -> gets a vertex attribute corresponding to tr::rgba8                                   //
//     - template <> inline constexpr tr::gfx::vertex_attribute as_vertex_attribute<my_vec4>{tr::gfx::as_vertex_attribute<glm::vec4>}    //
//       -> defines my_vec4 as an alias for glm::vec4 in terms of vertex attributes                                                      //
//                                                                                                                                       //
// tr::gfx::as_vertex_attribute_list is defined as a standard form of obtaining lists of attributes for vertex bindings. A type must     //
// define this in order to be eligible for use in vertex buffers. It is defined for lists of types that can be converted to vertex       //
// attributes (in which case they are combined into a list), as well as types that  declare T::as_vertex_attribute_list as a public      //
// static array of vertex attributes, and may be specialized for custom types:                                                           //
//     - tr::gfx::as_vertex_attribute_list<tr::u8, tr::u8, float> -> list containing the vertex attributes for u8, u8, and float         //
//     - struct my_struct {                                                                                                              //
//           glm::vec2 pos;                                                                                                              //
//           float opacity;                                                                                                              //
//                                                                                                                                       //
//           static constexpr auto as_vertex_attribute_list{tr::gfx::as_vertex_attribute_list<glm::vec2, float>};                        //
//       };                                                                                                                              //
//       tr::gfx::as_vertex_attribute<my_struct> -> takes the list from my_struct::as_vertex_attribute_list                              //
//                                                                                                                                       //
// As a convenience, tr::gfx::make_vertex_binding<T> is provided to avoid having to directly use tr::gfx::as_vertex_attribute list to    //
// construct vertex bindings:                                                                                                            //
//     - tr::gfx::make_vertex_binding<glm::vec2>()                                                                                       //
//       -> equivalent to {.divisor = tr::gfx::not_instanced, .attrs = tr::gfx::as_vertex_attribute_list<glm::vec2>}                     //
//     - tr::gfx::make_vertex_binding<tr::rgba8>(1)                                                                                      //
//       -> equivalent to {.divisor = 1, .attrs = tr::gfx::as_vertex_attribute_list<tr::rgba8>}                                          //
//                                                                                                                                       //
// A vertex format is constructed with a span of vertex bindings: this span must stay valid for the entire lifetime of the format.       //
// The label of a vertex format can be set with TR_SET_LABEL(format, label):                                                             //
//     - constexpr std::array format_bindings{tr::gfx::make_vertex_binding<glm::vec2>(), tr::gfx::make_vertex_binding<my_struct>}        //
//       -> defines two vertex bindings, one taking glm::vec2 data, and the other my_struct data                                         //
//     - tr::gfx::vertex_format format{format_bindings}; TR_SET_LABEL(format, "Example format")                                          //
//       -> creates a vertex format with the defined bindings with the label "Example format"                                            //
//                                                                                                                                       //
// tr::gfx::vertex2_format() is provided as a built-in vertex format that is always available: separated vec2 position, vec2 uv, and     //
// rgba8 color bindings.                                                                                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// A variable type that can be passed as a vertex attribute.
	enum class vertex_attribute_type : u16 {
		unknown,
		i8 = 0x1400,
		u8,
		i16,
		u16,
		i32,
		u32,
		f32 = 0x1406,
	};
	// Information about a vertex attribute.
	struct vertex_attribute {
		// The base type of the attribute.
		vertex_attribute_type type{vertex_attribute_type::unknown};
		// The number of elements in the attribute.
		u8 elements{0};
		// Whether the attribute is normalized (if integral).
		bool normalized{false};
	};
	// Tag to signify an integral vertex attribute should be normalized.
	template <class T> struct normalized {};

	// Maps a C++ type to a vertex attribute. Mappings for 32-bit or smaller integers (wrapped in normalized or not) and floats, glm::vecs
	// of such, and colors are defined by default.
	template <class T>
	inline constexpr vertex_attribute as_vertex_attribute{
		TR_UNSPECIALIZED_VARIABLE_TEMPLATE(T, vertex_attribute, "Type is not convertible to vertex_attribute!"),
	};

	// Maps a C++ type or a list of C++ types to a list of vertex attributes. If the list is composed entirely of types convertible to
	// vertex attributes, they will be grouped into a list. If the type has as_vertex_attribute_list as a public static array of vertex
	// attributes, that list will be used. Otherwise, the type must be manually specialized.
	template <class... Ts>
	inline constexpr std::array as_vertex_attribute_list{
		TR_UNSPECIALIZED_VARIABLE_TEMPLATE(Ts..., TR_MACRO_COMMA_GUARD(std::array<vertex_attribute, 0>),
										   "Type(s) is/are not convertible to vertex attribute list!"),
	};

	// Sentinel for a vertex binding that is not instanced (that is, the attributes are per vertex).
	constexpr u32 not_instanced{0};
	// A vertex binding point.
	struct vertex_binding {
		// The divisor of the binding.
		u32 divisor;
		// The attributes of the binding. This span is expected to last for the entire duration of the vertex format's lifetime.
		std::span<const vertex_attribute> attrs;
	};
	// Constructs a vertex binding using tr::gfx::as_vertex_attribute_list<T>.
	template <class T> constexpr vertex_binding make_vertex_binding(u32 divisor = not_instanced);

	// GPU vertex format.
	class vertex_format {
	  public:
		// Creates a new vertex format.
		vertex_format(std::span<const vertex_binding> bindings);

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the vertex format.
		void set_label(std::string_view label);
		// Gets the debug label of the vertex format.
		std::string label() const;
#endif

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL VAO.
		handle<unsigned int, 0, deleter> m_vao;
#ifdef TR_ENABLE_GL_CHECKS
		// Information about the vertex format's bindings.
		std::span<const vertex_binding> m_bindings;
#endif

		friend void set_vertex_format(const vertex_format& format);
	};

	// Gets the vertex format for a common 2D vertex type (separated position, uv, tint).
	vertex_format& vertex2_format();
} // namespace tr::gfx

// Vertex attribute formatter.
template <>
struct TR_FMT::formatter<tr::gfx::vertex_attribute> : private TR_FMT::formatter<const char*>, private TR_FMT::formatter<tr::u8> {
	using TR_FMT::formatter<const char*>::parse;
	// Formats a vertex attribute.
	template <typename FormatContext> constexpr auto format(tr::gfx::vertex_attribute v, FormatContext& ctx) const;
};

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "vertex_format_impl.hpp" // IWYU pragma: export