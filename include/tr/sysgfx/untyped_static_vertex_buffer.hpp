/// @file
/// @brief Provides `tr::untyped_static_vertex_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Static vertex buffer class for holding immutable vertex data.
	class untyped_static_vertex_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Uploads vertex data into a static vertex buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to upload to the buffer.
		[[nodiscard]] untyped_static_vertex_buffer(graphics_context& context, std::span<const std::byte> data);

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
		/// Size of the vertex buffer in bytes.
		ssize m_size;
	};
} // namespace tr

//

/// Untyped static vertex buffer formatter.
template <>
struct std::formatter<tr::untyped_static_vertex_buffer>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid untyped static vertex buffer format specification."};
		}
		return context.begin();
	}

	/// Formats an untyped static vertex buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Vertex buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::untyped_static_vertex_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid untyped static vertex buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};