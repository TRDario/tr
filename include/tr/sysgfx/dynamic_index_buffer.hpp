/// @file
/// @brief Provides `tr::dynamic_index_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Dynamic index buffer.
	/// @details
	/// `tr::dynamic_index_buffer` represents a graphics buffer used to store index data. Unlike `tr::static_index_buffer`, it allows for
	/// dynamic resizing and updating of its contents, making it suitable for scenarios where the index data changes often at runtime.
	///
	/// Strictly speaking, instances of `tr::dynamic_index_buffer` are only containers for these underlying buffer objects. This means, for
	/// example, that setting a dynamic index buffer on a graphics context does not set the literal `tr::dynamic_index_buffer` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::dynamic_index_buffer`, that value will still be set on the context. If the instance is overriden with a new value, the old
	/// value is destroyed and the graphics context will no longer have a set index buffer.
	///
	/// Every instance of `tr::dynamic_index_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::dynamic_index_buffer` are left in a special 'invalid' state. Invalid `tr::dynamic_index_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::dynamic_index_buffer` instances may be labeled and are formattable. Example format output: `"My index buffer" (OpenGL ID: 5)`.
	class dynamic_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an empty dynamic index buffer.
		/// @param context Graphics context to create the buffer on.
		[[nodiscard]] explicit dynamic_index_buffer(graphics_context& context) noexcept;

		/// Dynamic index buffers are not copyable.
		dynamic_index_buffer(const dynamic_index_buffer&) = delete;

		/// Moves a dynamic index buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Dynamic index buffer to move.
		[[nodiscard]] dynamic_index_buffer(dynamic_index_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Dynamic index buffers are not copyable.
		dynamic_index_buffer& operator=(const dynamic_index_buffer&) = delete;

		/// Moves a dynamic index buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Dynamic index buffer to move.
		/// @return Reference to `*this`.
		dynamic_index_buffer& operator=(dynamic_index_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the index buffer is empty.
		/// @return `true` if the buffer is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// Gets the size of the index buffer contents.
		/// @return Size of the index buffer in elements.
		[[nodiscard]] usize size() const noexcept;

		/// Gets the capacity of the index buffer.
		/// @return Capacity of the index buffer in elements.
		[[nodiscard]] usize capacity() const noexcept;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the size of the index buffer to 0.
		void clear() noexcept;

		/// Clears the buffer and resizes it, potentially reallocating in the process.
		/// @param size New size of the buffer in elements.
		void resize(usize size);

		/// Clears the buffer and guarantees a certain capacity for it.
		/// @param capacity New capacity of the buffer in elements.
		void reserve(usize capacity);

		/// Sets the contents of the buffer, potentially reallocating in the process.
		/// @param data Data to copy into the buffer.
		void set(std::span<const u16> data);

		/// Sets a region of the buffer.
		/// @param offset Starting element offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre `offset + data.size()` must be less than or equal to the size of the buffer.
		void set_region(usize offset, std::span<const u16> data) noexcept;

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
		/// Used size of the buffer in elements.
		usize m_size;

		/// Capacity of the buffer in elements.
		usize m_capacity;
	};
} // namespace tr

//

/// Dynamic index buffer formatter.
template <>
struct std::formatter<tr::dynamic_index_buffer>
{
	/// Parses the format specification.
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

	/// Formats a dynamic index buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Dynamic index buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::dynamic_index_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid dynamic index buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};