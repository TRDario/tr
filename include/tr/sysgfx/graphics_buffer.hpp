/// @file
/// @brief Provides a generic OpenGL buffer object.

#pragma once
#include "../utility/handle.hpp"

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// Wrapper around a generic OpenGL buffer object.
	/// @note This class isn't meant to be directly used, it is primarily documented because some buffers inherit methods from it.
	class graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs a buffer.
		/// @param context Graphics context to create the buffer on.
		graphics_buffer(graphics_context& context);

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the buffer is on.
		/// @return Reference to the context the buffer is on.
		graphics_context& context() const;

		/// @}
		/// @name ID
		/// @{

		/// Gets the buffer's ID.
		/// @return OpenGL buffer ID.
		unsigned int id() const;

		/// Reallocates the buffer while preserving its label (if applicable).
		void reallocate();

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

	  private:
		/// Buffer deleter.
		struct deleter
		{
			/// Reference to the context the buffer is on.
			graphics_context& context;

			//

			/// Deletes a buffer.
			/// @param id OpenGL buffer ID.
			void operator()(unsigned int id) const;
		};

		//

		/// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_handle;
	};
} // namespace tr