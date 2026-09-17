/// @file
/// @brief Provides `tr::render_target`.

#pragma once
#include <tr/sysgfx/internal/framebuffer_info.hpp>
#include <tr/utility/rectangle.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	class framebuffer;
	class graphics_context;
#ifdef TR_ENABLE_CHECKED_GRAPHICS
	namespace internal
	{
		enum class graphics_object_id : u32;
	}
#endif
} // namespace tr

//

namespace tr
{
	/// Reference to a rendering target.
	/// @details
	/// `tr::render_target` is a reference to a specific rendering target. It is designed to be cheap to pass around, though passing by
	/// reference is still recommended due to its size.
	///
	/// If not empty, the render target reference should not outlive the texture object it is pointing at, as it will otherwise become
	/// dangling.
	class render_target
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a backbuffer render target.
		/// @note This is equivalent to calling `context.backbuffer()`.
		/// @param context Graphics context whose backbuffer to create the render target for.
		[[nodiscard]] explicit render_target(const graphics_context& context) noexcept;

		/// Creates a render target on a framebuffer.
		/// @param framebuffer Framebuffer the render target is on.
		/// @param framebuffer_size Size of the framebuffer.
		[[nodiscard]] render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size) noexcept;

		/// Creates a render target on a framebuffer.
		/// @param framebuffer Framebuffer the render target is on.
		/// @param framebuffer_size Size of the framebuffer.
		/// @param viewport Viewport of the framebuffer.
		/// @param scissor_box Scissor box of the framebuffer.
		[[nodiscard]] render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size, rectangle<int> viewport,
									rectangle<int> scissor_box) noexcept;

		/// Render targets are trivially copyable.
		[[nodiscard]] render_target(const render_target&) noexcept = default;

		/// Render targets are trivially movable.
		[[nodiscard]] render_target(render_target&&) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Render targets are trivially copyable.
		/// @return Reference to `*this`.
		render_target& operator=(const render_target&) noexcept = default;

		/// Render targets are trivially movable.
		/// @return Reference to `*this`.
		render_target& operator=(render_target&&) noexcept = default;

		/// @}
		/// @name Properties
		/// @{

		/// Gets the size of the render target.
		/// @return Size of the render target.
		[[nodiscard]] glm::ivec2 size() const noexcept;

		/// Gets the viewport of the render target within the target framebuffer.
		/// @return Viewport rectangle of the render target within the target framebuffer.
		[[nodiscard]] rectangle<int> viewport() const noexcept;

		/// Gets the scissor box of the render target within the target framebuffer.
		/// @return Scissor box rectangle of the render target within the target framebuffer.
		[[nodiscard]] rectangle<int> scissor_box() const noexcept;

		/// @}
		/// @name Subtargets
		/// @{

		/// Creates a new render target with a cropped viewport and full scissor box.
		/// @param viewport Viewport of the render target.
		/// @return New subtarget.
		[[nodiscard]] render_target cropped(rectangle<int> viewport) const noexcept;

		/// Creates a new render target with the same viewport and a different scissor box.
		/// @param scissor_box Scissor box of the render target.
		/// @return New subtarget.
		[[nodiscard]] render_target scissored(rectangle<int> scissor_box) const noexcept;

		/// @}
		/// @cond implementation_details

		/// Gets information about the render target's framebuffer.
		/// @return Information about the render target's framebuffer.
		[[nodiscard]] const internal::framebuffer_info& framebuffer_info() const noexcept;

		/// @endcond

	  private:
		/// Information about the render target's framebuffer.
		internal::framebuffer_info m_framebuffer_info;

		/// Viewport of the render target.
		rectangle<int> m_viewport;

		/// Scissor box of the render target.
		rectangle<int> m_scissor_box;
	};
} // namespace tr