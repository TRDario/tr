/// @file
/// @brief Provides `tr::untyped_uniform_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>

namespace tr
{
	class mapped_untyped_graphics_buffer_span;
}

//

namespace tr
{
	/// @brief Owning container for an untyped shader uniform buffer.
	/// @details
	/// `tr::untyped_uniform_buffer` represents a graphics buffer used to store data for shader uniform access. `tr::untyped_uniform_buffer`
	/// does not enforce any specific data format and may be used when flexibility is needed, while `tr::uniform_buffer` models a buffer
	/// holding a specific structure. Instances of `tr::untyped_uniform_buffer` are allocated once and cannot be resized.
	///
	/// It's worth remembering that instances of `tr::untyped_uniform_buffer` are only containers for these underlying buffer objects. This
	/// means, for example, that setting an untyped uniform buffer on a shader does not set the literal `tr::untyped_uniform_buffer` object
	/// at a specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::untyped_uniform_buffer`, that value will still be set on the shader. If the instance is overriden with a new value, the old
	/// value is destroyed and the shader will no longer have a set buffer.
	///
	/// Every instance of `tr::untyped_uniform_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::untyped_uniform_buffer` are left in a special 'invalid' state. Invalid `tr::untyped_uniform_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator
	/// bool()`.
	///
	/// `tr::untyped_uniform_buffer` instances may be labeled and are formattable. Example format output: `"My buffer" (ID: 3, UBO: 5)`.
	class untyped_uniform_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param size Initial size of the buffer.
		/// @param label Label of the buffer.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] untyped_uniform_buffer(graphics_context& context, usize size, std::string_view label = {});

		/// @brief Untyped uniform buffers are not copyable.
		untyped_uniform_buffer(const untyped_uniform_buffer&) = delete;

		/// @brief Untyped uniform buffers are not copyable.
		untyped_uniform_buffer& operator=(const untyped_uniform_buffer&) = delete;

		/// @brief Constructs an untyped uniform buffer by transferring ownership from another `tr::untyped_uniform_buffer`.
		/// @param rhs Untyped uniform buffer to move from.
		/// @par Invalidation
		/// If the uniform buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] untyped_uniform_buffer(untyped_uniform_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held untyped uniform buffer and transfers ownership from another `tr::untyped_uniform_buffer`.
		/// @param rhs Untyped uniform buffer to move from.
		/// @par Invalidation
		/// If the uniform buffer previously held by `*this` is set on any shaders, it is unset.
		///
		/// If the uniform buffer previously held by `rhs` is set on any shaders, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		untyped_uniform_buffer& operator=(untyped_uniform_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the data of the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @param data Data to copy into the buffer.
		void set(std::span<const std::byte> data) noexcept;

		/// @brief Maps the buffer.
		/// @pre The buffer must be in a valid state and must not be currently mapped.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Write-only map of the buffer.
		[[nodiscard]] mapped_untyped_graphics_buffer_span map();

		using graphics_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using graphics_buffer::operator bool;

		using graphics_buffer::valid;

		using graphics_buffer::context;

		/// @brief Returns the size of the buffer.
		/// @pre The buffer must be in a valid state.
		/// @return Size of the buffer in bytes.
		[[nodiscard]] usize size() const noexcept;

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
		/// @brief Size of the buffer in bytes.
		usize m_size;
	};
} // namespace tr

//

/// @brief Untyped uniform buffer formatter.
template <>
struct std::formatter<tr::untyped_uniform_buffer>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid untyped uniform buffer format specification."};
		}
		return context.begin();
	}

	/// @brief Formats an untyped uniform buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Uniform buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_uniform_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, UBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped uniform buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};