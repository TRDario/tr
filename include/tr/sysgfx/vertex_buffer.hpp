/// @file
/// @brief Provides vertex buffer classes.

#pragma once
#include "../utility/concepts.hpp"
#include "../utility/specialization_of.hpp"
#include "graphics_buffer.hpp"

//

namespace tr
{
	/// Static vertex buffer class for holding immutable vertex data.
	class basic_static_vertex_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Uploads vertex data into a static vertex buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param data Data to upload to the buffer.
		basic_static_vertex_buffer(graphics_context& context, std::span<const std::byte> data);

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
		/// @name OpenGL interoperability
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
		/// Size of the vertex buffer in bytes.
		ssize m_size;
	};

	/// Typed static vertex buffer class for holding immutable vertex data of a single type.
	/// @tparam Element Type of the elements of the buffer.
	template <standard_layout Element>
	class static_vertex_buffer : private basic_static_vertex_buffer
	{
	  public:
		/// Value type used by the buffer.
		using value_type = Element;

		/// @name Constructors
		/// @{

		/// Uploads vertex data into a static vertex buffer.
		/// @tparam Range Vertex buffer data range.
		/// @param context Graphics context to create the buffer on.
		/// @param range Data to upload to the buffer.
		template <typed_contiguous_const_range<Element> Range>
		static_vertex_buffer(graphics_context& context, Range&& range);

		/// @}
		/// @name Context
		/// @{

		using basic_static_vertex_buffer::context;

		/// @}
		/// @name State
		/// @{

		using basic_static_vertex_buffer::valid;

		/// @}
		/// @name Label
		/// @{

		using basic_static_vertex_buffer::label;

		using basic_static_vertex_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using basic_static_vertex_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using basic_static_vertex_buffer::id;

		/// @}
		/// @endcond
#endif
	};

	//

	/// Dynamic vertex buffer class.
	class basic_dyn_vertex_buffer : private graphics_buffer
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
		/// @name State
		/// @{

		using graphics_buffer::valid;

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the vertex buffer is empty.
		/// @return `true` if thevertex buffer is empty, `false` otherwise.
		bool empty() const;

		/// Gets the size of the vertex buffer contents.
		/// @return Size of the vertex buffer in bytes.
		usize size() const;

		/// Gets the capacity of the vertex buffer.
		/// @return Capacity of the vertex buffer in bytes.
		usize capacity() const;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the size of the vertex buffer to 0.
		void clear();

		/// Clears the buffer and resizes it, potentially resizing it.
		/// @param size New size of the buffer in bytes.
		void resize(usize size);

		/// Clears the buffer and guarantees a certain capacity for it.
		/// @param capacity New capacity of the buffer in bytes.
		void reserve(usize capacity);

		/// Sets the contents of the buffer, potentially reallocating it.
		/// @param data Data to copy into the buffer.
		void set(std::span<const std::byte> data);

		/// Sets a region of the buffer.
		/// @param offset Starting byte offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre `offset + data.size()` must be less than or equal to the size of the buffer.
		void set_region(usize offset, std::span<const std::byte> data);

		/// @}
		/// @name Label
		/// @{

		using graphics_buffer::label;

		using graphics_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
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
		/// Used size of the buffer in bytes.
		usize m_size{0};

		/// Capacity of the buffer in bytes.
		usize m_capacity{0};
	};

	/// Typed dynamic vertex buffer class.
	/// @tparam Element Type of the elements of the buffer.
	template <standard_layout Element>
	class dyn_vertex_buffer : private basic_dyn_vertex_buffer
	{
	  public:
		/// Value type used by the buffer.
		using value_type = Element;

		/// @name Constructors
		/// @{

		using basic_dyn_vertex_buffer::basic_dyn_vertex_buffer;

		/// @}
		/// @name Context
		/// @{

		using basic_dyn_vertex_buffer::context;

		/// @}
		/// @name State
		/// @{

		using basic_dyn_vertex_buffer::valid;

		/// @}
		/// @name Size
		/// @{

		using basic_dyn_vertex_buffer::empty;

		/// Gets the size of the vertex buffer contents.
		/// @return Size of the vertex buffer in elements.
		usize size() const;

		/// Gets the capacity of the vertex buffer.
		/// @return Capacity of the vertex buffer in elements.
		usize capacity() const;

		/// @}
		/// @name Setting
		/// @{

		using basic_dyn_vertex_buffer::clear;

		/// Clears the buffer and resizes it, potentially reallocating it.
		/// @param size New size of the buffer in elements.
		void resize(usize size);

		/// Clears the buffer and guarantees a certain capacity for it.
		/// @param capacity New capacity of the buffer in elements.
		void reserve(usize capacity);

		/// Sets the contents of the buffer, potentially reallocating it.
		/// @tparam Range Vertex buffer data range.
		/// @param data Data to copy into the buffer.
		template <typed_contiguous_const_range<Element> Range>
		void set(Range&& data);

		/// Sets a region of the buffer.
		/// @tparam Range Vertex buffer data range.
		/// @param offset Starting element offset within the buffer.
		/// @param data Data to copy into the buffer.
		/// @pre `offset + data.size()` must be less than or equal to the size of the buffer.
		template <typed_contiguous_const_range<Element> Range>
		void set_region(usize offset, Range&& data);

		/// @}
		/// @name Label
		/// @{

		using basic_dyn_vertex_buffer::label;

		using basic_dyn_vertex_buffer::set_label;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		using basic_dyn_vertex_buffer::unwrap;

		/// @}
		/// @endcond
#ifdef TR_ENABLE_CHECKED_GRAPHICS
		/// @cond implementation_details
		/// @name Implementation details
		/// @{

		using basic_dyn_vertex_buffer::id;

		/// @}
		/// @endcond
#endif
	};

	//

	/// Untyped vertex buffer type.
	template <typename T>
	concept any_untyped_vertex_buffer = one_of<T, basic_static_vertex_buffer, basic_dyn_vertex_buffer>;

	/// Typed vertex buffer type.
	template <typename T>
	concept any_typed_vertex_buffer = specialization_of<T, static_vertex_buffer> || specialization_of<T, dyn_vertex_buffer>;

	/// Vertex buffer type.
	template <typename T>
	concept any_vertex_buffer = any_untyped_vertex_buffer<T> || any_typed_vertex_buffer<T>;
} // namespace tr

#include "impl/vertex_buffer.hpp" // IWYU pragma: export