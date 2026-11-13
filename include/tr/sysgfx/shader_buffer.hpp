/// @file
/// @brief Provides `tr::shader_buffer`.

#pragma once
#include <tr/sysgfx/access.hpp>
#include <tr/sysgfx/mapped_graphics_buffer_object.hpp>
#include <tr/sysgfx/mapped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_shader_buffer.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// @brief Owning container for a typed shader-accessible GPU buffer.
	/// @details
	/// `tr::shader_buffer` represents the typed equivalent of `tr::untyped_shader_buffer`, a graphics buffer used to store data for
	/// shader access. Shader buffers consist of a fixed-size header block and a resizable array block; the array block can be resized up to
	/// a maximum capacity. Shader buffers are allocated once and cannot be resized beyond their maximum capacity. Shader buffers can be set
	/// or mapped for direct access by the CPU; the map type of any given buffer is specified during construction and cannot be changed
	/// afterwards.
	///
	/// It's worth remembering that instances of `tr::shader_buffer` are only containers for these underlying buffer objects. This means,
	/// for example, that setting a shader buffer on a shader does not set the literal `tr::shader_buffer` object at a specific location in
	/// memory, but rather the value it contains. If the value is moved to a different instance of `tr::shader_buffer`, that value will
	/// still be set on the shader. If the instance is overriden with a new value, the old value is destroyed and the shader will no longer
	/// have a set buffer.
	///
	/// Every instance of `tr::shader_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::shader_buffer` are left in a special 'invalid' state. Invalid `tr::shader_buffer` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	///
	/// `tr::shader_buffer` instances may be labeled and are formattable. Example format output: `"My buffer" (ID: 3, SSBO: 5)`.
	/// @tparam Header Type of the header object stored at the front of the buffer.
	/// @tparam ArrayElement Type of the buffer dynamic array elements.
	template <typename Header, typename ArrayElement>
		requires std::is_trivially_copyable_v<Header> && std::is_trivially_destructible_v<Header> &&
				 std::is_trivially_copyable_v<ArrayElement> && std::is_trivially_destructible_v<ArrayElement>
	class shader_buffer : private untyped_shader_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// Constructs an uninitialized shader buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param capacity Maximum capacity of the buffer array in elements.
		/// @param map_access Buffer map access type.
		/// @param label Label of the buffer.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] shader_buffer(graphics_context& context, usize capacity, access map_access = access::write_only,
									std::string_view label = {})
			: untyped_shader_buffer{context, sizeof(Header), capacity * sizeof(ArrayElement), map_access, label}
		{
		}

		/// @brief Shader buffers are not copyable.
		shader_buffer(const shader_buffer&) = delete;

		/// @brief Shader buffers are not copyable.
		shader_buffer& operator=(const shader_buffer&) = delete;

		/// @brief Constructs a shader buffer by transferring ownership from another `tr::shader_buffer`.
		/// @param rhs Shader buffer to move from.
		/// @par Invalidation
		/// If the shader buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] shader_buffer(shader_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held shader buffer and replaces it with one moved from another `tr::shader_buffer`.
		/// @param rhs Shader buffer to move from.
		/// @par Invalidation
		/// If the shader buffer previously held by `*this` is set on any shaders, it is unset.
		///
		/// If the shader buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		shader_buffer& operator=(shader_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the data of the header.
		/// @param header Head object to copy into the buffer.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// `data` must be of the same size as the buffer's header.
		void set_header(const Header& header) noexcept
		{
			untyped_shader_buffer::set_header(as_bytes(header));
		}

		/// @brief Sets the data of the dynamic array.
		/// @param data Data to set the dynamic array with.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// The size of `data` must be smaller than or equal to the capacity of the buffer's dynamic array.
		/// @par Invalidation
		/// If `*this` is set on any shaders, it is unset.
		void set_array(std::span<const ArrayElement> data) noexcept
		{
			untyped_shader_buffer::set_array(std::as_bytes(data));
		}

		/// @brief Resizes the dynamic array.
		/// @param size Size of the dynamic array.
		/// @pre
		/// The buffer must be in a valid state and must not be currently mapped.
		///
		/// `size` must be less than or equal to the capacity of the buffer's dynamic array.
		/// @par Invalidation
		/// If `*this` is set on any shaders, it is unset.
		void resize_array(usize size) noexcept
		{
			untyped_shader_buffer::resize_array(size * sizeof(ArrayElement));
		}

		/// @brief Maps the fixed header of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Map of the fixed header of the buffer.
		[[nodiscard]] mapped_graphics_buffer_object<Header> map_header()
		{
			return mapped_graphics_buffer_object<Header>{untyped_shader_buffer::map_header()};
		}

		/// @brief Maps the dynamic array of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Map of the dynamic array of the buffer.
		[[nodiscard]] mapped_graphics_buffer_span<ArrayElement> map_array()
		{
			return mapped_graphics_buffer_span<ArrayElement>{untyped_shader_buffer::map_array()};
		}

		using untyped_shader_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using untyped_shader_buffer::operator bool;

		using untyped_shader_buffer::valid;

		using untyped_shader_buffer::context;

		/// @brief Returns the size of the dynamic array.
		/// @pre The buffer must be in a valid state.
		/// @return Size of the dynamic array.
		[[nodiscard]] usize array_size() const noexcept
		{
			return untyped_shader_buffer::array_size() / sizeof(ArrayElement);
		}

		/// @brief Returns the maximum capacity of the dynamic array.
		/// @pre The buffer must be in a valid state.
		/// @return Maximum capacity of the dynamic array.
		[[nodiscard]] usize array_capacity() const noexcept
		{
			return untyped_shader_buffer::array_capacity() / sizeof(ArrayElement);
		}

		using untyped_shader_buffer::mapped;

		using untyped_shader_buffer::label;

		/// @cond gl_interop
		using untyped_shader_buffer::handle;
		/// @endcond

		/// @cond implementation_details
		using untyped_shader_buffer::id;
		/// @endcond

		/// @}
	};
} // namespace tr

//

/// @brief Shader buffer formatter.
template <typename Header, typename ArrayElement>
struct std::formatter<tr::shader_buffer<Header, ArrayElement>>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a shader buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Shader buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::shader_buffer<Header, ArrayElement>& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, SSBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {}/{} shader buffer at {}>", tr::type_name<Header>(),
								  tr::type_name<ArrayElement>(), static_cast<const void*>(&buffer));
		}
	}
};