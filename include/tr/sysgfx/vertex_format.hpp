#pragma once
#include "../utility/color.hpp"
#include "../utility/handle.hpp"

namespace tr::gfx {
	// A variable type that can be passed as a vertex attribute.
	enum class vertex_attribute_type : u16 {
		UNKNOWN,
		I8 = 0x1400,
		U8,
		I16,
		U16,
		I32,
		U32,
		F32 = 0x1406,
	};
	// Information about a vertex attribute.
	struct vertex_attribute {
		vertex_attribute_type type{vertex_attribute_type::UNKNOWN};
		u8 elements{0};
		bool normalized{false};

		int size_bytes() const;
	};
	// Tag to signify an integral vertex attribute should be normalized.
	struct normalized {};

	template <class T, class AdditionalTag = void> inline constexpr vertex_attribute as_vertex_attribute{};
	template <> inline constexpr vertex_attribute as_vertex_attribute<i8>{vertex_attribute_type::I8, 1, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec2>{vertex_attribute_type::I8, 2, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec3>{vertex_attribute_type::I8, 3, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec4>{vertex_attribute_type::I8, 4, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<i8, normalized>{vertex_attribute_type::I8, 1, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec2, normalized>{vertex_attribute_type::I8, 2, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec3, normalized>{vertex_attribute_type::I8, 3, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i8vec4, normalized>{vertex_attribute_type::I8, 4, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<u8>{vertex_attribute_type::U8, 1, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec2>{vertex_attribute_type::U8, 2, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec3>{vertex_attribute_type::U8, 3, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec4>{vertex_attribute_type::U8, 4, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<u8, normalized>{vertex_attribute_type::U8, 1, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec2, normalized>{vertex_attribute_type::U8, 2, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec3, normalized>{vertex_attribute_type::U8, 3, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u8vec4, normalized>{vertex_attribute_type::U8, 4, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<i16>{vertex_attribute_type::I16, 1, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec2>{vertex_attribute_type::I16, 2, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec3>{vertex_attribute_type::I16, 3, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec4>{vertex_attribute_type::I16, 4, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<i16, normalized>{vertex_attribute_type::I16, 1, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec2, normalized>{vertex_attribute_type::I16, 2, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec3, normalized>{vertex_attribute_type::I16, 3, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i16vec4, normalized>{vertex_attribute_type::I16, 4, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<u16>{vertex_attribute_type::U16, 1, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec2>{vertex_attribute_type::U16, 2, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec3>{vertex_attribute_type::U16, 3, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec4>{vertex_attribute_type::U16, 4, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<u16, normalized>{vertex_attribute_type::U16, 1, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec2, normalized>{vertex_attribute_type::U16, 2, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec3, normalized>{vertex_attribute_type::U16, 3, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u16vec4, normalized>{vertex_attribute_type::U16, 4, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<i32>{vertex_attribute_type::I32, 1, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec2>{vertex_attribute_type::I32, 2, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec3>{vertex_attribute_type::I32, 3, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec4>{vertex_attribute_type::I32, 4, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<i32, normalized>{vertex_attribute_type::I32, 1, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec2, normalized>{vertex_attribute_type::I32, 2, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec3, normalized>{vertex_attribute_type::I32, 3, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::i32vec4, normalized>{vertex_attribute_type::I32, 4, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<u32>{vertex_attribute_type::U32, 1, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec2>{vertex_attribute_type::U32, 2, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec3>{vertex_attribute_type::U32, 3, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec4>{vertex_attribute_type::U32, 4, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<u32, normalized>{vertex_attribute_type::U32, 1, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec2, normalized>{vertex_attribute_type::U32, 2, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec3, normalized>{vertex_attribute_type::U32, 3, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::u32vec4, normalized>{vertex_attribute_type::U32, 4, true};
	template <> inline constexpr vertex_attribute as_vertex_attribute<float>{vertex_attribute_type::F32, 1, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::vec2>{vertex_attribute_type::F32, 2, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::vec3>{vertex_attribute_type::F32, 3, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<glm::vec4>{vertex_attribute_type::F32, 4, false};
	template <> inline constexpr vertex_attribute as_vertex_attribute<rgb8>{as_vertex_attribute<glm::u8vec3, normalized>};
	template <> inline constexpr vertex_attribute as_vertex_attribute<rgba8>{as_vertex_attribute<glm::u8vec4, normalized>};

	// Helper mechanism for generating a vertex attribute initializer list.
	template <class... Ts> struct unpacked_vertex_attributes {
		// Converts the type signature into a list of attributes.
		static std::initializer_list<vertex_attribute> list();
	};
	// Must be overloaded for custom structs by defining for example 'vertex_attributes<foo> : unpacked_vertex_attributes<int, float>'!
	template <class T> struct vertex_attributes : unpacked_vertex_attributes<T> {};

	// Sentinel for a vertex binding that is not instanced (that is, the attributes are per vertex).
	constexpr u32 NOT_INSTANCED{0};
	// A vertex binding point.
	struct vertex_binding {
		// The divisor of the binding.
		u32 divisor;
		// The attributes of the binding.
		std::initializer_list<vertex_attribute> attrs;
	};

	// GPU Vertex format.
	class vertex_format {
	  public:
		// Creates a new vertex format.
		vertex_format(std::initializer_list<vertex_binding> bindings);

		// Sets the debug label of the vertex format.
		void set_label(std::string_view label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL VAO.
		handle<unsigned int, 0, deleter> m_vao;
#ifdef TR_ENABLE_GL_CHECKS
		// Information about the vertex format's bindings.
		std::initializer_list<vertex_binding> m_bindings;
#endif

		friend void set_vertex_format(const vertex_format& format);
	};

	// Gets the vertex format for a common 2D vertex type (separated position, uv, tint).
	vertex_format& vertex2_format();
} // namespace tr::gfx

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <>
struct TR_FMT::formatter<tr::gfx::vertex_attribute> : private TR_FMT::formatter<const char*>, private TR_FMT::formatter<tr::u8> {
	using TR_FMT::formatter<const char*>::parse;

	template <typename FormatContext> constexpr auto format(tr::gfx::vertex_attribute v, FormatContext& ctx) const
	{
		switch (v.type) {
		case tr::gfx::vertex_attribute_type::I8:
			ctx.advance_to(TR_FMT::formatter<const char*>::format("i8", ctx));
			break;
		case tr::gfx::vertex_attribute_type::U8:
			ctx.advance_to(TR_FMT::formatter<const char*>::format("u8", ctx));
			break;
		case tr::gfx::vertex_attribute_type::I16:
			ctx.advance_to(TR_FMT::formatter<const char*>::format("i16", ctx));
			break;
		case tr::gfx::vertex_attribute_type::U16:
			ctx.advance_to(TR_FMT::formatter<const char*>::format("u16", ctx));
			break;
		case tr::gfx::vertex_attribute_type::I32:
			ctx.advance_to(TR_FMT::formatter<const char*>::format("i32", ctx));
			break;
		case tr::gfx::vertex_attribute_type::U32:
			ctx.advance_to(TR_FMT::formatter<const char*>::format("u32", ctx));
			break;
		case tr::gfx::vertex_attribute_type::F32:
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
};

template <class... Ts> std::initializer_list<tr::gfx::vertex_attribute> tr::gfx::unpacked_vertex_attributes<Ts...>::list()
{
	static std::initializer_list<vertex_attribute> attrs{as_vertex_attribute<Ts>...};
	return attrs;
}