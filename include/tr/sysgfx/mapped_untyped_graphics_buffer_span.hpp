/// @file
/// @brief Provides `tr::mapped_untyped_graphics_buffer_span`.

#pragma once
#include <tr/utility/handle.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// Map of a span of an untyped graphics buffer.
	/// @details
	/// `tr::mapped_untyped_graphics_buffer_span` represents the CPU mapping of a span of an untyped graphics buffer. Unlike
	/// `tr::mapped_graphics_buffer_span` or `tr::mapped_graphics_buffer_object`, it does not attempt to model the contents of the buffer,
	/// but is instead a minimal wrapper around a span of bytes that unmaps the buffer when destroyed.
	///
	/// Every instance of `tr::mapped_untyped_graphics_buffer_span` is associated with a graphics context, as well as with a buffer, and
	/// cannot outlive either.
	///
	/// Moved-from instances of `tr::mapped_untyped_graphics_buffer_span` are left in a special 'invalid' state. Invalid
	/// `tr::mapped_untyped_graphics_buffer_span` instances may not be interacted with besides moving a new value into them.
	class mapped_untyped_graphics_buffer_span
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// @cond gl_interop

		/// Wraps a raw buffer span map.
		/// @param context Reference to the graphics context the buffer is on.
		/// @param buffer OpenGL ID of the mapped buffer.
		/// @param span Span of the buffer map.
		[[nodiscard]] mapped_untyped_graphics_buffer_span(graphics_context& context, unsigned int buffer,
														  std::span<std::byte> span) noexcept;

		/// @endcond

		/// Untyped graphics buffer span maps are not copyable.
		mapped_untyped_graphics_buffer_span(const mapped_untyped_graphics_buffer_span&) = delete;

		/// Moves an untyped graphics buffer span map.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped graphics buffer span map to move from.
		[[nodiscard]] mapped_untyped_graphics_buffer_span(mapped_untyped_graphics_buffer_span&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Untyped graphics buffer span maps are not copyable.
		mapped_untyped_graphics_buffer_span& operator=(const mapped_untyped_graphics_buffer_span&) = delete;

		/// Moves an untyped graphics buffer span map.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Untyped graphics buffer span map to move from.
		/// @return Reference to `*this`.
		mapped_untyped_graphics_buffer_span& operator=(mapped_untyped_graphics_buffer_span&& rhs) noexcept = default;

		/// @name Span conversion
		/// @{

		/// Casts the map into a regular span.
		/// @return Span of bytes covering the buffer map.
		[[nodiscard]] operator std::span<std::byte>() const noexcept;

		/// Casts the map into a regular span.
		/// @return Span of bytes covering the buffer map.
		[[nodiscard]] std::span<std::byte> span() const noexcept;

		/// @}

	  private:
		/// Buffer unmapper.
		struct unmapper
		{
			/// Reference to the context the buffer object is on.
			ref<graphics_context> context;

			//

			/// Unmaps a buffer.
			/// @param id OpenGL buffer ID.
			void operator()(unsigned int id) const noexcept;
		};

		//

		/// Handle to the graphics buffer for the purposes of unmapping.
		handle<unsigned int, 0, unmapper> m_handle;

		/// Span of the buffer map.
		std::span<std::byte> m_span;
	};
} // namespace tr