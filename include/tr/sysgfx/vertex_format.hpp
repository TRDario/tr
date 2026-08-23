/// @file
/// @brief Provides a vertex format class and related functionality.

#pragma once
#include "../utility/handle.hpp"
#include "../utility/integer.hpp"
#include "../utility/macro.hpp"
#include "../utility/reference.hpp"

namespace tr
{
	class graphics_context;
}

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
	/// @tparam T Type to signify is normalized.
	template <typename T>
	struct normalized
	{
	};

	/// Maps a C++ type to a vertex attribute.
	/// @note Mappings for 32-bit or smaller integers (wrapped in normalized or not) and floats, vectors of such, and colors are predefined.
	/// @tparam T Type to get the vertex attribute for.
	template <typename T>
	inline constexpr vertex_attribute as_vertex_attribute{
		TR_UNSPECIALIZED_VARIABLE_TEMPLATE(T, vertex_attribute, "Type is not convertible to vertex_attribute!"),
	};

	/// Maps a C++ type or a list of C++ types to a list of vertex attributes.
	/// @note If the list is composed entirely of types convertible to vertex attributes, they will be grouped into a list.
	/// @note If the type has as_vertex_attribute_list as a public static array of vertex attributes, that list will be used.
	/// @note Otherwise, the type must be manually specialized.
	/// @tparam Ts Types to get the vertex attribute list for.
	template <typename... Ts>
	inline constexpr std::array as_vertex_attribute_list{
		TR_UNSPECIALIZED_VARIABLE_TEMPLATE(Ts..., TR_MACRO_COMMA_GUARD(std::array<vertex_attribute, 0>),
										   "Type(s) is/are not convertible to vertex attribute list!"),
	};

	//

	/// Sentinel for a vertex binding that is not instanced (that is, the attributes are per vertex).
	constexpr u32 not_instanced{0};

	/// Vertex binding point.
	struct vertex_binding
	{
		/// Divisor of the binding.
		u32 divisor;

		/// Attributes of the binding. This span is expected to last for the entire duration of the vertex format's lifetime.
		std::span<const vertex_attribute> attrs;
	};

	/// Constructs a vertex binding using `tr::as_vertex_attribute_list<T>`.
	/// @tparam T Type to get the attribute list for.
	/// @param divisor Divisor of the binding.
	template <typename T>
	constexpr vertex_binding make_vertex_binding(u32 divisor = not_instanced);

	//

	/// GPU vertex format.
	class vertex_format
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates a new vertex format.
		/// @param context Graphics context to create the format on.
		/// @param bindings Vertex bindings of the format.
		vertex_format(graphics_context& context, std::span<const vertex_binding> bindings);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the vertex format is on.
		/// @return Reference to the graphics context the vertex format is on.
		graphics_context& context() const;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the vertex format.
		/// @param label Label of the vertex format.
		void set_label(std::string_view label);

		/// Gets the debug label of the vertex format.
		/// @return Label of the vertex format.
		std::string label() const;

		/// @}
		/// @name ID.
		/// @{

		/// Gets the unique ID of the vertex format.
		/// @return Unique ID of the vertex format.
		unsigned int id() const;

		/// @}

#ifdef TR_ENABLE_GL_CHECKS
		/// @cond __hidden
		/// Gets information about the vertex format's bindings.
		/// @return Information about the vertex format's bindings.
		std::span<const vertex_binding> bindings() const;
		/// @endcond
#endif

	  private:
		/// VAO deleter class.
		struct deleter
		{
			/// Reference to the graphics context the VAO is on.
			ref<graphics_context> context;

			//

			/// Deletes the VAO.
			/// @param id OpenGL VAO ID.
			void operator()(unsigned int id) const;
		};

		//

		/// Handle to the OpenGL VAO.
		handle<unsigned int, 0, deleter> m_vao;

#ifdef TR_ENABLE_GL_CHECKS
		/// Information about the vertex format's bindings.
		std::span<const vertex_binding> m_bindings;
#endif
	};
} // namespace tr

#include "impl/vertex_format.hpp" // IWYU pragma: export