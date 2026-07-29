///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a texture class with the ability to be rendered to.                                                                          //
//                                                                                                                                       //
// tr::render_texture objects are extensions over the regular textures with the ability to be drawn to as render targets:                //
//     - tr::render_texture rtex{{256, 256}} -> creates a 256x256 texture that can be rendered to                                        //
//     - tr::set_render_target(rtex) -> 'rtex' is set as the target of rendering commands                                                //
//     - tr::set_render_target(rtex.render_target()) -> equivalent to the above                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "framebuffer.hpp"
#include "texture.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// 2D GPU texture with the ability to be rendered to.
	class render_texture {
	  public:
		// Creates an incomplete texture.
		render_texture(graphics_context& context);
		// Allocates an uninitialized texture.
		render_texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
					   pixel_format format = pixel_format::rgba32);
		// Constructs a texture with data uploaded from a bitmap.
		render_texture(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps = mipmaps::disabled,
					   std::optional<pixel_format> format = std::nullopt);

		// Gets a view to the texture.
		operator texture_view() const;
		// Gets a view to the texture.
		texture_view view() const;

		// Gets a render target spanning the texture.
		operator render_target() const;
		// Gets a render target spanning the texture.
		render_target render_target() const;

		// Gets a reference to the graphics context the texture is on.
		graphics_context& context() const;

		// Gets whether the texture is complete.
		bool complete() const;
		// Gets the size of the texture.
		glm::ivec2 size() const;

		// Allocates the texture and releases the previously held storage as a new texture.
		texture allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		// Sets the filters used by the texture sampler.
		void set_filtering(min_filter min_filter, mag_filter mag_filter);
		// Sets the wrapping used for by the texture sampler.
		void set_wrap(wrap wrap);
		// Sets the border color of the texture sampler (used when wrap::BORDER_CLAMP is in use).
		void set_border_color(rgbaf color);

		// Clears the texture.
		void clear(rgbaf color);
		// Clears a region of the texture.
		void clear_region(rectangle<int> region, rgbaf color);
		// Copies a region from another texture.
		void copy_region(glm::ivec2 tl, texture_view src, rectangle<int> region);
		// Sets a region of the texture.
		void set_region(glm::ivec2 tl, sub_bitmap bitmap);

		// Gets the debug label of the texture.
		std::string label() const;
		// Sets the debug label of the texture.
		void set_label(std::string_view label);

	  private:
		// Base texture.
		texture m_texture;
		// Framebuffer used by the texture.
		framebuffer m_framebuffer;
	};
} // namespace tr