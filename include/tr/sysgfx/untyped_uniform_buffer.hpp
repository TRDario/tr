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
	class untyped_uniform_buffer : private graphics_buffer
	{
	  public:
		/// Allocates an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param size Initial size of the buffer.
		[[nodiscard]] untyped_uniform_buffer(graphics_context& context, usize size);

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