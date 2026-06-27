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
#include "texture.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// 2D texture living on the GPU with the ability to be rendered to.
	class render_texture : private texture {
	  public:
		// Creates an empty texture.
		render_texture(graphics_context& context);
		// Allocates an uninitialized texture.
		render_texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
					   pixel_format format = pixel_format::rgba32);
		// Constructs a texture with data uploaded from a bitmap.
		render_texture(graphics_context& context, const sub_bitmap& bitmap, mipmaps mipmaps = mipmaps::disabled,
					   std::optional<pixel_format> format = std::nullopt);
		// Render textures are not copyable.
		render_texture(const render_texture&) = delete;
		// Moves a texture, updating all references pointing to it.
		render_texture(render_texture&&) noexcept;
		// Destroys the texture, emptying all references pointing to it.
		~render_texture();

		// Render textures are not copyable.
		render_texture& operator=(const render_texture&) = delete;
		// Moves a texture, updating all references pointing to it.
		render_texture& operator=(render_texture&& r) noexcept;

		// Gets a reference to the graphics context the texture is on.
		using texture::context;

		// Gets a reference to the texture.
		operator texture_ref() const;

		// Gets a render target spanning the texture.
		operator render_target() const;
		// Gets a render target spanning the texture.
		render_target render_target() const;

		// Gets whether the texture is empty.
		using texture::empty;
		// Gets the size of the texture.
		using texture::size;

		// Reallocates the texture and releases the previously held storage as a new texture.
		texture reallocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		// Sets the filters used by the texture sampler.
		using texture::set_filtering;
		// Sets the wrapping used for by the texture sampler.
		using texture::set_wrap;
		// Sets the border color of the texture sampler (used when wrap::BORDER_CLAMP is in use).
		using texture::set_border_color;

		// Clears the texture.
		using texture::clear;
		// Clears a region of the texture.
		using texture::clear_region;
		// Copies a region from another texture.
		using texture::copy_region;
		// Sets a region of the texture.
		using texture::set_region;

#ifdef TR_ENABLE_ASSERTS
		// Gets the debug label of the texture.
		using texture::label;
		// Sets the debug label of the texture.
		using texture::set_label;
#endif

	  private:
		// Handle to an OpenGL FBO.
		unsigned int m_fbo;
	};
} // namespace tr