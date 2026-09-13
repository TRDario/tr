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
	class mapped_untyped_graphics_buffer_span
	{
	  public:
		/// @cond gl_interop
		/// @name Constructors
		/// @{

		/// Wraps a raw buffer map.
		/// @param context Reference to the graphics context the buffer is on.
		/// @param buffer OpenGL ID of the mapped buffer.
		/// @param span Span of the buffer map.
		[[nodiscard]] mapped_untyped_graphics_buffer_span(graphics_context& context, unsigned int buffer,
														  std::span<std::byte> span) noexcept;

		/// @}
		/// @endcond
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