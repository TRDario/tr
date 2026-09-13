/// @file
/// @brief Provides `tr::dynamic_index_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Dynamic index buffer class.
	class dynamic_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		using graphics_buffer::graphics_buffer;

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
		usize m_size{0};

		/// Capacity of the buffer in elements.
		usize m_capacity{0};
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