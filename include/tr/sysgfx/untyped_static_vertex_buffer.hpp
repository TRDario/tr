/// @file
/// @brief Provides `tr::untyped_static_vertex_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Static, immutable untyped vertex buffer.
	/// @details
	/// `tr::untyped_static_vertex_buffer` represents a graphics buffer used to store vertex data. Unlike
	/// `tr::dynamic_untyped_vertex_buffer`, it represents a single, static allocation of immutable data, making it suitable for scenarios
	/// where the vertex data is constant. `tr::untyped_static_vertex_buffer` does not enforce any specific data format and may be used when
	/// flexibility is needed, while `tr::static_vertex_buffer` models a buffer holding vertices of a specific type.
	///
	/// Strictly speaking, instances of `tr::untyped_static_vertex_buffer` are only containers for these underlying buffer objects. This
	/// means, for example, that setting a static vertex buffer on a graphics context does not set the literal
	/// `tr::untyped_static_vertex_buffer` object at a specific location in memory, but rather the value it contains. If the value is moved
	/// to a different instance of `tr::untyped_static_vertex_buffer`, that value will still be set on the context. If the instance is
	/// overriden with a new value, the old value is destroyed and the graphics context will no longer have a set vertex buffer.
	///
	/// Every instance of `tr::untyped_static_vertex_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::untyped_static_vertex_buffer` are left in a special 'invalid' state. Invalid
	/// `tr::untyped_static_vertex_buffer` instances may not be interacted with besides moving a new value into them and checking for
	/// validity using `valid()`.
	///
	/// `tr::untyped_static_vertex_buffer` instances may be labeled and are formattable. Example format output: `"My vertex buffer" (OpenGL
	/// ID: 5)`.
	class untyped_static_vertex_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Uploads vertex data into a static vertex buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to upload to the buffer.
		[[nodiscard]] untyped_static_vertex_buffer(graphics_context& context, std::span<const std::byte> data);

		/// Untyped static vertex buffers are not copyable.
		untyped_static_vertex_buffer(const untyped_static_vertex_buffer&) = delete;

		/// Moves an untyped static vertex buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped static vertex buffer to move.
		[[nodiscard]] untyped_static_vertex_buffer(untyped_static_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Untyped static vertex buffers are not copyable.
		untyped_static_vertex_buffer& operator=(const untyped_static_vertex_buffer&) = delete;

		/// Moves an untyped static vertex buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped static vertex buffer to move.
		/// @return Reference to `*this`.
		untyped_static_vertex_buffer& operator=(untyped_static_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name State
		/// @{

		using graphics_buffer::valid;

		/// @}
		/// @name Label
		/// @{

		using graphics_buffer::label;

		using graphics_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using graphics_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using graphics_buffer::id;

		/// @}
		/// @endcond
#endif

	  private:
		/// Size of the vertex buffer in bytes.
		ssize m_size;
	};
} // namespace tr

//

/// Untyped static vertex buffer formatter.
template <>
struct std::formatter<tr::untyped_static_vertex_buffer>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid untyped static vertex buffer format specification."};
		}
		return context.begin();
	}

	/// Formats an untyped static vertex buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Vertex buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_static_vertex_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped static vertex buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};