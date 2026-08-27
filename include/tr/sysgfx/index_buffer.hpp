/// @file
/// @brief Provides index buffer classes.

#pragma once
#include "../utility/concepts.hpp"
#include "graphics_buffer.hpp"

//

namespace tr
{
	/// Static index buffer class for holding immutable index data.
	class static_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Uploads index data into a static index buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to copy into the buffer.
		static_index_buffer(graphics_context& context, std::span<const u16> data);

		/// @}
		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name State
		/// @{

		using graphics_buffer::valid;

		/// @}
		/// @name Label
		/// @{

		using graphics_buffer::label;

		using graphics_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interop
		/// @{

		using graphics_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using graphics_buffer::id;

		/// @}
		/// @endcond
#endif

	  private:
		/// Size of the buffer in elements.
		ssize m_size;
	};

	//

	/// Dynamic index buffer class.
	class dyn_index_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		using graphics_buffer::graphics_buffer;

		/// @}
		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the index buffer is empty.
		/// @return `true` if the buffer is empty, `false` otherwise.
		bool empty() const;

		/// Gets the size of the index buffer contents.
		/// @return Size of the index buffer in elements.
		usize size() const;

		/// Gets the capacity of the index buffer.
		/// @return Capacity of the index buffer in elements.
		usize capacity() const;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the size of the index buffer to 0.
		void clear();

		/// Clears the buffer and resizes it, potentially reallocating in the process.
		/// @param size New size of the buffer in elements.
		void resize(usize size);

		/// Clears the buffer and guarantees a certain capacity for it.
		/// @param capacity New capacity of the buffer in elements.
		void reserve(usize capacity);

		/// Sets the contents of the buffer, potentially reallocating in the process.
		/// @param data Data to copy into the buffer.
		void set(std::span<const u16> data);

		/// Sets a region of the buffer.
		/// @param offset Starting element offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre `offset + data.size()` must be less than or equal to the size of the buffer.
		void set_region(usize offset, std::span<const u16> data);

		/// @}
		/// @name State
		/// @{

		using graphics_buffer::valid;

		/// @}
		/// @name Label
		/// @{

		using graphics_buffer::label;

		using graphics_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interop
		/// @{

		using graphics_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using graphics_buffer::id;

		/// @}
		/// @endcond
#endif

	  private:
		/// Used size of the buffer in elements.
		usize m_size{0};

		/// Capacity of the buffer in elements.
		usize m_capacity{0};
	};

	/// Index buffer type.
	template <typename T>
	concept any_index_buffer = one_of<T, static_index_buffer, dyn_index_buffer>;
} // namespace tr

#include "impl/index_buffer.hpp" // IWYU pragma: export