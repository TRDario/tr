///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides GPU buffers accessible to shaders.                                                                                           //
//                                                                                                                                       //
// The shader buffer comes in three variants: the untyped tr::gfx::basic_shader_buffer, the typed                                        //
// tr::gfx::shader_buffer<Header, ArrayElement>, and the typed, but headerless tr::gfx::shader_array<ArrayElement>. The former two are   //
// divided into a fixed-size header and a fixed-capacity, variable-size array, while the latter only has the array.                      //
//                                                                                                                                       //
// Buffers are constructed with their capacity and a map type: buffer maps can be read/write-only, or both readable and writable:        //
//     - tr::gfx::basic_shader_buffer{64, 128, tr::gfx::map_type::write_only}                                                            //
//       -> creates a shader buffer with a header of 64 bytes and an array with capacity 128 bytes with write-only mapping               //
//     - tr::gfx::shader_buffer<T, E>{128, tr::gfx::map_type::read_only}                                                                 //
//       -> creates a shader buffer with a header of sizeof(T) bytes and an array with capacity 128 * sizeof(E) with read-only mapping   //
//     - tr::gfx::shader_array<E>{64, tr::gfx::map_type::read_write}                                                                     //
//       -> creates a shader array with capacity 64 * sizeof(E) with read+write mapping                                                  //
//                                                                                                                                       //
// The size and capacity of a buffer array can be queried, as can the size of the header in a basic buffer:                              //
//     - tr::gfx::basic_shader_buffer{64, 128}.header_size() -> 64                                                                       //
//     - tr::gfx::shader_buffer<T, E>{128}.array_size() -> 0, starts empty                                                               //
//     - tr::gfx::shader_buffer<T, E>{128}.array_capacity() -> 128                                                                       //
//     - tr::gfx::shader_array<E>{128}.size() -> 0, starts empty                                                                         //
//     - tr::gfx::shader_array<E>{128}.capacity() -> 128                                                                                 //
//                                                                                                                                       //
// Parts of a buffer (or the whole buffer for the basic shader buffer) may be mapped. Only one map of the buffer may exist at one time,  //
// and whether a buffer is mapped can be checked with the .mapped() method:                                                              //
//     - tr::gfx::shader_buffer<T, E> buffer{128}; buffer.map_header() -> tr::gfx::buffer_object_map<T>                                  //
//     - tr::gfx::shader_buffer<T, E> buffer2{128}; buffer2.map_array() -> tr::gfx::buffer_span_map<E>                                   //
//     - tr::gfx::shader_array<E> buffer3{128}; buffer3.map() -> tr::gfx::buffer_span_map<E>                                             //
//     - tr::gfx::basic_shader_buffer buffer4{64, 128}; buffer4.map() -> tr::gfx::basic_buffer_map                                       //
//     - buffer.mapped() -> true                                                                                                         //
//                                                                                                                                       //
// Parts of the buffer may also be directly set, or the array resized to a certain size:                                                 //
//     - tr::gfx::shader_buffer<glm::mat4, float> buf{64}; buf.set_header(glm::mat4{1.0f}) -> sets the header to an identity matrix      //
//     - tr::gfx::shader_buffer<glm::mat4, float> buf{64}; buf.resize_array(32) -> resizes the buffer's array to 32 elements             //
//     - tr::gfx::shader_buffer<glm::mat4, float> buf{64}; buf.set_array(some_array) -> resizes and sets the buffer's array              //
//     - tr::gfx::shader_array<float> buf{64}; buf.resize(32) -> resizes the array buffer to 32 elements                                 //
//     - tr::gfx::shader_array<float> buf{64}; buf.set(some_array) -> resizes and sets the array buffer                                  //
//                                                                                                                                       //
// The label of a shader buffer can be set with TR_SET_LABEL(sbuf, label):                                                               //
//     - TR_SET_LABEL(sbuf, "Example buffer") -> 'sbuf' is now labelled "Example buffer"                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "buffer_map.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// GPU buffer accessable to a shader.
	class basic_shader_buffer {
	  public:
		// Allocates an uninitialized shader buffer.
		basic_shader_buffer(usize header_size, usize capacity, map_type map_type = map_type::write_only);

		// Gets the size of the fixed header block.
		usize header_size() const;
		// Gets the size of the dynamic array.
		usize array_size() const;
		// Gets the maximum capacity of the dynamic array.
		usize array_capacity() const;

		// Sets the data of the header.
		void set_header(std::span<const std::byte> data);
		// Sets the data of the dynamic array.
		void set_array(std::span<const std::byte> data);
		// Resizes the dynamic array.
		void resize_array(usize size);

		// Gets whether the buffer is mapped.
		bool mapped() const;
		// Maps the fixed header.
		basic_buffer_map map_header();
		// Maps the dynamic array.
		basic_buffer_map map_array();
		// Maps the entire buffer.
		basic_buffer_map map();

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the shader buffer.
		void set_label(std::string_view label);
		// Gets the debug label of the shader buffer.
		std::string label() const;
#endif

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_sbo;
		// The map type of the buffer.
		map_type m_map_type;
		// The size of the header.
		usize m_header_size;
		// The current size of the array.
		usize m_array_size;
		// The capacity of the array.
		usize m_array_capacity;

		// Maps a range of the buffer.
		basic_buffer_map map_range(usize offset, usize size);

		friend class shader_base;
	};

	// Shader buffer with a typed header and array.
	template <class Header, class ArrayElement> class shader_buffer : public basic_shader_buffer {
	  public:
		// Allocates an uninitialized shader buffer.
		shader_buffer(usize capacity, map_type map_type = map_type::write_only);

		// Gets the size of the dynamic array.
		usize array_size() const;
		// Gets the maximum capacity of the dynamic array.
		usize array_capacity() const;

		// Sets the data of the header.
		void set_header(const Header& header);
		// Sets the data of the dynamic array.
		template <typed_contiguous_range<ArrayElement> R> void set_array(R&& data);
		// Resizes the dynamic array.
		void resize_array(usize size);

		// Maps the fixed header.
		buffer_object_map<Header> map_header();
		// Maps the dynamic array.
		buffer_span_map<ArrayElement> map_array();

	  private:
		using basic_shader_buffer::array_capacity;
		using basic_shader_buffer::array_size;
		using basic_shader_buffer::header_size;
		using basic_shader_buffer::map;
		using basic_shader_buffer::map_array;
		using basic_shader_buffer::map_header;
		using basic_shader_buffer::resize_array;
		using basic_shader_buffer::set_array;
		using basic_shader_buffer::set_header;
	};

	// Specialized shader buffer with no header before the array.
	template <class T> class shader_array : public basic_shader_buffer {
	  public:
		// Allocates an uninitialized shader array.
		shader_array(usize capacity, map_type map_type = map_type::write_only);

		// Gets the size of the array.
		usize size() const;
		// Gets the maximum capacity of the array.
		usize capacity() const;

		// Sets the data of the array.
		template <typed_contiguous_range<T> R> void set(R&& data);
		// Resizes the array.
		void resize(usize size);

		// Maps the array.
		buffer_span_map<T> map();

	  private:
		using basic_shader_buffer::array_capacity;
		using basic_shader_buffer::array_size;
		using basic_shader_buffer::header_size;
		using basic_shader_buffer::map;
		using basic_shader_buffer::map_array;
		using basic_shader_buffer::map_header;
		using basic_shader_buffer::resize_array;
		using basic_shader_buffer::set_array;
		using basic_shader_buffer::set_header;
	};
} // namespace tr::gfx

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class Header, class ArrayElement>
tr::gfx::shader_buffer<Header, ArrayElement>::shader_buffer(usize capacity, map_type map_type)
	: basic_shader_buffer{sizeof(Header), sizeof(ArrayElement) * capacity, map_type}
{
}

