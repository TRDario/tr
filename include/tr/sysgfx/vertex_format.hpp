/// @file
/// @brief Provides `tr::vertex_format` and related functionality.
/// @details For an explanation of vertex formats, see the description of `tr::vertex_format`.

#pragma once
#include <tr/sysgfx/vertex_binding.hpp>
#include <tr/utility/handle.hpp>
#include <tr/utility/ref.hpp>

#ifdef TR_ENABLE_CHECKED_GRAPHICS
#include <tr/sysgfx/internal/graphics_object_registry.hpp>
#endif

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
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
		[[nodiscard]] vertex_format(graphics_context& context, std::span<const vertex_binding> bindings) noexcept;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the vertex format is on.
		/// @return Reference to the graphics context the vertex format is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the vertex format is in a valid state.
		/// @return `true` if the vertex format is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the vertex format.
		/// @param label Label of the vertex format.
		void set_label(std::string_view label) noexcept;

		/// Gets the debug label of the vertex format.
		/// @return Label of the vertex format.
		[[nodiscard]] std::string label() const;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL vertex array object.
		/// @note This does not release the vertex array object.
		/// @return OpenGL vertex array object ID.
		[[nodiscard]] unsigned int unwrap() const noexcept;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		/// Gets the unique graphics object ID of the vertex format.
		/// @return Unique graphics object ID of the vertex format.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;

		/// Gets information about the vertex format's bindings.
		/// @return Information about the vertex format's bindings.
		[[nodiscard]] std::span<const vertex_binding> bindings() const noexcept;

		/// @}
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
			internal::graphics_object_id_handle id{};
#endif

			//

			/// Deletes the VAO.
			/// @param id OpenGL VAO ID.
			void operator()(unsigned int id) const noexcept;
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

//

/// Vertex format formatter.
template <>
struct std::formatter<tr::vertex_format>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid vertex format format specification."};
		}
		return context.begin();
	}

	/// Formats a vertex format.
	/// @tparam FormatContext Formatting context type.
	/// @param format Vertex format to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::vertex_format& format, FormatContext& context) const
	{
		if (format.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", format.label(), format.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid vertex format at {}>", static_cast<const void*>(&format));
		}
	}
};