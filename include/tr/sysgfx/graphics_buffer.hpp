/// @file
/// @brief Provides `tr::graphics_buffer`.

#pragma once
#include "../utility/handle.hpp"
#include "../utility/reference.hpp"

#ifdef TR_ENABLE_GL_CHECKS
#include "graphics_object_registry.hpp"
#endif

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// Base class for buffers living on a graphics context.
	/// @details
	/// @note `tr::graphics_buffer` isn't intended to be directly instanced, it is merely documented because some buffers inherit methods
	/// from it.
	///
	/// Every instance of `tr::graphics_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// `tr::graphics_buffer` instances are movable, but not copyable. A moved-from instance of `tr::graphics_buffer` is left in a special
	/// 'invalid' state. Invalid `tr::graphics_buffer` instances may not be interacted with besides moving a new value into them and
	/// checking for validity using `valid()`.
	///
	/// `tr::graphics_buffer` instances may be labeled and are formattable. Example format output: `"My buffer" (OpenGL ID: 5)`.

	/// @note `tr::graphics_buffer` isn't intended to be directly instanced, it is primarily documented because some buffers inherit methods
	/// from it.
	class graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs an empty buffer.
		/// @param context Graphics context to create the buffer on.
		graphics_buffer(graphics_context& context);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the buffer is on.
		/// @return Reference to the graphics context the buffer is on.
		graphics_context& context() const;

		/// @}
		/// @name State
		/// @{

		/// Gets whether the buffer is in a valid state.
		/// @return `true` if the buffer is in a valid state, `false` if it is in an invalid state.
		bool valid() const;

		/// @}
		/// @name Label
		/// @{

		/// Gets the label of the buffer.
		/// @return Label of the buffer, or "<unnamed>".
		std::string label() const;

		/// Sets the label of the buffer.
		/// @param label Label of the buffer.
		void set_label(std::string_view label);

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interop
		/// @{

		/// Unwraps the OpenGL buffer object.
		/// @note This does not release the buffer object.
		/// @return OpenGL buffer object ID.
		unsigned int unwrap() const;

		/// @}
		/// @endcond
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		/// Reallocates the buffer while preserving its label (if applicable).
		void reallocate();

#ifdef TR_ENABLE_GL_CHECKS
		/// Gets the unique graphics object ID of the buffer.
		/// @return Unique graphics object ID of the buffer.
		graphics_object_id id() const;
#endif

		/// @}
		/// @endcond

	  private:
		/// Buffer deleter.
		struct deleter
		{
			/// Reference to the context the buffer is on.
			ref<graphics_context> context;

#ifdef TR_ENABLE_GL_CHECKS
			/// Handle to the unique graphics object ID of the buffer.
			graphics_object_id_handle id{};
#endif

			//

			/// Deletes a buffer.
			/// @param id OpenGL buffer ID.
			void operator()(unsigned int bo) const;
		};

		//

		/// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_handle;
	};
} // namespace tr