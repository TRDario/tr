/// @file
/// @brief Provides `tr::render_target`.

#pragma once
#include <tr/utility/rectangle.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	class framebuffer;
	class graphics_context;
	namespace internal
	{
		enum class graphics_object_id : u32;
	}
} // namespace tr

//

namespace tr
{
	/// Reference to a rendering target.
	/// @details
	/// `tr::render_target` is a reference to a specific rendering target. It is designed to be cheap to pass around by value.
	///
	/// The render target reference should not outlive the framebuffer object it is pointing at, as it will otherwise become dangling.
	class render_target
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// @cond implementation_details

		/// Constructs a render target.
		/// @param context Context the render target is on.
		/// @param framebuffer_id Unique graphics object ID of the framebuffer the render target is on.
		/// @param framebuffer_fbo OpenGL framebuffer object ID of the framebuffer the render target is on.
		/// @param framebuffer_height Height of the framebuffer the render target is on.
		/// @param viewport Viewport of the render target.
		/// @param scissor_box Scissor box of the render target.
		[[nodiscard]] render_target(const graphics_context& context, internal::graphics_object_id framebuffer_id,
									unsigned int framebuffer_fbo, int framebuffer_height, rectangle<u16> viewport,
									rectangle<u16> scissor_box) noexcept;

		/// @endcond

		/// Constructs a backbuffer render target.
		/// @note This is equivalent to calling `context.backbuffer()`.
		/// @param context Graphics context whose backbuffer to create the render target for.
		[[nodiscard]] explicit render_target(const graphics_context& context) noexcept;

		/// Creates a render target on a framebuffer.
		/// @param framebuffer Framebuffer the render target is on.
		/// @param framebuffer_size Size of the framebuffer the render target is on.
		[[nodiscard]] render_target(const framebuffer& framebuffer, glm::ivec2 framebuffer_size) noexcept;

		/// Creates a render target on a framebuffer.
		/// @param framebuffer Framebuffer the render target is on.
		/// @param framebuffer_height Height of the framebuffer the render target is on.
		/// @param viewport Viewport of the framebuffer.
		/// @param scissor_box Scissor box of the framebuffer.
		[[nodiscard]] render_target(const framebuffer& framebuffer, int framebuffer_height, rectangle<u16> viewport,
									rectangle<u16> scissor_box) noexcept;

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
		[[nodiscard]] rectangle<u16> viewport() const noexcept;

		/// Gets the scissor box of the render target within the target framebuffer.
		/// @return Scissor box rectangle of the render target within the target framebuffer.
		[[nodiscard]] rectangle<u16> scissor_box() const noexcept;

		/// @}
		/// @name Subtargets
		/// @{

		/// Creates a new render target with a cropped viewport and full scissor box.
		/// @param viewport Viewport of the render target.
		/// @return New subtarget.
		[[nodiscard]] render_target cropped(rectangle<u16> viewport) const noexcept;

		/// Creates a new render target with the same viewport and a different scissor box.
		/// @param scissor_box Scissor box of the render target.
		/// @return New subtarget.
		[[nodiscard]] render_target scissored(rectangle<u16> scissor_box) const noexcept;

		/// @}
		/// @cond implementation_details

#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Gets a reference to the graphics context the render target is on.
		/// @return Reference to the graphics context the render target is on.
		[[nodiscard]] const graphics_context& context() const noexcept;
#endif

		/// Gets the unique graphics object ID of the framebuffer the render target is on.
		/// @return Unique graphics object ID of the framebuffer the render target is on.
		[[nodiscard]] internal::graphics_object_id framebuffer_id() const noexcept;

		/// Gets the OpenGL framebuffer object ID of the framebuffer the render target is on.
		/// @return OpenGL framebuffer object ID of the framebuffer the render target is on.
		[[nodiscard]] unsigned int framebuffer_fbo() const noexcept;

		/// Gets the height of the framebuffer the render target is on.
		/// @return Height of the framebuffer the render target is on.
		[[nodiscard]] int framebuffer_height() const noexcept;

		/// @endcond

	  private:
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// Reference to the graphics context the render target is on.
		ref<const graphics_context> m_context;
#endif

		/// Unique graphics object ID of the framebuffer the render target is on.
		internal::graphics_object_id m_framebuffer_id;

		/// OpenGL framebuffer object ID of the framebuffer the render target is on.
		unsigned int m_framebuffer_fbo;

		/// Height of the framebuffer the render target is on.
		int m_framebuffer_height;

		/// Viewport of the render target.
		rectangle<u16> m_viewport;

		/// Scissor box of the render target.
		rectangle<u16> m_scissor_box;
	};
} // namespace tr