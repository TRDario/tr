/// @file
/// @brief Provides `tr::dynamic_index_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/sysgfx/mapped_graphics_buffer_span.hpp>

//

namespace tr
{
	/// @brief Owning container for a dynamic index buffer.
	/// @details
	/// `tr::dynamic_index_buffer` represents a graphics buffer used to store index data. Unlike `tr::static_index_buffer`, it allows for
	/// dynamic resizing and updating of its contents, making it suitable for scenarios where the index data changes often at runtime.
	///
	/// It's worth remembering that instances of `tr::dynamic_index_buffer` are only containers for these underlying buffer objects. This
	/// means, for example, that setting a dynamic index buffer on a graphics context does not set the literal `tr::dynamic_index_buffer`
	/// object at a specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::dynamic_index_buffer`, that value will still be set on the context. If the instance is overriden with a new value, the old
	/// value is destroyed and the graphics context will no longer have a set index buffer.
	///
	/// Every instance of `tr::dynamic_index_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::dynamic_index_buffer` are left in a special 'invalid' state. Invalid `tr::dynamic_index_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator
	/// bool()`.
	///
	/// `tr::dynamic_index_buffer` instances may be labeled and are formattable. Example format output: `"My index buffer" (ID: 3, IBO: 5)`.
	class dynamic_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty dynamic index buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param label Label of the buffer.
		[[nodiscard]] dynamic_index_buffer(graphics_context& context, std::string_view label = {}) noexcept;

		/// @brief Dynamic index buffers are not copyable.
		dynamic_index_buffer(const dynamic_index_buffer&) = delete;

		/// @brief Dynamic index buffers are not copyable.
		dynamic_index_buffer& operator=(const dynamic_index_buffer&) = delete;

		/// @brief Constructs a dynamic index buffer by transferring ownership from another `tr::dynamic_index_buffer`.
		/// @param rhs Dynamic index buffer to move from.
		/// @par Invalidation
		/// If the index buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] dynamic_index_buffer(dynamic_index_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held dynamic index buffer and replaces it with one moved from another `tr::dynamic_index_buffer`.
		/// @param rhs Dynamic index buffer to move from.
		/// @par Invalidation
		/// If the index buffer previously held by `*this` was set on the associated graphics context, it is unset.
		///
		/// If the index buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		dynamic_index_buffer& operator=(dynamic_index_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the size of the index buffer to 0.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		void clear() noexcept;

		/// @brief Clears the buffer and guarantees a certain capacity for it.
		/// @param capacity New capacity of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Invalidation
		/// If `%capacity` exceeds the capacity of the buffer and `*this` is set on the associated graphics context, it is unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void reserve(usize capacity);

		/// @brief Clears the buffer and resizes it, potentially reallocating in the process.
		/// @param size New size of the buffer.
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
		void set(std::span<const u16> data);

		/// @brief Sets a region of the buffer.
		/// @param offset Starting offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// The set region must lie entirely within the buffer.
		void set_region(usize offset, std::span<const u16> data) noexcept;

		/// @brief Maps the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Write-only map of the buffer.
		[[nodiscard]] mapped_graphics_buffer_span<u16> map();

		using graphics_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using graphics_buffer::operator bool;

		using graphics_buffer::valid;

		using graphics_buffer::context;

		/// @brief Checks whether the index buffer is empty.
		/// @pre The buffer must be in a valid state.
		/// @return `true` if the buffer is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// @brief Returns the size of the index buffer contents.
		/// @pre The buffer must be in a valid state.
		/// @return Size of the index buffer.
		[[nodiscard]] usize size() const noexcept;

		/// @brief Returns the capacity of the index buffer.
		/// @pre The buffer must be in a valid state.
		/// @return Capacity of the index buffer.
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
		/// @brief Used size of the buffer.
		usize m_size;

		/// @brief Capacity of the buffer.
		usize m_capacity;
	};
} // namespace tr

//

/// @brief Dynamic index buffer formatter.
template <>
struct std::formatter<tr::dynamic_index_buffer>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid dynamic index buffer format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a dynamic index buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Dynamic index buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::dynamic_index_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, IBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid dynamic index buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};