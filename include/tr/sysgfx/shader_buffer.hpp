/// @file
/// @brief Provides `tr::shader_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer_map_access.hpp>
#include <tr/sysgfx/mapped_graphics_buffer_object.hpp>
#include <tr/sysgfx/mapped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_shader_buffer.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Shader buffer with a typed header and array.
	/// @tparam Header Type of the header object stored at the front of the buffer.
	/// @tparam ArrayElement Type of the buffer dynamic array elements.
	template <typename Header, typename ArrayElement>
	class shader_buffer : private untyped_shader_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		// Allocates an uninitialized shader buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param capacity Maximum capacity of the buffer array in elements.
		/// @param map_type Type of map to create when mapping the buffer.
		[[nodiscard]] shader_buffer(graphics_context& context, usize capacity,
									graphics_buffer_map_access map_type = graphics_buffer_map_access::write_only)
			: untyped_shader_buffer{context, sizeof(Header), capacity * sizeof(ArrayElement), map_type}
		{
		}

		/// @}
		/// @name Context
		/// @{

		using untyped_shader_buffer::context;

		/// @}
		/// @name State
		/// @{

		using untyped_shader_buffer::valid;

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the dynamic array.
		/// @return Size of the dynamic array in elements.
		[[nodiscard]] usize array_size() const noexcept
		{
			return untyped_shader_buffer::array_size() / sizeof(ArrayElement);
		}

		/// Gets the maximum capacity of the dynamic array.
		/// @return Maximum capacity of the dynamic array in elements.
		[[nodiscard]] usize array_capacity() const noexcept
		{
			return untyped_shader_buffer::array_capacity() / sizeof(ArrayElement);
		}

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the header.
		/// @param header Header object to copy into the buffer.
		void set_header(const Header& header) noexcept
		{
			untyped_shader_buffer::set_header(as_bytes(header));
		}

		/// Sets the data of the dynamic array.
		/// @param data Data to set the dynamic array to.
		void set_array(std::span<const ArrayElement> data) noexcept
		{
			untyped_shader_buffer::set_array(std::as_bytes(data));
		}

		/// Resizes the dynamic array.
		/// @param size Size of the array in elements.
		/// @pre `size` must be less than or equal to the capacity of the array.
		void resize_array(usize size) noexcept
		{
			untyped_shader_buffer::resize_array(size * sizeof(ArrayElement));
		}

		/// @}
		/// @name Mapping
		/// @{

		using untyped_shader_buffer::mapped;

		/// Maps the fixed header of the buffer.
		/// @return Map of the fixed header of the buffer.
		[[nodiscard]] mapped_graphics_buffer_object<Header> map_header()
		{
			return mapped_graphics_buffer_object<Header>{untyped_shader_buffer::map_header()};
		}

		/// Maps the dynamic array of the buffer.
		/// @return Map of the dynamic array of the buffer.
		[[nodiscard]] mapped_graphics_buffer_span<ArrayElement> map_array()
		{
			return mapped_graphics_buffer_span<ArrayElement>{untyped_shader_buffer::map_array()};
		}

		/// @}
		/// @name Label
		/// @{

		using untyped_shader_buffer::label;

		using untyped_shader_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using untyped_shader_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using untyped_shader_buffer::id;

		/// @}
		/// @endcond
#endif
	};
} // namespace tr

//

/// Shader buffer formatter.
template <typename Header, typename ArrayElement>
struct std::formatter<tr::shader_buffer<Header, ArrayElement>>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid shader buffer format specification."};
		}
		return context.begin();
	}

	/// Formats a shader buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Shader buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::shader_buffer<Header, ArrayElement>& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {}/{} shader buffer at {}>", tr::type_name<Header>(),
								  tr::type_name<ArrayElement>(), static_cast<const void*>(&buffer));
		}
	}
};