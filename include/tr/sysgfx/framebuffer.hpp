/// @file
/// @brief Provides `tr::framebuffer`.

#pragma once
#include "../utility/handle.hpp"
#include "../utility/reference.hpp"

#ifdef TR_ENABLE_CHECKED_GRAPHICS
#include "graphics_object_registry.hpp"
#endif

namespace tr
{
	class graphics_context;
	class texture_view;
} // namespace tr

//

namespace tr
{
	/// Container for a framebuffer object.
	/// @details
	/// A framebuffer is a collection of references to buffers used as a destination for rendering. The default framebuffer is used to
	/// render to the screen, while instances of `tr::framebuffer` may have arbitrary textures attached to them as rendering targets.
	///
	/// Strictly speaking, instances of `tr::framebuffer` are only containers for these underlying framebuffers. This means, for example,
	/// that setting a render target on a graphics context does not set a render target of a literal `tr::framebuffer` object at a specific
	/// location in memory, but rather the value it contains. If the value is moved to a different instance of `tr::framebuffer`, that value
	/// will still be set on the context. If the instance is overriden with a new value, the old value is destroyed and the graphics context
	/// will no longer have a set render target.
	///
	/// Every instance of `tr::framebuffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// `tr::framebuffer` instances are movable, but not copyable. A moved-from instance of `tr::framebuffer` is left in a special 'invalid'
	/// state. Invalid `tr::framebuffer` instances may not be interacted with besides moving a new value into them and checking for validity
	/// using `valid()`.
	///
	/// `tr::framebuffer` instances may be labeled and are formattable. Example format output: `"My %framebuffer" (OpenGL ID: 5)`.
	class framebuffer
	{
	  public:
		/// Framebuffer attachments.
		enum class attachment
		{
			/// First color attachment.
			color0 = 36064,
			/// Second color attachment.
			color1,
			/// Third color attachment.
			color2,
			/// Fourth color attachment.
			color3,
			/// Fifth color attachment.
			color4,
			/// Sixth color attachment.
			color5,
			/// Seventh color attachment.
			color6,
			/// Eighth color attachment.
			color7,
			/// Depth attachment.
			depth = 36096,
			/// Stencil attachment.
			stencil = 36128,
		};

		/// @name Constructors
		/// @{

		/// Creates an empty framebuffer.
		/// @param context Graphics context to create the framebuffer on.
		framebuffer(graphics_context& context);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the framebuffer is on.
		graphics_context& context() const;

		/// @}
		/// @name Attachments
		/// @{

		/// Attaches a texture to the framebuffer.
		/// @param attachment Attachment to set.
		/// @param texture Texture to attach to the framebuffer.
		void attach(attachment attachment, texture_view texture);

		/// Detaches whatever is on the specified attachment.
		/// @param attachment Attachment to clear.
		void detach(attachment attachment);

		/// @}
		/// @name State
		/// @{

		/// Gets whether the framebuffer is in a valid state.
		/// @return `true` if the framebuffer is in a valid state, `false` if it is in an invalid state.
		bool valid() const;

		/// @}
		/// @name Label
		/// @{

		/// Sets the debug label of the framebuffer.
		/// @param label Debug label of the framebuffer.
		void set_label(std::string_view label);

		/// Gets the debug label of the framebuffer.
		/// @return Debug label of the framebuffer.
		std::string label() const;

		/// @}
		/// @cond gl_interop

		/// Unwraps the OpenGL framebuffer.
		/// @note This does not release the framebuffer.
		/// @return OpenGL framebuffer ID.
		unsigned int unwrap() const;

		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details

		/// Gets the unique graphics object ID of the framebuffer.
		/// @return Unique graphics object ID of the framebuffer.
		graphics_object_id id() const;

		/// @endcond
#endif

	  private:
		/// Framebuffer deleter.
		struct deleter
		{
			/// Reference to the graphics context the framebuffer is on.
			ref<graphics_context> context;

#ifdef TR_ENABLE_CHECKED_GRAPHICS
			/// Handle to the unique graphics object ID of the framebuffer.
			graphics_object_id_handle id{};
#endif

			//

			/// Deletes a framebuffer.
			/// @param fbo OpenGL framebuffer ID.
			void operator()(unsigned int fbo) const;
		};

		//

		/// Handle to the OpenGL framebuffer.
		handle<unsigned int, 0, deleter> m_handle;
	};
} // namespace tr

#include "impl/framebuffer.hpp" // IWYU pragma: export