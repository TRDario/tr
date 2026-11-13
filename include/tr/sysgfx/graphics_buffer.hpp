/// @file
/// @brief Provides `tr::graphics_buffer`.

#pragma once
#include <tr/sysgfx/internal/graphics_object_id.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// @brief Owning container for a generic graphics buffer object.
	/// @details
	/// @note `tr::graphics_buffer` isn't intended to be directly instanced, it is merely documented because some buffers inherit methods
	/// from it.
	///
	/// Every instance of `tr::graphics_buffer` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::graphics_buffer` are left in a special 'invalid' state. Invalid `tr::graphics_buffer` instances may
	/// not be interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	class graphics_buffer
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty graphics buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param label Label of the buffer.
		[[nodiscard]] explicit graphics_buffer(graphics_context& context, std::string_view label = {}) noexcept;

		/// @brief Graphics buffers are not copyable.
		graphics_buffer(const graphics_buffer&) = delete;

		/// @brief Graphics buffers are not copyable.
		graphics_buffer& operator=(const graphics_buffer&) = delete;

		/// @brief Constructs a graphics buffer by transferring ownership from another `tr::graphics_buffer`.
		/// @param rhs `tr::graphics_buffer` to move from.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] graphics_buffer(graphics_buffer&& rhs) noexcept = default;

		/// @brief Destroys the currently held graphics buffer and replaces it with one moved from another `tr::graphics_buffer`.
		/// @param rhs `tr::graphics_buffer` to move from.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		graphics_buffer& operator=(graphics_buffer&& rhs) noexcept = default;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the buffer is in a valid state.
		/// @return `true` if the buffer is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the buffer is in a valid state.
		/// @return `true` if the buffer is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the buffer is on.
		/// @pre The buffer must be in a valid state.
		/// @return Reference to the graphics context the buffer is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Checks whether the buffer is mapped.
		/// @pre The buffer must be in a valid state.
		/// @return `true` if the buffer is mapped, `false` otherwise.
		[[nodiscard]] bool mapped() const noexcept;

		/// @brief Returns the label of the buffer.
		/// @pre The buffer must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the buffer, or "<unnamed>".
		[[nodiscard]] std::string label() const;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL buffer object.
		/// @return Handle to the underlying OpenGL buffer object.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @cond implementation_details
		/// @brief Returns the ID uniquely identifying the buffer.
		/// @return ID uniquely identifying the buffer.
		[[nodiscard]] internal::graphics_object_id id() const noexcept;
		/// @endcond

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the label of the buffer.
		/// @pre The buffer must be in a valid state.
		/// @param label Label of the buffer.
		void set_label(std::string_view label) noexcept;

		/// @cond implementation_details
		/// @brief Replaces the buffer while preserving its label (if applicable).
		/// @param buffer Buffer to transfer the contents of.
		void replace_with(graphics_buffer&& buffer);
		/// @endcond

		/// @}

	  private:
		/// @brief OpenGL buffer object deleter.
		struct gl_buffer_object_deleter
		{
			/// @brief Reference to the context the buffer is on.
			ref<graphics_context> context;

			/// @brief ID uniquely identifying the buffer.
			unique_handle<internal::graphics_object_id, internal::graphics_object_id::invalid> id{internal::generate_graphics_object_id()};

			//

			/// @brief Destroys a buffer object.
			/// @param handle OpenGL buffer object handle.
			void operator()(unsigned int handle) const noexcept;
		};

		//

		/// @brief Managed handle to an OpenGL buffer object.
		unique_handle<unsigned int, 0, gl_buffer_object_deleter> m_handle;
	};
} // namespace tr