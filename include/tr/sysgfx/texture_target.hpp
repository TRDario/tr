/// @file
/// @brief Provides a texture rendering target.

#pragma once
#include "framebuffer.hpp"
#include "texture.hpp"

namespace tr
{
	class render_target;
}

//

namespace tr
{
	/// 2D texture rendering target.
	class texture_target
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an incomplete texture target.
		/// @param context Graphics context to create the texture target on.
		[[nodiscard]] explicit texture_target(graphics_context& context) noexcept;

		/// Allocates an uninitialized texture target.
		/// @param context Graphics context to create the texture target on.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		[[nodiscard]] texture_target(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
									 pixel_format format = pixel_format::rgba32);

		/// Constructs a texture target with data uploaded from a bitmap.
		/// @param context Graphics context to create the texture target on.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		[[nodiscard]] texture_target(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps = mipmaps::disabled,
									 std::optional<pixel_format> format = std::nullopt);

		/// @}
		/// @name Conversion operators
		/// @{

		/// Creates a view to the texture target texture.
		/// @return View to the texture target texture.
		[[nodiscard]] operator texture_view() const noexcept;

		/// Creates a reference to the texture render target.
		/// @return Reference to the texture render target.
		[[nodiscard]] operator render_target() noexcept;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the texture target is on.
		/// @return Reference to the graphics context the texture target is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the texture target is in a valid state.
		/// @return `true` if the texture target is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// Gets whether the texture target is complete.
		/// @return `true` if the texture target is complete, `false` otherwise.
		[[nodiscard]] bool complete() const noexcept;

		/// Gets the size of the texture target.
		/// @return Size of the texture target.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// @}
		/// @name Allocation
		/// @{

		/// Allocates a texture and releases the previously held storage as a new texture.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @return Previously held storage as a new texture.
		texture allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		/// @}
		/// @name Attributes
		/// @{

		/// Sets the filters used by the texture sampler.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept;

		/// Sets the wrapping used by the texture sampler.
		/// @param wrap Wrapping type to use.
		void set_wrap(wrap wrap) noexcept;

		/// Sets the border color of the texture sampler (used when `wrap::BORDER_CLAMP` is in use).
		/// @param color Border color to use.
		void set_border_color(rgbaf color) noexcept;

		/// @}
		/// @name Clearing & setting
		/// @{

		/// Clears the texture target.
		/// @param color Color to clear the texture to.
		void clear(rgbaf color) noexcept;

		/// Clears a region of the texture target.
		/// @param region Region of the texture to clear.
		/// @param color Color to clear the texture region to.
		void clear_region(rectangle<int> region, rgbaf color) noexcept;

		/// Copies a region from another texture.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param src Source texture view.
		/// @param region Region from the texture to copy.
		void copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region) noexcept;

		/// Sets a region of the texture target.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param bitmap Bitmap data to copy to the texture.
		void set_region(glm::ivec2 tl, sub_bitmap bitmap) noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Gets the debug label of the texture target.
		/// @return Debug label of the texture target.
		[[nodiscard]] std::string label() const;

		/// Sets the debug label of the texture target.
		/// @param label Debug label of the texture target.
		void set_label(std::string_view label);

		/// @}

	  private:
		/// Base texture.
		texture m_texture;

		/// Framebuffer used by the texture.
		framebuffer m_framebuffer;
	};
} // namespace tr