/// @file
/// @brief Provides a texture rendering target.

#pragma once
#include <tr/sysgfx/framebuffer.hpp>
#include <tr/sysgfx/texture.hpp>

namespace tr
{
	class render_target;
}

//

namespace tr
{
	/// Two-dimensional texture rendering target.
	/// @details
	/// `tr::texture_target` bundles a `tr::texture` object together with a `tr::framebuffer` object, modelling a texture that can be used
	/// as a rendering target and sampled at a later time.
	///
	/// All caveats pertaining to the distinction between class instances and the underlying texture objects brought up in the class
	/// documentation of `tr::texture` holds for `tr::texture_target` as well. Setting a render target of the texture to the graphics
	/// context and then moving it maintains the set render target, while overwriting it clears the set render target for that context.
	///
	/// `tr::texture_target` has a notion of 'completeness'. An instance of the class may be constructed without allocating actual texture
	/// data. Such an instance is considered incomplete until the `allocate()` method is invoked. Incomplete textures may not be used for
	/// most actions.
	///
	/// Every instance of `tr::texture_target` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::texture_target` are left in a special 'invalid' state, distinct from the incomplete state. Invalid
	/// `tr::texture_target` instances may not be interacted with besides moving a new value into them and checking for validity using
	/// `valid()`.
	class texture_target
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Creates an incomplete texture target.
		/// @param context Graphics context to create the texture target on.
		/// @param label Label of the texture target.
		[[nodiscard]] explicit texture_target(graphics_context& context, std::string_view label = {}) noexcept;

		/// Allocates an uninitialized texture target.
		/// @param context Graphics context to create the texture target on.
		/// @param size Size of the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @param label Label of the texture target.
		[[nodiscard]] texture_target(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
									 pixel_format format = pixel_format::rgba32, std::string_view label = {});

		/// Constructs a texture target with data uploaded from a bitmap.
		/// @param context Graphics context to create the texture target on.
		/// @param bitmap Bitmap data to copy to the texture.
		/// @param mipmaps Whether to generate mipmaps for the texture.
		/// @param format Pixel format of the texture.
		/// @param label Label of the texture target.
		[[nodiscard]] texture_target(graphics_context& context, sub_bitmap bitmap, mipmaps mipmaps = mipmaps::disabled,
									 std::optional<pixel_format> format = std::nullopt, std::string_view label = {});

		/// Texture targets are not copyable.
		texture_target(const texture_target&) = delete;

		/// Moves a texture target.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Texture target to move from.
		[[nodiscard]] texture_target(texture_target&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Texture targets are not copyable.
		texture_target& operator=(const texture_target&) noexcept = delete;

		/// Moves a texture target.
		/// @details `rhs` is left in an invalid state after the move as per the class description.
		/// @param rhs Texture target to move from.
		/// @return Reference to `*this`.
		texture_target& operator=(texture_target&& rhs) = default;

		/// @}
		/// @name View
		/// @{

		/// Gets a mutable view to the texture.
		/// @return Mutable view to the texture.
		[[nodiscard]] operator mutable_texture_view() noexcept;

		/// Gets a view to the texture.
		/// @return View to the texture.
		[[nodiscard]] operator texture_view() const noexcept;

		/// Gets a mutable view to the texture.
		/// @return Mutable view to the texture.
		[[nodiscard]] mutable_texture_view view() noexcept;

		/// Gets a view to the texture.
		/// @return View to the texture.
		[[nodiscard]] texture_view view() const noexcept;

		/// @}
		/// @name Render target
		/// @{

		/// Creates a render target spanning the texture.
		/// @return Render target spanning the texture.
		[[nodiscard]] operator render_target() noexcept;

		/// Creates a render target spanning the texture.
		/// @return Render target spanning the texture.
		[[nodiscard]] render_target target() noexcept;

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