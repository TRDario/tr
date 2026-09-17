/// @file
/// @brief Provides `tr::untyped_uniform_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

namespace tr
{
	class mapped_untyped_graphics_buffer_span;
}

//

namespace tr
{
	/// Untyped shader uniform buffer.
	/// @details
	/// `tr::untyped_uniform_buffer` represents a graphics buffer used to store data for shader uniform access. `tr::untyped_uniform_buffer`
	/// does not enforce any specific data format and may be used when flexibility is needed, while `tr::uniform_buffer` models a buffer
	/// holding a specific structure. Instances of `tr::untyped_uniform_buffer` are allocated once and cannot be resized.
	///
	/// Strictly speaking, instances of `tr::untyped_uniform_buffer` are only containers for these underlying buffer objects. This means,
	/// for example, that setting an untyped uniform buffer on a shader does not set the literal `tr::untyped_uniform_buffer` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::untyped_uniform_buffer`, that value will still be set on the shader. If the instance is overriden with a new value, the old
	/// value is destroyed and the shader will no longer have a set buffer.
	///
	/// Every instance of `tr::untyped_uniform_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::untyped_uniform_buffer` are left in a special 'invalid' state. Invalid `tr::untyped_uniform_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::untyped_uniform_buffer` instances may be labeled and are formattable. Example format output: `"My buffer" (OpenGL ID: 5)`.
	class untyped_uniform_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Allocates an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param size Initial size of the buffer.
		[[nodiscard]] untyped_uniform_buffer(graphics_context& context, usize size);

		/// Untyped uniform buffers are not copyable.
		untyped_uniform_buffer(const untyped_uniform_buffer&) = delete;

		/// Moves an untyped uniform buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped uniform buffer to move from.
		[[nodiscard]] untyped_uniform_buffer(untyped_uniform_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Untyped uniform buffers are not copyable.
		untyped_uniform_buffer& operator=(const untyped_uniform_buffer&) = delete;

		/// Moves an untyped uniform buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped uniform buffer to move from.
		/// @return Reference to `*this`.
		untyped_uniform_buffer& operator=(untyped_uniform_buffer&& rhs) noexcept = default;

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

		/// Gets the size of the buffer.
		/// @return Size of the buffer in bytes.
		[[nodiscard]] usize size() const noexcept;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the buffer.
		/// @param data Data to copy into the buffer.
		void set(std::span<const std::byte> data) noexcept;

		/// @}
		/// @name Mapping
		/// @{

		/// Gets whether the buffer is mapped.
		/// @return `true` if the buffer is mapped, `false` otherwise.
		[[nodiscard]] bool mapped() const noexcept;

		/// Maps the buffer.
		/// @return Write-only map of the buffer.
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
		/// Size of the buffer.
		usize m_size;
	};
} // namespace tr

//

/// Untyped uniform buffer formatter.
template <>
struct std::formatter<tr::untyped_uniform_buffer>
{
	/// Parses the format specification.
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

	/// Formats an untyped uniform buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Uniform buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_uniform_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped uniform buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};