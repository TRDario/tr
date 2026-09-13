/// @file
/// @brief Provides `tr::static_index_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Static index buffer class for holding immutable index data.
	class static_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Uploads index data into a static index buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to copy into the buffer.
		[[nodiscard]] static_index_buffer(graphics_context& context, std::span<const u16> data);

		/// @}
		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name State
		/// @{

		using graphics_buffer::valid;

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
		/// Size of the buffer in elements.
		ssize m_size;
	};
} // namespace tr

//

/// Static index buffer formatter.
template <>
struct std::formatter<tr::static_index_buffer>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid static index buffer format specification."};
		}
		return context.begin();
	}

	/// Formats a static index buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Static index buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::static_index_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid static index buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};