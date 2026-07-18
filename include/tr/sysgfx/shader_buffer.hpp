///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides GPU buffers accessible to shaders.                                                                                           //
//                                                                                                                                       //
// The shader buffer comes in three variants: the untyped tr::basic_shader_buffer, the typed  tr::shader_buffer<Header, ArrayElement>,   //
// and the typed, but headerless tr::shader_array<ArrayElement>. The former two are divided into a fixed-size header and a               //
// fixed-capacity, variable-size array, while the latter only has the array.                                                             //
//                                                                                                                                       //
// Buffers are constructed with their capacity and a map type: buffer maps can be read/write-only, or both readable and writable:        //
//     - tr::basic_shader_buffer{context, 64, 128, tr::map_type::write_only}                                                             //
//       -> creates a shader buffer with a header of 64 bytes and an array with capacity 128 bytes with write-only mapping               //
//     - tr::shader_buffer<T, E>{context, 128, tr::map_type::read_only}                                                                  //
//       -> creates a shader buffer with a header of sizeof(T) bytes and an array with capacity 128 * sizeof(E) with read-only mapping   //
//     - tr::shader_array<E>{context, 64, tr::map_type::read_write}                                                                      //
//       -> creates a shader array with capacity 64 * sizeof(E) with read+write mapping                                                  //
//                                                                                                                                       //
// The size and capacity of a buffer array can be queried, as can the size of the header in a basic buffer:                              //
//     - tr::basic_shader_buffer{context, 64, 128}.header_size() -> 64                                                                   //
//     - tr::shader_buffer<T, E>{context, 128}.array_size() -> 0, starts empty                                                           //
//     - tr::shader_buffer<T, E>{context, 128}.array_capacity() -> 128                                                                   //
//     - tr::shader_array<E>{context, 128}.size() -> 0, starts empty                                                                     //
//     - tr::shader_array<E>{context, 128}.capacity() -> 128                                                                             //
//                                                                                                                                       //
// Parts of a buffer (or the whole buffer for the basic shader buffer) may be mapped. Only one map of the buffer may exist at one time,  //
// and whether a buffer is mapped can be checked with the .mapped() method:                                                              //
//     - tr::shader_buffer<T, E> buffer{context, 128}; buffer.map_header() -> tr::buffer_object_map<T>                                   //
//     - tr::shader_buffer<T, E> buffer2{context, 128}; buffer2.map_array() -> tr::buffer_span_map<E>                                    //
//     - tr::shader_array<E> buffer3{context, 128}; buffer3.map() -> tr::buffer_span_map<E>                                              //
//     - tr::basic_shader_buffer buffer4{context, 64, 128}; buffer4.map() -> tr::basic_buffer_map                                        //
//     - buffer.mapped() -> true                                                                                                         //
//                                                                                                                                       //
// Parts of the buffer may also be directly set, or the array resized to a certain size:                                                 //
//     - tr::shader_buffer<glm::mat4, float> buf{context, 64}; buf.set_header(glm::mat4{1.0f}) -> sets the header to an identity matrix  //
//     - tr::shader_buffer<glm::mat4, float> buf{context, 64}; buf.resize_array(32) -> resizes the buffer's array to 32 elements         //
//     - tr::shader_buffer<glm::mat4, float> buf{context, 64}; buf.set_array(some_array) -> resizes and sets the buffer's array          //
//     - tr::shader_array<float> buf{context, 64}; buf.resize(32) -> resizes the array buffer to 32 elements                             //
//     - tr::shader_array<float> buf{context, 64}; buf.set(some_array) -> resizes and sets the array buffer                              //
//                                                                                                                                       //
// The label of a shader buffer can be set with .set_label():                                                                            //
//     - sbuf.set_label("Example buffer") -> 'sbuf' is now labelled "Example buffer"                                                     //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/concepts.hpp"
#include "graphics_buffer.hpp"
#include "graphics_buffer_map.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// GPU buffer accessable to a shader.
	class basic_shader_buffer : private graphics_buffer {
	  public:
		// Allocates an uninitialized shader buffer.
		basic_shader_buffer(graphics_context& context, usize header_size, usize capacity, map_type map_type = map_type::write_only);

		// Gets a reference to the graphics context the buffer is on.
		using graphics_buffer::context;

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
		basic_graphics_buffer_map map_header();
		// Maps the dynamic array.
		basic_graphics_buffer_map map_array();
		// Maps the entire buffer.
		basic_graphics_buffer_map map();

		// Gets the debug label of the shader buffer.
		using graphics_buffer::label;
		// Sets the debug label of the shader buffer.
		using graphics_buffer::set_label;

	  private:
		// The map type of the buffer.
		map_type m_map_type;
		// The size of the header.
		usize m_header_size;
		// The current size of the array.
		usize m_array_size;
		// The capacity of the array.
		usize m_array_capacity;

		// Maps a range of the buffer.
		basic_graphics_buffer_map map_range(usize offset, usize size);

		friend class shader_base;
	};

	// Shader buffer with a typed header and array.
	template <typename Header, typename ArrayElement> class shader_buffer : private basic_shader_buffer {
	  public:
		// Allocates an uninitialized shader buffer.
		shader_buffer(graphics_context& context, usize capacity, map_type map_type = map_type::write_only);

		// Gets a reference to the graphics context the buffer is on.
		using basic_shader_buffer::context;

		// Gets the size of the dynamic array.
		usize array_size() const;
		// Gets the maximum capacity of the dynamic array.
		usize array_capacity() const;

		// Sets the data of the header.
		void set_header(const Header& header);
		// Sets the data of the dynamic array.
		template <typed_contiguous_const_range<ArrayElement> R> void set_array(R&& data);
		// Resizes the dynamic array.
		void resize_array(usize size);

		// Gets whether the buffer is mapped.
		using basic_shader_buffer::mapped;
		// Maps the fixed header.
		graphics_buffer_object_map<Header> map_header();
		// Maps the dynamic array.
		graphics_buffer_span_map<ArrayElement> map_array();

		// Gets the debug label of the shader buffer.
		using basic_shader_buffer::label;
		// Sets the debug label of the shader buffer.
		using basic_shader_buffer::set_label;
	};

	// Specialized shader buffer with no header before the array.
	template <typename Element> class shader_array : private basic_shader_buffer {
	  public:
		// Allocates an uninitialized shader array.
		shader_array(graphics_context& context, usize capacity, map_type map_type = map_type::write_only);

		// Gets a reference to the graphics context the buffer is on.
		using basic_shader_buffer::context;

		// Gets the size of the array.
		usize size() const;
		// Gets the maximum capacity of the array.
		usize capacity() const;

		// Sets the data of the array.
		template <typed_contiguous_const_range<Element> R> void set(R&& data);
		// Resizes the array.
		void resize(usize size);

		// Gets whether the buffer is mapped.
		using basic_shader_buffer::mapped;
		// Maps the array.
		graphics_buffer_span_map<Element> map();

		// Gets the debug label of the shader array.
		using basic_shader_buffer::label;
		// Sets the debug label of the shader array.
		using basic_shader_buffer::set_label;
	};
} // namespace tr

#include "impl/shader_buffer.hpp" // IWYU pragma: export