template <class Header, class ArrayElement> tr::usize tr::gfx::shader_buffer<Header, ArrayElement>::array_size() const
{
	return basic_shader_buffer::array_size() / sizeof(ArrayElement);
}

template <class Header, class ArrayElement> tr::usize tr::gfx::shader_buffer<Header, ArrayElement>::array_capacity() const
{
	return basic_shader_buffer::array_capacity() / sizeof(ArrayElement);
}

template <class Header, class ArrayElement> void tr::gfx::shader_buffer<Header, ArrayElement>::set_header(const Header& header)
{
	basic_shader_buffer::set_header(as_bytes(header));
}

template <class Header, class ArrayElement>
template <tr::typed_contiguous_range<ArrayElement> R>
void tr::gfx::shader_buffer<Header, ArrayElement>::set_array(R&& data)
{
	basic_shader_buffer::set_array(range_bytes(data));
}

template <class Header, class ArrayElement> void tr::gfx::shader_buffer<Header, ArrayElement>::resize_array(usize size)
{
	basic_shader_buffer::resize_array(size * sizeof(ArrayElement));
}

template <class Header, class ArrayElement> tr::gfx::buffer_object_map<Header> tr::gfx::shader_buffer<Header, ArrayElement>::map_header()
{
	return basic_shader_buffer::map_header();
}

template <class Header, class ArrayElement> tr::gfx::buffer_span_map<ArrayElement> tr::gfx::shader_buffer<Header, ArrayElement>::map_array()
{
	return basic_shader_buffer::map_array();
}

//

template <class T>
tr::gfx::shader_array<T>::shader_array(usize capacity, map_type map_type)
	: basic_shader_buffer{0, sizeof(T) * capacity, map_type}
{
}

template <class T> tr::usize tr::gfx::shader_array<T>::size() const
{
	return basic_shader_buffer::array_size() / sizeof(T);
}

template <class T> tr::usize tr::gfx::shader_array<T>::capacity() const
{
	return basic_shader_buffer::array_capacity() / sizeof(T);
}

template <class T> template <tr::typed_contiguous_range<T> R> void tr::gfx::shader_array<T>::set(R&& data)
{
	basic_shader_buffer::set_array(range_bytes(data));
}

template <class T> void tr::gfx::shader_array<T>::resize(usize size)
{
	basic_shader_buffer::resize_array(size * sizeof(T));
}

template <class T> tr::gfx::buffer_span_map<T> tr::gfx::shader_array<T>::map()
{
	return basic_shader_buffer::map_array();
}