/// @file
/// @brief Provides `tr::untyped_dynamic_vertex_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Untyped dynamic vertex buffer class.
	/// @details
	/// `tr::untyped_dynamic_vertex_buffer` represents a graphics buffer used to store vertex data. Unlike
	/// `tr::untyped_static_vertex_buffer`, it allows for dynamic resizing and updating of its contents, making it suitable for scenarios
	/// where the vertex data changes often at runtime. `tr::untyped_dynamic_vertex_buffer` does not enforce any specific data format and
	/// may be used when flexibility is needed, while `tr::dynamic_vertex_buffer` models a buffer holding vertices of a specific type.
	///
	/// Strictly speaking, instances of `tr::untyped_dynamic_vertex_buffer` are only containers for these underlying buffer objects. This
	/// means, for example, that setting a dynamic vertex buffer on a graphics context does not set the literal
	/// `tr::untyped_dynamic_vertex_buffer` object at a specific location in memory, but rather the value it contains. If the value is moved
	/// to a different instance of `tr::untyped_dynamic_vertex_buffer`, that value will still be set on the context. If the instance is
	/// overriden with a new value, the old value is destroyed and the graphics context will no longer have a set vertex buffer.
	///
	/// Every instance of `tr::untyped_dynamic_vertex_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::untyped_dynamic_vertex_buffer` are left in a special 'invalid' state. Invalid
	/// `tr::untyped_dynamic_vertex_buffer` instances may not be interacted with besides moving a new value into them and checking for
	/// validity using `valid()`.
	///
	/// `tr::untyped_dynamic_vertex_buffer` instances may be labeled and are formattable. Example format output: `"My vertex buffer" (ID: 3,
	/// VBO: 5)`.
	class untyped_dynamic_vertex_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an empty untyped dynamic index buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param label Label of the buffer.
		[[nodiscard]] explicit untyped_dynamic_vertex_buffer(graphics_context& context, std::string_view label = {}) noexcept;

		/// Untyped dynamic index buffers are not copyable.
		untyped_dynamic_vertex_buffer(const untyped_dynamic_vertex_buffer&) = delete;

		/// Moves an untyped dynamic index buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped dynamic index buffer to move.
		[[nodiscard]] untyped_dynamic_vertex_buffer(untyped_dynamic_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Untyped dynamic index buffers are not copyable.
		untyped_dynamic_vertex_buffer& operator=(const untyped_dynamic_vertex_buffer&) = delete;

		/// Moves an untyped dynamic vertex buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped dynamic vertex buffer to move.
		/// @return Reference to `*this`.
		untyped_dynamic_vertex_buffer& operator=(untyped_dynamic_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name State
		/// @{

		using graphics_buffer::valid;

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the vertex buffer is empty.
		/// @return `true` if thevertex buffer is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// Gets the size of the vertex buffer contents.
		/// @return Size of the vertex buffer in bytes.
		[[nodiscard]] usize size() const noexcept;

		/// Gets the capacity of the vertex buffer.
		/// @return Capacity of the vertex buffer in bytes.
		[[nodiscard]] usize capacity() const noexcept;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the size of the vertex buffer to 0.
		void clear() noexcept;

		/// Clears the buffer and resizes it, potentially resizing it.
		/// @param size New size of the buffer in bytes.
		void resize(usize size);

		/// Clears the buffer and guarantees a certain capacity for it.
		/// @param capacity New capacity of the buffer in bytes.
		void reserve(usize capacity);

		/// Sets the contents of the buffer, potentially reallocating it.
		/// @param data Data to copy into the buffer.
		void set(std::span<const std::byte> data);

		/// Sets a region of the buffer.
		/// @param offset Starting byte offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre `offset + data.size()` must be less than or equal to the size of the buffer.
		void set_region(usize offset, std::span<const std::byte> data) noexcept;

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
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using graphics_buffer::id;

		/// @}
		/// @endcond

	  private:
		/// Used size of the buffer in bytes.
		usize m_size;

		/// Capacity of the buffer in bytes.
		usize m_capacity;
	};
} // namespace tr

//

/// Untyped dynamic vertex buffer formatter.
template <>
struct std::formatter<tr::untyped_dynamic_vertex_buffer>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid untyped dynamic vertex buffer format specification."};
		}
		return context.begin();
	}

	/// Formats an untyped dynamic vertex buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Vertex buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_dynamic_vertex_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, VBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped dynamic vertex buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};