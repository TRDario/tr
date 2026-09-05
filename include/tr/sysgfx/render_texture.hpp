/// @file
/// @brief Provides a texture class with the ability to be rendered to.

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
	/// 2D GPU texture with the ability to be rendered to.
	class render_texture
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an incomplete texture.
		/// @param context Graphics context to create the texture on.
		render_texture(graphics_context& context);

		/// Allocates an uninitialized texture.
		/// @param context Graphics context to create the texture on.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		render_texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
					   pixel_format format = pixel_format::rgba32);

		/// Constructs a texture with data uploaded from a bitmap.
		/// @param context Graphics context to create the texture on.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		render_texture(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps = mipmaps::disabled,
					   std::optional<pixel_format> format = std::nullopt);

		/// @}
		/// @name Views
		/// @{

		/// Gets a view to the texture.
		/// @return View to the texture.
		operator texture_view() const;

		/// Gets a view to the texture.
		/// @return View to the texture.
		texture_view view() const;

		/// @}
		/// @name Render target
		/// @{

		/// Gets a render target spanning the texture.
		/// @return Render target spanning the texture.
		operator render_target() const;

		/// Gets a render target spanning the texture.
		/// @return Render target spanning the texture.
		render_target target() const;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the texture is on.
		/// @return Reference to the graphics context the texture is on.
		graphics_context& context() const;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the texture is complete.
		/// @return `true` if the texture is complete, `false` otherwise.
		bool complete() const;

		/// Gets the size of the texture.
		/// @return Size of the texture.
		glm::ivec2 size() const;

		/// @}
		/// @name Allocation
		/// @{

		/// Allocates the texture and releases the previously held storage as a new texture.
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
		void set_filtering(min_filter min_filter, mag_filter mag_filter);

		/// Sets the wrapping used by the texture sampler.
		/// @param wrap Wrapping type to use.
		void set_wrap(wrap wrap);

		/// Sets the border color of the texture sampler (used when `wrap::BORDER_CLAMP` is in use).
		/// @param color Border color to use.
		void set_border_color(rgbaf color);

		/// @}
		/// @name Clearing & setting
		/// @{

		/// Clears the texture.
		/// @param color Color to clear the texture to.
		void clear(rgbaf color);

		/// Clears a region of the texture.
		/// @param region Region of the texture to clear.
		/// @param color Color to clear the texture region to.
		void clear_region(rectangle<int> region, rgbaf color);

		/// Copies a region from another texture.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param src Source texture view.
		/// @param region Region from the texture to copy.
		void copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region);

		/// Sets a region of the texture.
		/// @param tl Top-left corner of the copied region within the target texture.
		/// @param bitmap Bitmap data to copy to the texture.
		void set_region(glm::ivec2 tl, sub_bitmap bitmap);

		/// @}
		/// @name Label
		/// @{

		/// Gets the debug label of the texture.
		/// @return Debug label of the texture.
		std::string label() const;

		/// Sets the debug label of the texture.
		/// @param label Debug label of the texture.
		void set_label(std::string_view label);

		/// @}

	  private:
		/// Base texture.
		texture m_texture;

		/// Framebuffer used by the texture.
		framebuffer m_framebuffer;
	};
} // namespace tr