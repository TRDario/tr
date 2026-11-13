/// @file
/// @brief Provides `tr::ping_pong_target`.

#pragma once
#include <tr/sysgfx/texture_target.hpp>

//

namespace tr
{
	/// @brief Owning container for a ping-pong texture rendering target.
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
	/// Moved-from instances of `tr::ping_pong_target` are left in a special 'invalid' state. Invalid `tr::ping_pong_target` instances may
	/// not be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	class ping_pong_target
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an incomplete ping-pong target.
		/// @param context Graphics context to create the ping-pong target on.
		/// @param label Label of the ping-pong target.
		[[nodiscard]] explicit ping_pong_target(graphics_context& context, std::string_view label = {}) noexcept;

		/// @brief Constructs a complete ping-pong target.
		/// @param context Graphics context to create the ping-pong target on.
		/// @param size Initial size of the target textures.
		/// @param mipmaps Whether to generate mipmaps for the target textures.
		/// @param format Pixel format of the target textures.
		/// @param label Label of the ping-pong target.
		[[nodiscard]] ping_pong_target(graphics_context& context, glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled,
									   pixel_format format = pixel_format::rgba32, std::string_view label = {});

		/// @brief Constructs a complete ping-pong target.
		/// @param context Graphics context to create the ping-pong target on.
		/// @param size Initial size of the target textures.
		/// @param label Label of the ping-pong target.
		[[nodiscard]] ping_pong_target(graphics_context& context, glm::ivec2 size, std::string_view label);

		/// @brief Ping-pong targets are not copyable.
		ping_pong_target(const ping_pong_target&) = delete;

		/// @brief Ping-pong targets are not copyable.
		ping_pong_target& operator=(const ping_pong_target&) = delete;

		/// @brief Constructs a ping-pong target by transferring ownership from another `tr::ping_pong_target`.
		/// @param rhs Ping-pong target to move.
		/// @par Invalidation
		/// Any textures and render targets associated with `rhs` stay valid, but now point to the equivalent objects in `*this`. If the
		/// objects were set anywhere, they remain set.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] ping_pong_target(ping_pong_target&& rhs) noexcept = default;

		/// @brief Destroys the currently held ping-pong target and replaces it with one moved from another `tr::ping_pong_target`.
		/// @param rhs Ping-pong target to move.
		/// @par Invalidation
		/// Any textures and render targets associated with `*this` are invalidated. If the objects were set anywhere, they are considered
		/// unset.
		///
		/// Any textures and render targets associated with `rhs` stay valid, but now point to the equivalent objects in `*this`. If the
		/// objects were set anywhere, they remain set.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		ping_pong_target& operator=(ping_pong_target&& rhs) noexcept = default;

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a mutable view to the source target texture.
		/// @pre The ping-pong target must be in a valid state and complete.
		/// @return Mutable view to the source target texture.
		[[nodiscard]] mutable_texture_view source() noexcept;

		/// @brief Returns a view to the source target texture.
		/// @pre The ping-pong target must be in a valid state and complete.
		/// @return View to the source target texture.
		[[nodiscard]] texture_view source() const noexcept;

		/// @brief Returns the destination render target.
		/// @pre The ping-pong target must be in a valid state and complete.
		/// @return Destination render target.
		[[nodiscard]] render_target destination() noexcept;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Allocates the ping-pong target and discards any previous data.
		/// @details If the target was previously constucted incomplete, this function completes it.
		/// @param size Size of the target textures.
		/// @param mipmaps Whether to generate mipmaps for the target textures.
		/// @param format Pixel format of the target textures.
		/// @pre The ping-pong target must be in a valid state.
		/// @par Invalidation
		/// Any textures and render targets associated with `*this` are invalidated. If the objects were set anywhere, they are considered
		/// unset.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		void allocate(glm::ivec2 size, mipmaps mipmaps = mipmaps::disabled, pixel_format format = pixel_format::rgba32);

		/// @brief Sets the filters used by the target texture samplers.
		/// @param min_filter Minifying filter to use.
		/// @param mag_filter Magnifying filter to use.
		/// @pre The ping-pong target must be in a valid state and complete.
		void set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept;

		/// @brief Sets the wrapping configuration used by the target texture samplers.
		/// @param wrap Wrapping configuration to use.
		/// @pre The ping-pong target must be in a valid state and complete.
		void set_wrap(wrap wrap) noexcept;

		/// @brief Sets the border color of the target texture samplers (used when `wrap::BORDER_CLAMP` is in use).
		/// @param color Border color to use.
		/// @pre The ping-pong target must be in a valid state and complete.
		void set_border_color(rgba<float> color) noexcept;

		/// @brief Clears the destination buffer.
		/// @param color Color to clear the destination buffer to.
		/// @pre The ping-pong target must be in a valid state and complete.
		void clear_destination(rgba<float> color = rgba<float>{0, 0, 0, 0}) noexcept;

		/// @brief Swaps the source and destination targets.
		/// @pre The ping-pong target must be in a valid state and complete.
		/// @par Invalidation
		/// Any textures and render targets associated with `*this` are invalidated. If the objects were set anywhere, they are considered
		/// unset.
		void swap() noexcept;

		/// @brief Sets the label of the ping-pong target.
		/// @param label Label of the ping-pong target.
		/// @pre The ping-pong target must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void set_label(std::string_view label);

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the ping-pong target is in a valid state.
		/// @return `true` if the ping-pong target is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the ping-pong target is in a valid state.
		/// @return `true` if the ping-pong target is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the ping-pong target is on.
		/// @return Reference to the graphics context the ping-pong target is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Checks whether the ping-pong target is complete.
		/// @pre The ping-pong target must be in a valid state.
		/// @return `true` if the ping-pong target is complete, `false` otherwise.
		[[nodiscard]] bool complete() const noexcept;

		/// @brief Returns the size of the ping-pong target textures.
		/// @pre The ping-pong target must be in a valid state.
		/// @return Size of the ping-pong target textures.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// @brief Returns the label of the ping-pong target.
		/// @pre The ping-pong target must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the ping-pong target.
		[[nodiscard]] std::string label() const;

		/// @}

	  private:
		/// @brief Source and destination texture targets.
		std::array<texture_target, 2> m_targets;

		/// @brief Index of the source target within `m_targets`.
		usize m_source_index;
	};
} // namespace tr

//

/// @brief Ping-pong target formatter.
template <>
struct std::formatter<tr::ping_pong_target>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid ping-pong target format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a ping-pong target.
	/// @tparam FormatContext Formatting context type.
	/// @param target Ping-pong target to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::ping_pong_target& target, FormatContext& context) const
	{
		if (target.valid()) {
			return std::format_to(context.out(), "\"{}\"", target.label());
		}
		else {
			return std::format_to(context.out(), "<invalid ping-pong target at {}>", static_cast<const void*>(&target));
		}
	}
};