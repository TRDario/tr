/// @file
/// @brief Provides `tr::vertex_format` and related functionality.
/// @details For an explanation of vertex formats, see the description of `tr::vertex_format`.

#pragma once
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/sysgfx/vertex_binding.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// @brief Owning container for a GPU vertex format.
	/// @details
	/// Vertex formats define the layout of vertex data passed to a drawing operation. This includes the type, attributes and divisor of the
	/// data set to be held in one or more indexed vertex buffers. They can be considered the counterpart of vertex array objects in OpenGL.
	///
	/// It's worth remembering that instances of `tr::vertex_format` are only containers for these underlying vertex format objects. This
	/// means, for example, that setting a vertex format on a graphics context does not set the literal `tr::vertex_format` object at a
	/// specific location in memory, but rather the value it contains. If the value is moved to a different instance of `tr::vertex_format`,
	/// that value will still be set on the context. If the instance is overriden with a new value, the old value is destroyed and the
	/// graphics context will no longer have a set vertex format.
	///
	/// Every instance of `tr::vertex_format` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::vertex_format` are left in a special 'invalid' state. Invalid `tr::vertex_format` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	///
	/// `tr::vertex_format` instances may be labeled and are formattable. Example format output: `"My vertex format" (ID: 3, VAO: 5)`.
	class vertex_format
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a vertex format.
		/// @param context Graphics context to create the format on.
		/// @param bindings Vertex bindings of the format.
		/// @param label Label of the vertex format.
		/// @post `bindings` must stay valid for the duration of the lifetime of the vertex format.
		[[nodiscard]] vertex_format(graphics_context& context, std::span<const vertex_binding> bindings,
									std::string_view label = {}) noexcept;

		/// @brief Vertex formats are not copyable.
		vertex_format(const vertex_format&) = delete;

		/// @brief Vertex formats are not copyable.
		vertex_format& operator=(const vertex_format&) = delete;

		/// @brief Constructs a vertex format by transferring ownership from another `tr::vertex_format`.
		/// @param rhs Vertex format to move.
		/// @par Invalidation
		/// If the vertex format previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] vertex_format(vertex_format&& rhs) noexcept = default;

		/// @brief Destroys the currently held vertex format and replaces it with one moved from another `tr::vertex_format`.
		/// @param rhs Vertex format to move.
		/// @par Invalidation
		/// If the vertex format previously held by `*this` was set on the associated graphics context, it is unset.
		///
		/// If the vertex format previously held by `rhs` is set on the associated graphics context, it stays set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		vertex_format& operator=(vertex_format&& rhs) noexcept = default;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the label of the vertex format.
		/// @param label Label of the vertex format.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the vertex format is in a valid state.
		/// @return `true` if the vertex format is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the vertex format is in a valid state.
		/// @return `true` if the vertex format is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the vertex format is on.
		/// @pre The vertex format must be in a valid state.
		/// @return Reference to the graphics context the vertex format is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Returns the label of the vertex format.
		/// @pre The vertex format must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the vertex format.
		[[nodiscard]] std::string label() const;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL vertex array object.
		/// @return Handle to the underlying OpenGL vertex array object.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @cond implementation_details
		/// @brief Returns the ID uniquely identifying the vertex format.
		/// @return ID uniquely identifying the vertex format.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @brief Returns information about the vertex format's bindings.
		/// @return Information about the vertex format's bindings.
		[[nodiscard]] std::span<const vertex_binding> bindings() const noexcept;
#endif
		/// @endcond

		/// @}

	  private:
		/// @brief OpenGL vertex array object deleter.
		struct gl_vertex_array_object_deleter
		{
			/// @brief Reference to the graphics context the vertex array object is on.
			ref<graphics_context> context;

			/// @brief ID uniquely identifying the vertex format.
			unique_handle<internal::graphics_object_id, internal::graphics_object_id::invalid> id{internal::generate_graphics_object_id()};

			//

			/// @brief Destroys the vertex array object.
			/// @param handle OpenGL vertex array object handle.
			void operator()(unsigned int handle) const noexcept;
		};

		//

		/// @brief Managed handle to an OpenGL vertex array object.
		unique_handle<unsigned int, 0, gl_vertex_array_object_deleter> m_handle;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @brief Information about the vertex format's bindings.
		std::span<const vertex_binding> m_bindings;
#endif
	};
} // namespace tr

//

/// @brief Vertex format formatter.
template <>
struct std::formatter<tr::vertex_format>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid vertex format format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a vertex format.
	/// @tparam FormatContext Formatting context type.
	/// @param format Vertex format to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::vertex_format& format, FormatContext& context) const
	{
		if (format.valid()) {
			return std::format_to(context.out(), "\"{}\" (ID: {}, VAO: {})", format.label(), std::to_underlying(format.id()),
								  format.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid vertex format at {}>", static_cast<const void*>(&format));
		}
	}
};