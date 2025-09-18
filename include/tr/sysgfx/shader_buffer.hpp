#pragma once
#include "buffer_map.hpp"

namespace tr::gfx {
	// GPU buffer accessable to a shader.
	class basic_shader_buffer {
	  public:
		// Shader buffer map access type.
		enum class access : u32 {
			READ_ONLY = 1,  // Buffer maps are read-only.
			WRITE_ONLY = 2, // Buffer maps are write-only.
			READ_WRITE = 3  // Buffer maps are readable and writable.
		};

		// Allocates an uninitialized shader buffer.
		basic_shader_buffer(usize header_size, usize capacity, access access = access::WRITE_ONLY);

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
		// The access type of the buffer.
		access m_access;
		// The size of the header.
		usize m_header_size;
		// The current size of the array.
		usize m_array_size;
		// The capacity of the array.
		usize m_array_capacity;

		friend class shader_base;
	};

	// Shader buffer with a typed header and array.
	template <class Header, class ArrayElement> class shader_buffer : public basic_shader_buffer {
	  public:
		// Allocates an uninitialized shader buffer.
		shader_buffer(usize capacity, access access = access::WRITE_ONLY);

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
		shader_array(usize capacity, access access = access::WRITE_ONLY);

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
tr::gfx::shader_buffer<Header, ArrayElement>::shader_buffer(usize capacity, access access)
	: basic_shader_buffer{sizeof(Header), sizeof(ArrayElement) * capacity, access}
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
tr::gfx::shader_array<T>::shader_array(usize capacity, access access)
	: basic_shader_buffer{0, sizeof(T) * capacity, access}
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