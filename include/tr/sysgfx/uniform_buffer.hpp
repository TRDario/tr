/// @file
/// @brief Provides `tr::uniform_buffer`.

#pragma once
#include <tr/sysgfx/mapped_graphics_buffer_object.hpp>
#include <tr/sysgfx/untyped_uniform_buffer.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Typed shader uniform buffer.
	/// @tparam Object Objet contained in the buffer.
	template <typename Object>
	class uniform_buffer : private untyped_uniform_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Allocates an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		[[nodiscard]] uniform_buffer(graphics_context& context)
			: untyped_uniform_buffer{context, sizeof(Object)}
		{
		}

		/// @}
		/// @name Context
		/// @{

		using untyped_uniform_buffer::context;

		/// @}
		/// @name State
		/// @{

		using untyped_uniform_buffer::valid;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the contents of the buffer.
		/// @param data Object to copy into the buffer.
		void set(const Object& data) noexcept
		{
			untyped_uniform_buffer::set(as_bytes(data));
		}

		/// @}
		/// @name Mapping
		/// @{

		using untyped_uniform_buffer::mapped;

		/// Maps the buffer.
		/// @return Write-only map of the buffer object.
		[[nodiscard]] mapped_graphics_buffer_object<Object> map()
		{
			return mapped_untyped_graphics_buffer_span{untyped_uniform_buffer::map()};
		}

		/// @}
		/// @name Label
		/// @{

		using untyped_uniform_buffer::label;

		using untyped_uniform_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using untyped_uniform_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using untyped_uniform_buffer::id;

		/// @}
		/// @endcond
#endif
	};
} // namespace tr

//

/// Uniform buffer formatter.
template <typename Object>
struct std::formatter<tr::uniform_buffer<Object>>
{
	/// Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid uniform buffer format specification."};
		}
		return context.begin();
	}

	/// Formats a uniform buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Uniform buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::uniform_buffer<Object>& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", buffer.label(), buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} uniform buffer at {}>", tr::type_name<Object>(),
								  static_cast<const void*>(&buffer));
		}
	}
};