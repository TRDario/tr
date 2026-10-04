/// @file
/// @brief Provides `tr::static_vertex_buffer`.

#pragma once
#include <tr/sysgfx/untyped_static_vertex_buffer.hpp>
#include <tr/utility/concepts.hpp>
#include <tr/utility/type_name.hpp>

//

namespace tr
{
	/// Static, immutable typed vertex buffer.
	/// @details
	/// `tr::static_vertex_buffer` represents a graphics buffer used to store vertex data. Unlike `tr::dynamic_vertex_buffer`, it represents
	/// a single, static allocation of immutable data, making it suitable for scenarios where the vertex data is constant. It is the typed
	/// equivalent of `tr::untyped_static_vertex_buffer`.
	///
	/// Strictly speaking, instances of `tr::static_vertex_buffer` are only containers for these underlying buffer objects. This means, for
	/// example, that setting a static vertex buffer on a graphics context does not set the literal `tr::static_vertex_buffer` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of
	/// `tr::static_vertex_buffer`, that value will still be set on the context. If the instance is overriden with a new value, the old
	/// value is destroyed and the graphics context will no longer have a set vertex buffer.
	///
	/// Every instance of `tr::static_vertex_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::static_vertex_buffer` are left in a special 'invalid' state. Invalid `tr::static_vertex_buffer`
	/// instances may not be interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::static_vertex_buffer` instances may be labeled and are formattable. Example format output: `"My vertex buffer" (ID: 3, VBO:
	/// 5)`.
	/// @tparam Element Type of the elements of the buffer.
	template <standard_layout Element>
	class static_vertex_buffer : private untyped_static_vertex_buffer
	{
	  public:
		/// Value type used by the buffer.
		using value_type = Element;

		/// @name Constructors and destructors
		/// @{

		/// Uploads vertex data into a static vertex buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to upload to the buffer.
		/// @param label Label of the buffer.
		[[nodiscard]] static_vertex_buffer(graphics_context& context, std::span<const Element> data, std::string_view label = {})
			: untyped_static_vertex_buffer{context, std::as_bytes(data), label}
		{
		}

		/// Static vertex buffers are not copyable.
		static_vertex_buffer(const static_vertex_buffer&) = delete;

		/// Moves a static vertex buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Static vertex buffer to move.
		[[nodiscard]] static_vertex_buffer(static_vertex_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Static vertex buffers are not copyable.
		static_vertex_buffer& operator=(const static_vertex_buffer&) = delete;

		/// Moves a static vertex buffer.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Static vertex buffer to move.
		/// @return Reference to `*this`.
		static_vertex_buffer& operator=(static_vertex_buffer&& rhs) noexcept = default;

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
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using untyped_static_vertex_buffer::id;

		/// @}
		/// @endcond
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
			return std::format_to(context.out(), "\"{}\" (ID: {}, VBO: {})", buffer.label(), std::to_underlying(buffer.id()),
								  buffer.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid {} static vertex buffer at {}>", tr::type_name<Element>(),
								  static_cast<const void*>(&buffer));
		}
	}
};