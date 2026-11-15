/// @file
/// @brief Provides `tr::dynamic_vertex_buffer`.

#pragma once
#include <tr/sysgfx/mapped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_dynamic_vertex_buffer.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// @brief Owning container for a typed dynamic vertex buffer.
	/// @details
	/// `tr::dynamic_vertex_buffer` represents a graphics buffer used to store vertex data. Unlike
	/// `tr::static_vertex_buffer`, it allows for dynamic resizing and updating of its contents, making it suitable for scenarios where the
	/// vertex data changes often at runtime. It is the typed equivalent of `tr::untyped_dynamic_vertex_buffer`.
	///
	/// It's worth remembering that instances of `tr::dynamic_vertex_buffer` are only containers for these underlying buffer objects. This
	/// means, for example, that setting a dynamic vertex buffer on a graphics context does not set the literal `tr::dynamic_vertex_buffer`
	/// object at a specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::dynamic_vertex_buffer`, that value will still be set on the context. If the instance is overriden with a new value, the old
	/// value is destroyed and the graphics context will no longer have a set vertex buffer.
	///
	/// Every instance of `tr::dynamic_vertex_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::dynamic_vertex_buffer` are left in a special 'invalid' state. Invalid `tr::dynamic_vertex_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator
	/// bool()`.
	///
	/// `tr::dynamic_vertex_buffer` instances may be labeled and are formattable. Example format output: `"My vertex buffer" (ID: 3, VBO:
	/// 5)`.
	/// @tparam Element Type of the elements of the buffer.
	template <typename Element>
		requires std::is_trivially_copyable_v<Element> && std::is_trivially_destructible_v<Element>
	class dynamic_vertex_buffer : private untyped_dynamic_vertex_buffer
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Value type used by the buffer.
		using value_type = Element;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty dynamic vertex buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param label Label of the buffer.
		[[nodiscard]] dynamic_vertex_buffer(graphics_context& context, std::string_view label = {}) noexcept
			: untyped_dynamic_vertex_buffer{context, label}
		{
		}

		/// @brief Dynamic vertex buffers are not copyable.
		dynamic_vertex_buffer(const dynamic_vertex_buffer&) = delete;

		/// @brief Dynamic vertex buffers are not copyable.
		dynamic_vertex_buffer& operator=(const dynamic_vertex_buffer&) = delete;

		/// @brief Constructs a dynamic vertex buffer by transferring ownership from another `tr::dynamic_vertex_buffer`.
		/// @param rhs Dynamic vertex buffer to move from.
		/// @par Invalidation
		/// If the vertex buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] dynamic_vertex_buffer(dynamic_vertex_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held dynamic vertex buffer and replaces it with one moved from another
		/// `tr::dynamic_vertex_buffer`.
		/// @param rhs Dynamic vertex buffer to move from.
		/// @par Invalidation
		/// If the vertex buffer previously held by `*this` was set on the associated graphics context, it is unset.
		///
		/// If the vertex buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		dynamic_vertex_buffer& operator=(dynamic_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		using untyped_dynamic_vertex_buffer::clear;

		/// @brief Clears the buffer and resizes it, potentially reallocating in the process.
		/// @param capacity New capacity of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Invalidation
		/// If `%capacity` exceeds the capacity of the buffer and `*this` is set on the associated graphics context, it is unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void reserve(usize capacity)
		{
			untyped_dynamic_vertex_buffer::reserve(capacity * sizeof(Element));
		}

		/// @brief Clears the buffer and guarantees a certain capacity for it.
		/// @param size New size of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Invalidation
		/// If `%size` exceeds the capacity of the buffer and `*this` is set on the associated graphics context, it is unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void resize(usize size)
		{
			untyped_dynamic_vertex_buffer::resize(size * sizeof(Element));
		}

		/// @brief Sets the contents of the buffer, potentially reallocating in the process.
		/// @param data Data to copy into the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Invalidation
		/// If the size of `data` exceeds the capacity of the buffer and `*this` is set on the associated graphics context, it is unset.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set(std::span<const Element> data)
		{
			untyped_dynamic_vertex_buffer::set(std::as_bytes(data));
		}

		/// @brief Sets a region of the buffer.
		/// @param offset Starting offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// The set region must lie entirely within the buffer.
		void set_region(usize offset, std::span<const Element> data) noexcept
		{
			untyped_dynamic_vertex_buffer::set_region(offset * sizeof(Element), std::as_bytes(data));
		}

		/// @brief Maps the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Write-only map of the buffer.
		[[nodiscard]] mapped_graphics_buffer_span<Element> map()
		{
			return mapped_graphics_buffer_span<Element>{untyped_dynamic_vertex_buffer::map()};
		}

		using untyped_dynamic_vertex_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using untyped_dynamic_vertex_buffer::operator bool;

		using untyped_dynamic_vertex_buffer::valid;

		using untyped_dynamic_vertex_buffer::context;

		using untyped_dynamic_vertex_buffer::empty;

		/// @brief Returns the size of the vertex buffer contents.
		/// @pre The buffer must be in a valid state.
		/// @return Size of the vertex buffer.
		[[nodiscard]] usize size() const noexcept
		{
			return untyped_dynamic_vertex_buffer::size() / sizeof(Element);
		}

		/// @brief Returns the capacity of the vertex buffer.
		/// @pre The buffer must be in a valid state.
		/// @return Capacity of the vertex buffer.
		[[nodiscard]] usize capacity() const noexcept
		{
			return untyped_dynamic_vertex_buffer::capacity() / sizeof(Element);
		}

		using untyped_dynamic_vertex_buffer::mapped;

		using untyped_dynamic_vertex_buffer::label;

		/// @cond gl_interop
		using untyped_dynamic_vertex_buffer::handle;
		/// @endcond

		/// @cond implementation_details
		using untyped_dynamic_vertex_buffer::id;
		/// @endcond

		/// @}
	};
} // namespace tr

//

/// @brief Dynamic vertex buffer formatter.
template <typename Element>
struct std::formatter<tr::dynamic_vertex_buffer<Element>>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a dynamic vertex buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Vertex buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::dynamic_vertex_buffer<Element>& buffer, FormatContext& context) const
	{
		if (buffer) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, VBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} dynamic vertex buffer at {}>", tr::type_name<Element>(),
								  static_cast<const void*>(&buffer));
		}
	}
};