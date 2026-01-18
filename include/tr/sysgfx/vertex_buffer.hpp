///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides vertex buffer classes.                                                                                                       //
//                                                                                                                                       //
// Vertex buffers are an abstraction over OpenGL VBOs.                                                                                   //
//                                                                                                                                       //
// The vertex buffer comes in two variants: static vertex buffers are initialized once and are immutable, while dynamic vertex buffers   //
// can be resized and modified at will. An untyped (prefixed with basic_) and typed variant of each of these is provided:                //
//     - tr::gfx::basic_static_vertex_buffer -> untyped static vertex buffer                                                             //
//     - tr::gfx::static_vertex_buffer<T> -> typed static vertex buffer                                                                  //
//     - tr::gfx::basic_dyn_vertex_buffer -> untyped dynamic vertex buffer                                                               //
//     - tr::gfx::dyn_vertex_buffer<T> -> typed dynamic vertex buffer                                                                    //
//                                                                                                                                       //
// Static vertex buffers are constructed with a span of data that will be copied into the buffer:                                        //
//     - tr::gfx::static_vertex_buffer<glm::vec2> buffer{data} -> copies 'data' into the buffer                                          //
//                                                                                                                                       //
// Dynamic vertex buffers are constructed empty. Whether a buffer is empty can be checked with the .empty() method. Much like            //
// std::vector, dynamic vertex buffers distinguish between buffer size and capacity, both of which can be queried with the respective    //
// method. The buffer automatically reallocates itself if its current capacity is insufficient, but the user can reserve a capacity in   //
// advance with the .advance() method. Note that unlike std::vector, this clears any previous buffer data. The buffer can be resized     //
// with .resize() or set to a copy of a span with .set(), which automatically sets its size to match that of the copied buffer.          //
// The .set_region() method can be used to set a region of the buffer; this function never affects the size or capacity of the buffer.   //
// The buffer can be cleared with the .clear() method, but like with std::vector, the allocated capacity is retained:                    //
//     - tr::gfx::dyn_vertex_buffer<glm::vec2> buffer; buffer.empty() -> true                                                            //
//     - buffer.reserve(100); buffer.resize(50) -> buffer now has size 50, capacity 128                                                  //
//     - buffer.size(); buffer.capacity() -> 50, 128                                                                                     //
//     - std::array<glm::vec2, 500> data; buffer.set(data) -> buffer now stores a copy of 'data', has size 500, capacity 512             //
//     - std::array<glm::vec2, 100> data2; buffer.set_region(400, data2) -> a copy of data2 is now in buffer[400-499]                    //
//     - buffer.clear() -> buffer now has size 0, capacity 512                                                                           //
//                                                                                                                                       //
// The label of a vertex buffer can be set with TR_SET_LABEL(vbuf, label):                                                               //
//     - TR_SET_LABEL(vbuf, "Example buffer") -> 'atlas' is now labelled "Example buffer"                                                //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"
#include "../utility/ranges.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Static vertex buffer class for holding immutable vertex data.
	class basic_static_vertex_buffer {
	  public:
		// Uploads vertex data into a static vertex buffer.
		basic_static_vertex_buffer(std::span<const std::byte> data);

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the vertex buffer.
		void set_label(std::string_view label);
		// Gets the debug label of the vertex buffer.
		std::string label() const;
#endif

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_vbo;
		// The size of the vertex buffer.
		ssize m_size;

		friend void set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, ssize offset, usize stride);
	};

	// Typed static vertex buffer class for holding immutable vertex data of a single type.
	template <standard_layout T> class static_vertex_buffer : public basic_static_vertex_buffer {
	  public:
		// Creates a static vertex buffer.
		template <typed_contiguous_range<T> R> static_vertex_buffer(R&& range);
	};

	// Dynamic vertex buffer class.
	class basic_dyn_vertex_buffer {
	  public:
		// Creates a dynamic vertex buffer.
		basic_dyn_vertex_buffer();

		// Gets whether the vertex buffer is empty.
		bool empty() const;
		// Gets the size of the vertex buffer contents.
		usize size() const;
		// Gets the capacity of the vertex buffer.
		usize capacity() const;

		// Sets the size of the vertex buffer to 0.
		void clear();
		// Clears the buffer and resizes it, potentially resizing it.
		void resize(usize size);
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(usize capacity);
		// Sets the contents of the buffer, potentially reallocating it.
		void set(std::span<const std::byte> data);
		// Sets a region of the buffer.
		void set_region(usize offset, std::span<const std::byte> data);

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the vertex buffer.
		void set_label(std::string_view label);
		// Gets the debug label of the vertex buffer.
		std::string label() const;
#endif

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_vbo;
		// The used size of the buffer.
		usize m_size;
		// The capacity of the buffer.
		usize m_capacity;

		friend void set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, ssize offset, usize stride);
	};

	// Typed dynamic vertex buffer class.
	template <standard_layout T> class dyn_vertex_buffer : public basic_dyn_vertex_buffer {
	  public:
		using basic_dyn_vertex_buffer::basic_dyn_vertex_buffer;

		// Gets the size of the vertex buffer contents.
		usize size() const;
		// Gets the capacity of the vertex buffer.
		usize capacity() const;

		// Clears the buffer and resizes it, potentially reallocating it.
		void resize(usize size);
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(usize capacity);
		// Sets the contents of the buffer, potentially reallocating it.
		template <typed_contiguous_range<T> R> void set(R&& data);
		// Sets a region of the buffer.
		template <typed_contiguous_range<T> R> void set_region(usize offset, R&& data);

	  private:
		using basic_dyn_vertex_buffer::capacity;
		using basic_dyn_vertex_buffer::reserve;
		using basic_dyn_vertex_buffer::resize;
		using basic_dyn_vertex_buffer::set_region;
		using basic_dyn_vertex_buffer::size;
	};
} // namespace tr::gfx

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::standard_layout T>
template <tr::typed_contiguous_range<T> R>
tr::gfx::static_vertex_buffer<T>::static_vertex_buffer(R&& range)
	: basic_static_vertex_buffer{range_bytes(range)}
{
}

template <tr::standard_layout T> tr::usize tr::gfx::dyn_vertex_buffer<T>::size() const
{
	return basic_dyn_vertex_buffer::size() / sizeof(T);
}

template <tr::standard_layout T> tr::usize tr::gfx::dyn_vertex_buffer<T>::capacity() const
{
	return basic_dyn_vertex_buffer::capacity() / sizeof(T);
}

template <tr::standard_layout T> void tr::gfx::dyn_vertex_buffer<T>::resize(usize size)
{
	basic_dyn_vertex_buffer::resize(size * sizeof(T));
}

template <tr::standard_layout T> void tr::gfx::dyn_vertex_buffer<T>::reserve(usize size)
{
	basic_dyn_vertex_buffer::reserve(size * sizeof(T));
}

template <tr::standard_layout T> template <tr::typed_contiguous_range<T> R> void tr::gfx::dyn_vertex_buffer<T>::set(R&& data)
{
	basic_dyn_vertex_buffer::set(range_bytes(data));
}

template <tr::standard_layout T>
template <tr::typed_contiguous_range<T> R>
void tr::gfx::dyn_vertex_buffer<T>::set_region(usize offset, R&& data)
{
	basic_dyn_vertex_buffer::set_region(offset * sizeof(T), range_bytes(data));
}