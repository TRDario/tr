/// @file
/// @brief Provides `tr::static_index_buffer`.

#pragma once
#include <tr/sysgfx/graphics_buffer.hpp>

//

namespace tr
{
	/// @brief Owning container for a static and immutable index buffer.
	/// @details
	/// `tr::static_index_buffer` represents a graphics buffer used to store index data. Unlike `tr::dynamic_index_buffer`, it represents a
	/// single, static allocation of immutable data, making it suitable for scenarios where the index data is constant.
	///
	/// It's worth remembering that instances of `tr::static_index_buffer` are only containers for these underlying buffer objects. This
	/// means, for example, that setting a static index buffer on a graphics context does not set the literal `tr::static_index_buffer`
	/// object at a specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::static_index_buffer`, that value will still be set on the context. If the instance is overriden with a new value, the old
	/// value is destroyed and the graphics context will no longer have a set index buffer.
	///
	/// Every instance of `tr::static_index_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::static_index_buffer` are left in a special 'invalid' state. Invalid `tr::static_index_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator
	/// bool()`.
	///
	/// `tr::static_index_buffer` instances may be labeled and are formattable. Example format output: `"My index buffer" (ID: 3, IBO:
	/// 5)`.
	class static_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a static index buffer with index data.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to copy into the buffer.
		/// @param label Label of the buffer.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] static_index_buffer(graphics_context& context, std::span<const u16> data, std::string_view label = {});

		/// @brief Static index buffers are not copyable.
		static_index_buffer(const static_index_buffer&) = delete;

		/// @brief Static index buffers are not copyable.
		static_index_buffer& operator=(const static_index_buffer&) = delete;

		/// @brief Constructs a static index buffer by transferring ownership from another `tr::static_index_buffer`.
		/// @param rhs Static index buffer to move from.
		/// @par Invalidation
		/// If the index buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] static_index_buffer(static_index_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held static index buffer and transfers ownership from another `tr::static_index_buffer`.
		/// @param rhs Static index buffer to move from.
		/// @par Invalidation
		/// If the index buffer previously held by `*this` was set on the associated graphics context, it is unset.
		///
		/// If the index buffer previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		static_index_buffer& operator=(static_index_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		using graphics_buffer::set_label;

		/// @}
		/// @name Observers
		/// @{

		using graphics_buffer::operator bool;

		using graphics_buffer::valid;

		using graphics_buffer::context;

		using graphics_buffer::label;

		/// @cond gl_interop
		using graphics_buffer::handle;
		/// @endcond

		/// @cond implementation_details
		using graphics_buffer::id;
		/// @endcond

		/// @}

	  private:
		/// @brief Size of the buffer.
		ssize m_size;
	};
} // namespace tr

//

/// @brief Static index buffer formatter.
template <>
struct std::formatter<tr::static_index_buffer>
{
	/// @brief Parses the format specification.
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

	/// @brief Formats a static index buffer.
	/// @tparam FormatContext Formatting context type.
	/// @param buffer Static index buffer to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::static_index_buffer& buffer, FormatContext& context) const
	{
		if (buffer.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, IBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid static index buffer at {}>", static_cast<const void*>(&buffer));
		}
	}
};