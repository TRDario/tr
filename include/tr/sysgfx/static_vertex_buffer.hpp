/// @file
/// @brief Provides `tr::static_vertex_buffer`.

#pragma once
#include <tr/sysgfx/untyped_static_vertex_buffer.hpp>
#include <tr/utility/concepts.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Typed static vertex buffer class for holding immutable vertex data of a single type.
	/// @tparam Element Type of the elements of the buffer.
	template <standard_layout Element>
	class static_vertex_buffer : private untyped_static_vertex_buffer
	{
	  public:
		/// Value type used by the buffer.
		using value_type = Element;

		/// @name Constructors
		/// @{

		/// Uploads vertex data into a static vertex buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param range Data to upload to the buffer.
		[[nodiscard]] static_vertex_buffer(graphics_context& context, std::span<const Element> range)
			: untyped_static_vertex_buffer{context, std::as_bytes(range)}
		{
		}

		/// @}
		/// @name Context
		/// @{

		using untyped_static_vertex_buffer::context;

		/// @}
		/// @name State
		/// @{

		using untyped_static_vertex_buffer::valid;

		/// @}
		/// @name Label
		/// @{

		using untyped_static_vertex_buffer::label;

		using untyped_static_vertex_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using untyped_static_vertex_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using untyped_static_vertex_buffer::id;

		/// @}
		/// @endcond
#endif
	};
} // namespace tr

//

/// Static vertex buffer formatter.
template <tr::standard_layout Element>
struct std::formatter<tr::static_vertex_buffer<Element>>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid static vertex buffer format specification."};
		}
		return context.begin();
	}

	/// Formats a static vertex buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Vertex buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::static_vertex_buffer<Element>& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} static vertex buffer at {}>", tr::type_name<Element>(),
								  static_cast<const void*>(&buffer));
		}
	}
};