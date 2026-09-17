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
	/// @details
	/// `tr::dynamic_vertex_buffer` represents a graphics buffer used to store vertex data. Unlike
	/// `tr::static_vertex_buffer`, it allows for dynamic resizing and updating of its contents, making it suitable for scenarios where the
	/// vertex data changes often at runtime. It is the typed equivalent of `tr::untyped_static_vertex_buffer`.
	///
	/// Strictly speaking, instances of `tr::dynamic_vertex_buffer` are only containers for these underlying buffer objects. This means, for
	/// example, that setting a dynamic vertex buffer on a graphics context does not set the literal `tr::dynamic_vertex_buffer` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::dynamic_vertex_buffer`, that value will still be set on the context. If the instance is overriden with a new value, the old
	/// value is destroyed and the graphics context will no longer have a set vertex buffer.
	///
	/// Every instance of `tr::dynamic_vertex_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::dynamic_vertex_buffer` are left in a special 'invalid' state. Invalid `tr::dynamic_vertex_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::dynamic_vertex_buffer` instances may be labeled and are formattable. Example format output: `"My vertex buffer" (OpenGL ID:
	/// 5)`.
	/// @tparam Element Type of the elements of the buffer.
	template <standard_layout Element>
	class dynamic_vertex_buffer : private untyped_dynamic_vertex_buffer
	{
	  public:
		/// Value type used by the buffer.
		using value_type = Element;

		/// @name Constructors and destructors
		/// @{

		/// Creates an empty dynamic vertex buffer.
		/// @param context Graphics context to create the buffer on.
		[[nodiscard]] explicit dynamic_vertex_buffer(graphics_context& context) noexcept
			: untyped_dynamic_vertex_buffer{context}
		{
		}

		/// Dynamic vertex buffers are not copyable.
		dynamic_vertex_buffer(const dynamic_vertex_buffer&) = delete;

		/// Moves a dynamic vertex buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Dynamic vertex buffer to move.
		[[nodiscard]] dynamic_vertex_buffer(dynamic_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Dynamic vertex buffers are not copyable.
		dynamic_vertex_buffer& operator=(const dynamic_vertex_buffer&) = delete;

		/// Moves a dynamic vertex buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Dynamic vertex buffer to move.
		/// @return Reference to `*this`.
		dynamic_vertex_buffer& operator=(dynamic_vertex_buffer&& rhs) noexcept = default;

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