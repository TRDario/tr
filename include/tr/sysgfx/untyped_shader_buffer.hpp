/// @file
/// @brief Provides `tr::untyped_shader_buffer`.

#pragma once
#include <tr/sysgfx/access.hpp>
#include <tr/sysgfx/graphics_buffer.hpp>

namespace tr
{
	class mapped_untyped_graphics_buffer_span;
}

//

namespace tr
{
	/// @brief Owning container for an untyped shader-accessible GPU buffer.
	/// @details
	/// `tr::untyped_shader_buffer` represents a graphics buffer used to store data for shader access. Shader buffers consist of a
	/// fixed-size header block and a resizable array block; the array block can be resized up to a maximum capacity.
	/// `tr::untyped_shader_buffer` does not enforce any specific data format and may be used when flexibility is needed, while
	/// `tr::shader_buffer` models a more typical uniformly typed buffer. Shader buffers are allocated once and cannot be resized beyond
	/// their maximum capacity. Shader buffers can be set or mapped for direct access by the CPU; the map type of any given buffer is
	/// specified during construction and cannot be changed afterwards.
	///
	/// It's worth remembering that instances of `tr::untyped_shader_buffer` are only containers for these underlying buffer objects. This
	/// means, for example, that setting an untyped shader buffer on a shader does not set the literal `tr::untyped_shader_buffer` object at
	/// a specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::untyped_shader_buffer`, that value will still be set on the shader. If the instance is overriden with a new value, the old
	/// value is destroyed and the shader will no longer have a set buffer.
	///
	/// Every instance of `tr::untyped_shader_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::untyped_shader_buffer` are left in a special 'invalid' state. Invalid `tr::untyped_shader_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator
	/// bool()`.
	///
	/// `tr::untyped_shader_buffer` instances may be labeled and are formattable. Example format output: `"My buffer" (ID: 3, SSBO: 5)`.
	class untyped_shader_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an uninitialized shader buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param header_size Size of the fixed header block in bytes.
		/// @param capacity Maximum capacity of the dynamic array in bytes.
		/// @param map_access Buffer map access type.
		/// @param label Label of the buffer.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] untyped_shader_buffer(graphics_context& context, usize header_size, usize capacity,
											access map_access = access::write_only, std::string_view label = {});

		/// @brief Untyped shader buffers are not copyable.
		untyped_shader_buffer(const untyped_shader_buffer&) = delete;

		/// @brief Untyped shader buffers are not copyable.
		untyped_shader_buffer& operator=(const untyped_shader_buffer&) = delete;

		/// @brief Constructs an untyped shader buffer by transferring ownership from another `tr::untyped_shader_buffer`.
		/// @param rhs Untyped shader buffer to move from.
		/// @par Invalidation
		/// If the shader buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] untyped_shader_buffer(untyped_shader_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held untyped shader buffer and replaces it with one moved from another
		/// `tr::untyped_shader_buffer`.
		/// @param rhs Untyped shader buffer to move from.
		/// @par Invalidation
		/// If the shader buffer previously held by `*this` is set on any shaders, it is unset.
		///
		/// If the shader buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		untyped_shader_buffer& operator=(untyped_shader_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the data of the header.
		/// @param data Data to set the header with.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// `data` must be of the same size as the buffer's header.
		void set_header(std::span<const std::byte> data) noexcept;

		/// @brief Sets the data of the dynamic array.
		/// @param data Data to set the dynamic array with.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// The size of `data` must be smaller than or equal to the capacity of the buffer's dynamic array.
		/// @par Invalidation
		/// If `*this` is set on any shaders, it is unset.
		void set_array(std::span<const std::byte> data) noexcept;

		/// @brief Resizes the dynamic array.
		/// @param size Size of the dynamic array in bytes.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// `size` must be less than or equal to the capacity of the buffer's dynamic array.
		/// @par Invalidation
		/// If `*this` is set on any shaders, it is unset.
		void resize_array(usize size) noexcept;

		/// @brief Maps the fixed header of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Map of the fixed header of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map_header();

		/// @brief Maps the dynamic array of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Map of the dynamic array of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map_array();

		/// @brief Maps the entire buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Map of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map();

		using graphics_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using graphics_buffer::operator bool;

		using graphics_buffer::valid;

		using graphics_buffer::context;

		/// @brief Returns the size of the fixed header block.
		/// @pre The buffer must be in a valid state.
		/// @return Size of the fixed header block in bytes.
		[[nodiscard]] usize header_size() const noexcept;

		/// @brief Returns the size of the dynamic array.
		/// @pre The buffer must be in a valid state.
		/// @return Size of the dynamic array in bytes.
		[[nodiscard]] usize array_size() const noexcept;

		/// @brief Returns the maximum capacity of the dynamic array.
		/// @pre The buffer must be in a valid state.
		/// @return Maximum capacity of the dynamic array in bytes.
		[[nodiscard]] usize array_capacity() const noexcept;

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
		/// @brief Buffer map access type.
		access m_map_access;

		/// @brief Size of the header in bytes.
		usize m_header_size;

		/// @brief Size of the array in bytes.
		usize m_array_size;

		/// @brief Capacity of the array in bytes.
		usize m_array_capacity;

		//

		/// @brief Maps a range of the buffer.
		/// @param offset Starting offset of the map in bytes.
		/// @param size Size of the map in bytes.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Buffer range map.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map_range(usize offset, usize size);
	};
} // namespace tr

//

/// @brief Untyped shader buffer formatter.
template <>
struct std::formatter<tr::untyped_shader_buffer>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats an untyped shader buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Shader buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_shader_buffer& buffer, FormatContext& context) const
	{
		if (buffer) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, SSBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped shader buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};