#pragma once
#include "../utility/handle.hpp"

namespace tr::gfx {
	// Single-precision float vertex attribute.
	struct vertex_attributef {
		// The source data's type.
		enum class type {
			SI8 = 0x1400,
			UI8,
			SI16,
			UI16,
			SI32,
			UI32,
			FP16 = 0x140B,
			FP32 = 0x1406,
			FP64 = 0x140A,
			FIXED32 = 0x140C,
			SI2_10_10_10 = 0x8D9F,
			UI2_10_10_10 = 0x8368,
			FP10_11_11 = 0x8C3B
		};

		// The type being converted to a float.
		type type;
		// The number of elements in the attribute (1 - 4).
		std::uint8_t elements;
		// Whether integer data is normalized when converted to floating point. Must be false for floats.
		bool normalized;
		// The relative position of the attribute within a binding point element in bytes.
		std::uint32_t offset;
	};
	// Integer vertex attribute.
	struct vertex_attributei {
		// The source data's type.
		enum class type {
			SI8 = 0x1400,
			UI8,
			SI16,
			UI16,
			SI32,
			UI32,
			SI2_10_10_10 = 0x8D9F,
			UI2_10_10_10 = 0x8368
		};

		// The type being converted to an intger.
		type type;
		// The number of elements in the attribute (1 - 4).
		std::uint8_t elements;
		// The relative position of the attribute within a binding point element in bytes.
		std::uint32_t offset;
	};
	// Union of vertex attribute types.
	using vertex_attribute = std::variant<vertex_attributef, vertex_attributei>;

	// Sentinel for a vertex binding that is not instanced (that is, the attributes are per vertex).
	constexpr std::uint32_t NOT_INSTANCED{0};
	// A vertex binding point.
	struct vertex_binding {
		// The divisor of the binding.
		std::uint32_t divisor;
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

		friend void set_vertex_format(const vertex_format& format);
	};

	// Gets the vertex format for a common 2D vertex type (separated position, uv, tint).
	vertex_format& vertex2_format();
} // namespace tr::gfx
