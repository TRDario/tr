/// @file
/// @brief Provides `tr::dynamic_vertex_buffer`.

#pragma once
#include <tr/sysgfx/untyped_dynamic_vertex_buffer.hpp>
#include <tr/utility/concepts.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Typed dynamic vertex buffer class.
	/// @tparam Element Type of the elements of the buffer.
	template <standard_layout Element>
	class dynamic_vertex_buffer : private untyped_dynamic_vertex_buffer
	{
	  public:
		/// Value type used by the buffer.
		using value_type = Element;

		/// @name Constructors
		/// @{

		using untyped_dynamic_vertex_buffer::untyped_dynamic_vertex_buffer;

		/// @}
		/// @name Context
		/// @{

		using untyped_dynamic_vertex_buffer::context;

		/// @}
		/// @name State
		/// @{

		using untyped_dynamic_vertex_buffer::valid;

		/// @}
		/// @name Size
		/// @{

		using untyped_dynamic_vertex_buffer::empty;

		/// Gets the size of the vertex buffer contents.
		/// @return Size of the vertex buffer in elements.
		[[nodiscard]] usize size() const noexcept
		{
			return untyped_dynamic_vertex_buffer::size() / sizeof(Element);
		}

		/// Gets the capacity of the vertex buffer.
		/// @return Capacity of the vertex buffer in elements.
		[[nodiscard]] usize capacity() const noexcept
		{
			return untyped_dynamic_vertex_buffer::capacity() / sizeof(Element);
		}

		/// @}
		/// @name Setting
		/// @{

		using untyped_dynamic_vertex_buffer::clear;

		/// Clears the buffer and resizes it, potentially reallocating it.
		/// @param size New size of the buffer in elements.
		void resize(usize size)
		{
			untyped_dynamic_vertex_buffer::resize(size * sizeof(Element));
		}

		/// Clears the buffer and guarantees a certain capacity for it.
		/// @param capacity New capacity of the buffer in elements.
		void reserve(usize capacity)
		{
			untyped_dynamic_vertex_buffer::reserve(capacity * sizeof(Element));
		}

		/// Sets the contents of the buffer, potentially reallocating it.
		/// @param data Data to copy into the buffer.
		void set(std::span<const Element> data)
		{
			untyped_dynamic_vertex_buffer::set(std::as_bytes(data));
		}

		/// Sets a region of the buffer.
		/// @param offset Starting element offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre `offset + data.size()` must be less than or equal to the size of the buffer.
		void set_region(usize offset, std::span<const Element> data) noexcept
		{
			untyped_dynamic_vertex_buffer::set_region(offset * sizeof(Element), std::as_bytes(data));
		}

		/// @}
		/// @name Label
		/// @{

		using untyped_dynamic_vertex_buffer::label;

		using untyped_dynamic_vertex_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using untyped_dynamic_vertex_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using untyped_dynamic_vertex_buffer::id;

		/// @}
		/// @endcond
#endif
	};
} // namespace tr

//

/// Dynamic vertex buffer formatter.
template <tr::standard_layout Element>
struct std::formatter<tr::dynamic_vertex_buffer<Element>>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid dynamic vertex buffer format specification."};
		}
		return context.begin();
	}

	/// Formats a dynamic vertex buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Vertex buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::dynamic_vertex_buffer<Element>& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} dynamic vertex buffer at {}>", tr::type_name<Element>(),
								  static_cast<const void*>(&buffer));
		}
	}
};