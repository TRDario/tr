/// @file
/// @brief Provides `tr::ping_pong_target`.

#pragma once
#include "texture_target.hpp"

//

namespace tr
{
	/// Ping-pong texture rendering target.
	/// @details
	/// `tr::ping_pong_target` models a pair of `tr::texture_target` objects, the "source" and "destination". The source target is read-only
	/// and may be sampled from, while the destination target is write-only and may be rendered to. The targets may be swapped at any point.
	/// This arrangement is useful for certain techniques such as applying post-processing shaders.
	///
	/// `tr::ping_pong_target` has a notion of 'completeness'. An instance of the class may be constructed without allocating complete
	/// textures; such an instance is considered incomplete until the textures are fully allocated. Incomplete ping-pong targets may not be
	/// used for most actions.
	///
	/// Every instance of `tr::ping_pong_target` is associated with a graphics context and cannot outlive its parent context.
	///
	/// `tr::ping_pong_target` instances are movable, but not copyable. A moved-from instance of `tr::ping_pong_target` is left in a special
	/// 'invalid' state. Invalid `tr::ping_pong_target` instances may not be interacted with besides moving a new value into them and
	/// checking for validity using `valid()`.
	class ping_pong_target
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an incomplete ping-pong target.
		/// @param context Graphics context to create the ping-pong target on.
		[[nodiscard]] explicit ping_pong_target(graphics_context& context) noexcept;

		/// Constructs a complete ping-pong target.
		/// @param context Graphics context to create the ping-pong target on.
		/// @param size Initial size of the target textures.
		/// @param mipmaps Whether to generate mipmaps for the target textures.
		/// @param format Pixel format of the target textures.
		[[nodiscard]] ping_pong_target(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
									   pixel_format format = pixel_format::rgba32);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the ping-pong target is on.
		/// @return Reference to the graphics context the ping-pong target is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the ping-pong target is in a valid state.
		/// @return `true` if the ping-pong target is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// Gets whether the ping-pong target is complete.
		/// @return `true` if the ping-pong target is complete, `false` otherwise.
		[[nodiscard]] bool complete() const noexcept;

		/// Gets the size of the ping-pong target textures.
		/// @return Size of the ping-pong target textures.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// @}
		/// @name Manipulation
		/// @{

		/// (Re-)allocates the ping-pong target textures and discards any previous data.
		/// @details If the target was previously constucted incomplete, this function completes it.
		/// @param size Size of the target textures.
		/// @param mipmaps Whether to generate mipmaps for the target textures.
		/// @param format Pixel format of the target textures.
		void allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		/// Clears the destination buffer.
		/// @param color Color to clear the destination buffer to.
		/// @pre The target must be complete to call this function.
		void clear_destination(rgbaf color = rgbaf{}) noexcept;

		/// @}
		/// @name Texture attributes
		/// @{

		/// Sets the filters used by the target texture samplers.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept;

		/// Sets the wrapping used by the target texture samplers.
		/// @param wrap Wrapping type to use.
		void set_wrap(wrap wrap) noexcept;

		/// Sets the border color of the target texture samplers (used when `wrap::BORDER_CLAMP` is in use).
		/// @param color Border color to use.
		void set_border_color(rgbaf color) noexcept;

		/// @}
		/// @name Targets
		/// @{

		/// Gets a view to the source target texture.
		/// @details This texture view stays valid until a call to `allocate(), swap()`, or the destruction of the buffer.
		/// @pre The target must be complete to call this function.
		/// @return View to the source target texture.
		[[nodiscard]] texture_view source() const noexcept;

		/// Gets the destination render target.
		/// @details This render target stays valid until a call to `allocate(), swap()`, or the destruction of the buffer.
		/// @pre The target must be complete to call this function.
		/// @return Destination render target.
		[[nodiscard]] render_target destination() noexcept;

		/// Swaps the source and destination targets.
		void swap() noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Gets the debug label of the ping-pong target.
		/// @return Debug label of the ping-pong target.
		[[nodiscard]] std::string label() const;

		/// Sets the debug label of the ping-pong target.
		/// @param label Debug label of the ping-pong target.
		void set_label(std::string_view label);

		/// @}

	  private:
		/// Source and destination texture targets.
		std::array<texture_target, 2> m_targets;

		/// Index of the source target within `m_targets`.
		usize m_source_index;
	};
} // namespace tr

#include "impl/ping_pong_target.hpp" // IWYU pragma: export