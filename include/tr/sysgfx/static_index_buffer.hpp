/// @file
/// @brief Provides `tr::static_index_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>
#include <tr/utility/integer.hpp>

//

namespace tr
{
	/// Static, immutable index buffer.
	/// @details
	/// `tr::static_index_buffer` represents a graphics buffer used to store index data. Unlike `tr::dynamic_index_buffer`, it represents a
	/// single, static allocation of immutable data, making it suitable for scenarios where the index data is constant.
	///
	/// Strictly speaking, instances of `tr::static_index_buffer` are only containers for these underlying buffer objects. This means, for
	/// example, that setting a static index buffer on a graphics context does not set the literal `tr::static_index_buffer` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::static_index_buffer`, that value will still be set on the context. If the instance is overriden with a new value, the old
	/// value is destroyed and the graphics context will no longer have a set index buffer.
	///
	/// Every instance of `tr::static_index_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::static_index_buffer` are left in a special 'invalid' state. Invalid `tr::static_index_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::static_index_buffer` instances may be labeled and are formattable. Example format output: `"My index buffer" (OpenGL ID: 5)`.
	class static_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Uploads index data into a static index buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to copy into the buffer.
		[[nodiscard]] static_index_buffer(graphics_context& context, std::span<const u16> data);

		/// Static index buffers are not copyable.
		static_index_buffer(const static_index_buffer&) = delete;

		/// Moves a static index buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Static index buffer to move from.
		[[nodiscard]] static_index_buffer(static_index_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Static index buffers are not copyable.
		static_index_buffer& operator=(const static_index_buffer&) = delete;

		/// Moves a static index buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Static index buffer to move from.
		/// @return Reference to `*this`.
		static_index_buffer& operator=(static_index_buffer&& rhs) noexcept = default;

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