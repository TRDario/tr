/// @file
/// @brief Provides `tr::untyped_dynamic_vertex_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>

namespace tr
{
	class mapped_untyped_graphics_buffer_span;
}

//

namespace tr
{
	/// @brief Owning container for an untyped dynamic vertex buffer.
	/// @details
	/// `tr::untyped_dynamic_vertex_buffer` represents a graphics buffer used to store vertex data. Unlike
	/// `tr::untyped_static_vertex_buffer`, it allows for dynamic resizing and updating of its contents, making it suitable for scenarios
	/// where the vertex data changes often at runtime. `tr::untyped_dynamic_vertex_buffer` does not enforce any specific data format and
	/// may be used when flexibility is needed, while `tr::dynamic_vertex_buffer` models a buffer holding vertices of a specific type.
	///
	/// It's worth remembering that instances of `tr::untyped_dynamic_vertex_buffer` are only containers for these underlying buffer
	/// objects. This means, for example, that setting a dynamic vertex buffer on a graphics context does not set the literal
	/// `tr::untyped_dynamic_vertex_buffer` object at a specific location in memory, but rather the value it contains. If the value is moved
	/// to a different instance of `tr::untyped_dynamic_vertex_buffer`, that value will still be set on the context. If the instance is
	/// overriden with a new value, the old value is destroyed and the graphics context will no longer have a set vertex buffer.
	///
	/// Every instance of `tr::untyped_dynamic_vertex_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::untyped_dynamic_vertex_buffer` are left in a special 'invalid' state. Invalid
	/// `tr::untyped_dynamic_vertex_buffer` instances may not be interacted with besides moving a new value into them and checking for
	/// validity using `valid()` or `operator bool()`.
	///
	/// `tr::untyped_dynamic_vertex_buffer` instances may be labeled and are formattable. Example format output: `"My vertex buffer" (ID: 3,
	/// VBO: 5)`.
	class untyped_dynamic_vertex_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty untyped dynamic index buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param label Label of the buffer.
		[[nodiscard]] explicit untyped_dynamic_vertex_buffer(graphics_context& context, std::string_view label = {}) noexcept;

		/// @brief Untyped dynamic index buffers are not copyable.
		untyped_dynamic_vertex_buffer(const untyped_dynamic_vertex_buffer&) = delete;

		/// @brief Untyped dynamic index buffers are not copyable.
		untyped_dynamic_vertex_buffer& operator=(const untyped_dynamic_vertex_buffer&) = delete;

		/// @brief Constructs an untyped dynamic vertex buffer by transferring ownership from another `tr::untyped_dynamic_vertex_buffer`.
		/// @param rhs Untyped dynamic vertex buffer to move from.
		/// @par Invalidation
		/// If the vertex buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] untyped_dynamic_vertex_buffer(untyped_dynamic_vertex_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held untyped dynamic vertex buffer and replaces it with one moved from another
		/// `tr::untyped_dynamic_vertex_buffer`.
		/// @param rhs Untyped dynamic vertex buffer to move from.
		/// @par Invalidation
		/// If the vertex buffer previously held by `*this` was set on the associated graphics context, it is unset.
		///
		/// If the vertex buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		untyped_dynamic_vertex_buffer& operator=(untyped_dynamic_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the size of the vertex buffer to 0.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		void clear() noexcept;

		/// @brief Clears the buffer and resizes it, potentially reallocating in the process.
		/// @param capacity New capacity of the buffer in bytes.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Invalidation
		/// If `%capacity` exceeds the capacity of the buffer and `*this` is set on the associated graphics context, it is unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void reserve(usize capacity);

		/// @brief Clears the buffer and guarantees a certain capacity for it.
		/// @param size New size of the buffer in bytes.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Invalidation
		/// If `%size` exceeds the capacity of the buffer and `*this` is set on the associated graphics context, it is unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void resize(usize size);

		/// @brief Sets the contents of the buffer, potentially reallocating in the process.
		/// @param data Data to copy into the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Invalidation
		/// If the size of `data` exceeds the capacity of the buffer and `*this` is set on the associated graphics context, it is unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set(std::span<const std::byte> data);

		/// @brief Sets a region of the buffer.
		/// @param offset Starting byte offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// The set region must lie entirely within the buffer.
		void set_region(usize offset, std::span<const std::byte> data) noexcept;

		/// @brief Maps the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Write-only map of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map();

		using graphics_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using graphics_buffer::operator bool;

		using graphics_buffer::valid;

		using graphics_buffer::context;

		/// @brief Checks whether the vertex buffer is empty.
		/// @pre The buffer must be in a valid state.
		/// @return `true` if thevertex buffer is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// @brief Returns the size of the vertex buffer contents.
		/// @pre The buffer must be in a valid state.
		/// @return Size of the vertex buffer in bytes.
		[[nodiscard]] usize size() const noexcept;

		/// @brief Returns the capacity of the vertex buffer.
		/// @pre The buffer must be in a valid state.
		/// @return Capacity of the vertex buffer in bytes.
		[[nodiscard]] usize capacity() const noexcept;

		using graphics_buffer::mapped;

		using graphics_buffer::label;

		/// @cond gl_interop
		using graphics_buffer::handle;
		/// @endcond

		/// @cond implementation_details
		using graphics_buffer::id;
		/// @endcond

		/// @}

	  private:
		/// @brief Size of the buffer in bytes.
		usize m_size;

		/// @brief Capacity of the buffer in bytes.
		usize m_capacity;
	};
} // namespace tr

//

/// @brief Untyped dynamic vertex buffer formatter.
template <>
struct std::formatter<tr::untyped_dynamic_vertex_buffer>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats an untyped dynamic vertex buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Vertex buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_dynamic_vertex_buffer& buffer, FormatContext& context) const
	{
		if (buffer) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, VBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped dynamic vertex buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};