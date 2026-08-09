/// @file
/// @brief Provides a GPU texture container class and related types.

#pragma once
#include "../utility/handle.hpp"
#include "bitmap.hpp"

namespace tr
{
	class graphics_context;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// Whether mipmapping should be enabled or disabled on a texture.
	enum class mipmaps : bool
	{
		/// Mipmapping disabled.
		disabled,
		/// Mipmapping enabled.
		enabled
	};

	/// Texture wrapping types.
	enum class wrap
	{
		/// The texture is repeated.
		repeat = 0x2901,
		/// The texture is repeated and mirrored.
		mirror_repeat = 0x8370,
		/// The value of the edge pixel is used.
		edge_clamp = 0x812F,
		/// The value of the border color is used.
		border_clamp = 0x812D
	};

	/// Minifying filter types.
	enum class min_filter
	{
		/// The value of the texture element that is nearest to the specified texture coordinates is used.
		nearest = 0x2600,
		/// The average of the four texture elements that are closest to the specified texture coordinates is used.
		linear = 0x2601,
		/// Chooses the mipmap that most closely matches the size of the pixel being textured and uses "nearest".
		nmip_nearest = 0x2700,
		/// Chooses the mipmap that most closely matches the size of the pixel being textured and uses "linear".
		nmip_linear = 0x2702,
		/// Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "nearest".
		lmips_nearest = 0x2701,
		/// Chooses the two mipmaps that most closely match the size of the pixel being textured and uses "linear".
		lmips_linear = 0x2703
	};

	/// Magnifying filter types.
	enum class mag_filter
	{
		/// The value of the texture element that is nearest to the specified texture coordinates is used.
		nearest = 0x2600,
		/// The average of the four texture elements that are closest to the specified texture coordinates is used.
		linear = 0x2601
	};

	//

	/// 2D GPU texture container.
	class texture
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an incomplete texture.
		/// @param context Graphics context to create the texture on.
		texture(graphics_context& context);

		/// Allocates an uninitialized texture.
		/// @param context Graphics context to create the texture on.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		texture(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
				pixel_format format = pixel_format::rgba32);

		/// Constructs a texture with data uploaded from a bitmap.
		/// @param context Graphics context to create the texture on.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		texture(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps = mipmaps::disabled,
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
		/// Texture deleter.
		struct deleter
		{
			/// Reference to the graphics context the texture is on.
			graphics_context& context;

			//

			/// Deletes a texture.
			/// @param texture OpenGL texture ID.
			void operator()(unsigned int texture) const;
		};

		//

		/// Handle to the OpenGL texture.
		mutable handle<unsigned int, 0, deleter> m_handle;

		/// Cached size of the texture.
		glm::ivec2 m_size;

		//

		/// Creates a released texture.
		/// @param context Graphics context the texture is on.
		/// @param handle Handle to the OpenGL texture.
		/// @param size Cached size of the texture.
		texture(graphics_context& context, unsigned int handle, glm::ivec2 size);

		//

		/// Creates a texture handle.
		void create_handle() const;
	};
} // namespace tr