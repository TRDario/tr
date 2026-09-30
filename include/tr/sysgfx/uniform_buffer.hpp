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
	/// @details
	/// `tr::uniform_buffer` is the typed equivalent of `tr::untyped_uniform_buffer`, modeling a buffer holding a specific uniform
	/// structure.
	///
	/// Strictly speaking, instances of `tr::uniform_buffer` are only containers for these underlying buffer objects. This means, for
	/// example, that setting a uniform buffer on a shader does not set the literal `tr::uniform_buffer` object at a specific location in
	/// memory, but rather the value it contains. If the value is moved to a different instance of `tr::uniform_buffer`, that value will
	/// still be set on the shader. If the instance is overriden with a new value, the old value is destroyed and the shader will no longer
	/// have a set buffer.
	///
	/// Every instance of `tr::uniform_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::uniform_buffer` are left in a special 'invalid' state. Invalid `tr::uniform_buffer` instances may not
	/// be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::uniform_buffer` instances may be labeled and are formattable. Example format output: `"My buffer" (ID: 3, UBO: 5)`.
	/// @tparam Object Object contained in the buffer.
	template <typename Object>
	class uniform_buffer : private untyped_uniform_buffer
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Allocates an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		[[nodiscard]] uniform_buffer(graphics_context& context)
			: untyped_uniform_buffer{context, sizeof(Object)}
		{
		}

		/// Uniform buffers are not copyable.
		uniform_buffer(const uniform_buffer&) = delete;

		/// Moves a uniform buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Uniform buffer to move from.
		[[nodiscard]] uniform_buffer(uniform_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Uniform buffers are not copyable.
		uniform_buffer& operator=(const uniform_buffer&) = delete;

		/// Moves a uniform buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Uniform buffer to move from.
		/// @return Reference to `*this`.
		uniform_buffer& operator=(uniform_buffer&& rhs) noexcept = default;

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
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using untyped_uniform_buffer::id;

		/// @}
		/// @endcond
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
			return std::format_to(context.out(), "\"{}\" (ID: {}, UBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} uniform buffer at {}>", tr::type_name<Object>(),
								  static_cast<const void*>(&buffer));
		}
	}
};