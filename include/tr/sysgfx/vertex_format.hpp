/// @file
/// @brief Provides `tr::vertex_format` and related functionality.
/// @details For an explanation of vertex formats, see the description of `tr::vertex_format`.

#pragma once
#include "../utility/handle.hpp"
#include "../utility/integer.hpp"
#include "../utility/macro.hpp"
#include "../utility/reference.hpp"
#include "../utility/specialization_of.hpp"

#ifdef TR_ENABLE_CHECKED_GRAPHICS
#include "graphics_object_registry.hpp"
#endif

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
	/// @hideinitializer
	template <typename T>
	inline constexpr vertex_attribute as_vertex_attribute{
		TR_UNSPECIALIZED_VARIABLE_TEMPLATE(T, vertex_attribute, "Type is not convertible to vertex_attribute!"),
	};

	/// Maps a C++ type or a list of C++ types to a list of vertex attributes.
	/// @note If the list is composed entirely of types convertible to vertex attributes, they will be grouped into a list.
	/// @note If the type has as_vertex_attribute_list as a public static array of vertex attributes, that list will be used.
	/// @note Otherwise, the type must be manually specialized.
	/// @tparam Ts Types to get the vertex attribute list for.
	/// @hideinitializer
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

	/// Maps a type (attributes obtained using `tr::as_vertex_attribute_list<T>`) and divisor to a vertex binding at compile-time.
	/// @tparam T Type to get the attribute list for.
	/// @tparam Divisor Divisor of the binding.
	/// @hideinitializer
	template <typename T, u32 Divisor = not_instanced>
	inline constexpr vertex_binding as_vertex_binding{Divisor, as_vertex_attribute_list<T>};

	//

	/// Vertex binding point tag struct used in `tr::as_vertex_bindings`.
	/// @tparam T Type to get the attribute list for.
	/// @tparam Divisor Divisor of the binding.
	template <typename T, u32 Divisor = not_instanced>
	struct vertex_binding_tag
	{
		/// Type to get the attribute list for.
		using type = T;

		/// Divisor of the binding.
		static constexpr u32 divisor{Divisor};
	};

	/// @cond implementation_details

	/// Holds a list of vertex bindings as if gotten by `tr::as_vertex_binding`.
	/// @tparam BindingTags List of vertex binding tags.
	/// @hideinitializer
	template <specialization_of_tv<vertex_binding_tag>... BindingTags>
	inline constexpr std::array<vertex_binding, sizeof...(BindingTags)> as_vertex_bindings_array{
		as_vertex_binding<typename BindingTags::type, BindingTags::divisor>...,
	};

	/// @endcond

	/// Holds a span of vertex bindings as if gotten by `tr::as_vertex_binding` with a guaranteed static lifetime.
	/// @tparam BindingTags List of vertex binding tags, see `tr::vertex_binding_tag`.
	/// @hideinitializer
	template <specialization_of_tv<vertex_binding_tag>... BindingTags>
	inline constexpr std::span<const vertex_binding, sizeof...(BindingTags)> as_vertex_bindings{as_vertex_bindings_array<BindingTags...>};

	//

	/// Container for a GPU vertex format.
	/// @details
	/// Vertex formats define the layout of vertex data passed to a drawing operation. This includes the type, attributes and divisor of the
	/// data set to be held in one or more indexed vertex buffers. They can be considered the counterpart of vertex array objects in OpenGL.
	///
	/// Strictly speaking, instances of `tr::vertex_format` are only containers for these underlying vertex format objects. This means, for
	/// example, that setting a vertex format on a graphics context does not set the literal `tr::vertex_format` object at a specific
	/// location in memory, but rather the value it contains. If the value is moved to a different instance of `tr::vertex_format`, that
	/// value will still be set on the context. If the instance is overriden with a new value, the old value is destroyed and the graphics
	/// context will no longer have a set vertex format.
	///
	/// Every instance of `tr::vertex_format` is associated with a graphics context and cannot outlive its parent context.
	///
	/// `tr::vertex_format` instances are movable, but not copyable. A moved-from instance of `tr::vertex_format` is left in a special
	/// 'invalid' state. Invalid `tr::vertex_format` instances may not be interacted with besides moving a new value into them and checking
	/// for validity using `valid()`.
	///
	/// `tr::vertex_format` instances may be labeled and are formattable. Example format output: `"My vertex format" (OpenGL ID: 5)`.
	class vertex_format
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates a new vertex format.
		/// @param context Graphics context to create the format on.
		/// @param bindings Vertex bindings of the format.
		/// @post `bindings` must stay valid for the duration of the lifetime of the vertex format.
		vertex_format(graphics_context& context, std::span<const vertex_binding> bindings);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the vertex format is on.
		/// @return Reference to the graphics context the vertex format is on.
		graphics_context& context() const;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the vertex format is in a valid state.
		/// @return `true` if the vertex format is in a valid state, `false` if it is in an invalid state.
		bool valid() const;

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
		/// @cond gl_interop

		/// Unwraps the OpenGL vertex array object.
		/// @note This does not release the vertex array object.
		/// @return OpenGL vertex array object ID.
		unsigned int unwrap() const;

		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details

		/// Gets the unique graphics object ID of the vertex format.
		/// @return Unique graphics object ID of the vertex format.
		graphics_object_id id() const;

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

#ifdef TR_ENABLE_CHECKED_GRAPHICS
			/// Handle to the unique graphics object ID of the vertex format.
			graphics_object_id_handle id{};
#endif

			//

			/// Deletes the VAO.
			/// @param id OpenGL VAO ID.
			void operator()(unsigned int id) const;
		};

		//

		/// Handle to the OpenGL VAO.
		handle<unsigned int, 0, deleter> m_handle;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Information about the vertex format's bindings.
		std::span<const vertex_binding> m_bindings;
#endif
	};
} // namespace tr

#include "impl/vertex_format.hpp" // IWYU pragma: export