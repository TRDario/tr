/// @file
/// @brief Provides `tr::untyped_shader_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/sysgfx/graphics_buffer_map_access.hpp>

namespace tr
{
	class mapped_untyped_graphics_buffer_span;
}

//

namespace tr
{
	/// Untyped shader-accessible GPU buffer.
	/// @details Shader buffers are divided into a fixed header block segment and a resizable array segment.
	class untyped_shader_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Allocates an uninitialized shader buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param header_size Size of the fixed header block in bytes.
		/// @param capacity Maximum capacity of the dyuamic array in bytes.
		/// @param map_type Type of map to create when mapping the buffer.
		[[nodiscard]] untyped_shader_buffer(graphics_context& context, usize header_size, usize capacity,
											graphics_buffer_map_access map_type = graphics_buffer_map_access::write_only);

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

		/// Gets the size of the fixed header block.
		/// @return Size of the fixed header block in bytes.
		[[nodiscard]] usize header_size() const noexcept;

		/// Gets the size of the dynamic array.
		/// @return Size of the dynamic array in bytes.
		[[nodiscard]] usize array_size() const noexcept;

		/// Gets the maximum capacity of the dynamic array.
		/// @return Maximum capacity of the dynamic array in bytes.
		[[nodiscard]] usize array_capacity() const noexcept;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the header.
		/// @param data Data to set the header with.
		/// @pre `data` must be of the same size as the buffer's header.
		void set_header(std::span<const std::byte> data) noexcept;

		/// Sets the data of the dynamic array.
		/// @param data Data to set the dynamic array with.
		/// @pre `data` must be smaller than or equal to the capacity of the buffer's dynamic array.
		void set_array(std::span<const std::byte> data) noexcept;

		/// Resizes the dynamic array.
		/// @param size Size of the dynamic array in bytes.
		/// @pre `size` must be less than or equal to the capacity of the buffer's dynamic array.
		void resize_array(usize size) noexcept;

		/// @}
		/// @name Mapping
		/// @{

		/// Gets whether the buffer is mapped.
		/// @return `true` if the buffer is mapped, `false` otherwise.
		[[nodiscard]] bool mapped() const noexcept;

		/// Maps the fixed header of the buffer.
		/// @return Map of the fixed header of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map_header();

		/// Maps the dynamic array of the buffer.
		/// @return Map of the dynamic array of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map_array();

		/// Maps the entire buffer.
		/// @return Map of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map();

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
		// Map type of the buffer.
		graphics_buffer_map_access m_map_type;

		// Size of the header.
		usize m_header_size;

		// Current size of the array.
		usize m_array_size;

		// Capacity of the array.
		usize m_array_capacity;

		//

		/// Maps a range of the buffer.
		/// @param offset Starting offset of the map in bytes.
		/// @param size Size of the map in bytes.
		/// @return Buffer range map.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map_range(usize offset, usize size);
	};
} // namespace tr

//

/// Untyped shader buffer formatter.
template <>
struct std::formatter<tr::untyped_shader_buffer>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid untyped shader buffer format specification."};
		}
		return context.begin();
	}

	/// Formats an untyped shader buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Shader buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_shader_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped shader buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};