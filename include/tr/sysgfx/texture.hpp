///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a GPU texture class and related types.                                                                                       //
//                                                                                                                                       //
// Textures are collections of pixel data representing 2D images stored on the GPU and available for use in rendering. Textures can be   //
// default-constructed (in which case they will be empty), constructed uninitialized, or initialized with bitmap data that will be       //
// copied into the texture:                                                                                                              //
//     - tr::texture tex{}                                                                                                               //
//       -> creates an empty texture                                                                                                     //
//     - tr::texture tex{{512, 512}, tr::mipmaps::enabled, tr::pixel_format::rgb24}                                                      //
//       -> creates an uninitialized 512x512 mipmapped RGB24 texture                                                                     //
//     - tr::texture tex{bmp}                                                                                                            //
//       -> creates a texture by copying the data from 'bmp' and using the same format                                                   //
//     - tr::texture tex{bmp, tr::mipmaps::disabled, tr::pixel_format::rgb24}                                                            //
//       -> creates a texture by copying the data from 'bmp' converted to RGB24                                                          //
//                                                                                                                                       //
// Textures may be reallocated using the .reallocate() method. When reallocating, the previous storage is released as a new texture:     //
//     - tex.reallocate({1024, 1024}) -> reallocates tex as an uninitialized 1024x1024 texture, and releases its old data                //
//                                                                                                                                       //
// Textures may be queried for whether they're empty or their size:                                                                      //
//     - tr::texture tex{}; tex.empty() -> true                                                                                          //
//     - tr::texture tex{{512, 512}}; tex.size() -> {512, 512}                                                                           //
//                                                                                                                                       //
// The filtering, wrapping, and border color attribtes of textures may be set:                                                           //
//     - tex.set_filtering(tr::min_filter::linear, tr::mag_filter::linear) -> 'tex' uses linear filtering                                //
//     - tex.set_wrap(tr::wrap::border_clamp) -> the border color is used for out-of-bounds UVs of 'tex'                                 //
//     - tex.set_border_color("FF0000"_rgba8) -> the border color of 'tex' is set to red                                                 //
//                                                                                                                                       //
// Textures or their regions can be cleared to a color, copied from another texture, or set from a bitmap:                               //
//     - tex.clear("FFFFFF"_rgba8) -> clears 'tex' to white                                                                              //
//     - tex.clear_region({{100, 100}, {100, 100}}, "0000FF"_rgba8) -> clears the region between (100, 100) and (200, 200) to blue       //
//     - tex.copy_region({256, 256}, tex2, {{256, 256}, {256, 256}}) -> copies a region of 'tex2' to 'tex' beginning at (256, 256)       //
//     - tex.set_region({128, 128}, tr::load_bitmap_file("data.bmp")) -> sets a region of 'tex' beginning at (128, 128) with bitmap data //
//                                                                                                                                       //
// The label of a texture can be set with .set_label() and gotten with .label():                                                         //
//     - tex.set_label("Example texture"); tex.label() -> "Example texture"                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/reference.hpp"
#include "bitmap.hpp"

namespace tr {
	class graphics_context;
	class texture;
	class texture_ref;
	class render_target;
} // namespace tr

#ifdef TR_HAS_IMGUI
using ImTextureID = unsigned long long;
namespace tr::ImGui {
	ImTextureID GetTextureID(const texture& texture);
}
#endif

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Whether mipmapping should be enabled or disabled on a texture.
	enum class mipmaps : bool {
		disabled, // Mipmapping disabled.
		enabled   // Mipmapping enabled.
	};

	// Texture wrapping types.
	enum class wrap {
		repeat = 0x2901,        // The texture is repeated.
		mirror_repeat = 0x8370, // The texture is repeated and mirrored.
		edge_clamp = 0x812F,    // The value of the edge pixel is used.
		border_clamp = 0x812D   // The value of the border color is used.
	};

	// Minifying filter types.
	enum class min_filter {
		nearest = 0x2600,       // The value of the texture element that is nearest to the specified texture coordinates is used.
		linear = 0x2601,        // The average of the four texture elements that are closest to the specified texture coordinates is used.
		nmip_nearest = 0x2700,  // Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		nmip_linear = 0x2702,   // Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		lmips_nearest = 0x2701, // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		lmips_linear = 0x2703   // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
	};

	// Magnifying filter types.
	enum class mag_filter {
		nearest = 0x2600, // The value of the texture element that is nearest to the specified texture coordinates is used.
		linear = 0x2601   // The average of the four texture elements that are closest to the specified texture coordinates is used.
	};

	// 2D texture living on the GPU.
	class texture {
	  public:
		// Creates an empty texture.
		texture(graphics_context& context);
		// Allocates an uninitialized texture.
		texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
				pixel_format format = pixel_format::rgba32);
		// Constructs a texture with data uploaded from a bitmap.
		texture(graphics_context& context, const sub_bitmap& bitmap, mipmaps mipmaps = mipmaps::disabled,
				std::optional<pixel_format> format = std::nullopt);
		// Moves a texture, updating all references pointing to it.
		texture(texture&& r) noexcept;
		// Destroys the texture, emptying all references pointing to it.
		~texture();

		// Moves a texture, updating all references pointing to it.
		texture& operator=(texture&& r) noexcept;

		// Gets a reference to the graphics context the texture is on.
		graphics_context& context() const;

		// Gets whether the texture is empty.
		bool empty() const;
		// Gets the size of the texture.
		glm::ivec2 size() const;

		// Reallocates the texture and releases the previously held storage as a new texture.
		texture reallocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		// Sets the filters used by the texture sampler.
		void set_filtering(min_filter min_filter, mag_filter mag_filter);
		// Sets the wrapping used for by the texture sampler.
		void set_wrap(wrap wrap);
		// Sets the border color of the texture sampler (used when wrap::BORDER_CLAMP is in use).
		void set_border_color(rgbaf color);

		// Clears the texture.
		void clear(const rgbaf& color);
		// Clears a region of the texture.
		void clear_region(const rectangle<int>& region, const rgbaf& color);
		// Copies a region from another texture.
		void copy_region(glm::ivec2 tl, const texture& src, const rectangle<int>& region);
		// Sets a region of the texture.
		void set_region(glm::ivec2 tl, const sub_bitmap& bitmap);

		// Gets the debug label of the texture.
		std::string label() const;
		// Sets the debug label of the texture.
		void set_label(std::string_view label);

	  protected:
		// Reference to the graphics context the texture is on.
		graphics_context& m_context;
		// Handle to the OpenGL texture.
		unsigned int m_handle;
		// The cached size of the texture.
		glm::ivec2 m_size;
		// List of active references to this texture.
		mutable std::vector<ref<texture_ref>> m_references;

		// Creates a released texture.
		texture(graphics_context& context, unsigned int handle, glm::ivec2 size);

		friend class texture_ref;
		friend class shader_base;
		friend class graphics_context;

#ifdef TR_HAS_IMGUI
		friend ImTextureID ImGui::GetTextureID(const texture& texture);
#endif
	};
} // namespace tr