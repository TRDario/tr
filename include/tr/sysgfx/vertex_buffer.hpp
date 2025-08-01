#pragma once
#include "../utility/handle.hpp"
#include "../utility/ranges.hpp"

namespace tr::gfx {
	// Static vertex buffer class for holding immutable vertex data.
	class basic_static_vertex_buffer {
	  public:
		// Uploads vertex data into a static vertex buffer.
		basic_static_vertex_buffer(std::span<const std::byte> data);

		// Sets the debug label of the vertex buffer.
		void set_label(std::string_view label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_vbo;
		// The size of the vertex buffer.
		std::intptr_t m_size;

		friend void set_vertex_buffer(const basic_static_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride);
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
		std::size_t size() const;
		// Gets the capacity of the vertex buffer.
		std::size_t capacity() const;

		// Sets the size of the vertex buffer to 0.
		void clear();
		// Clears the buffer and resizes it, potentially resizing it.
		void resize(std::size_t size);
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(std::size_t capacity);
		// Sets the contents of the buffer, potentially reallocating it.
		void set(std::span<const std::byte> data);
		// Sets a region of the buffer.
		void set_region(std::size_t offset, std::span<const std::byte> data);

		// Sets the debug label of the vertex buffer.
		void set_label(const std::string& label);
		// Sets the debug label of the vertex buffer.
		void set_label(std::string&& label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_vbo;
		// The used size of the buffer.
		std::size_t m_size;
		// The capacity of the buffer.
		std::size_t m_capacity;
		// The label of the vertex buffer.
		std::string m_label;

		friend void set_vertex_buffer(const basic_dyn_vertex_buffer& buffer, int slot, std::intptr_t offset, std::size_t stride);
	};

	// Typed dynamic vertex buffer class.
	template <standard_layout T> class dyn_vertex_buffer : public basic_dyn_vertex_buffer {
	  public:
		using basic_dyn_vertex_buffer::basic_dyn_vertex_buffer;

		// Gets the size of the vertex buffer contents.
		std::size_t size() const;
		// Gets the capacity of the vertex buffer.
		std::size_t capacity() const;

		// Clears the buffer and resizes it, potentially reallocating it.
		void resize(std::size_t size);
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(std::size_t capacity);
		// Sets the contents of the buffer, potentially reallocating it.
		template <typed_contiguous_range<T> R> void set(R&& data);
		// Sets a region of the buffer.
		template <typed_contiguous_range<T> R> void set_region(std::size_t offset, R&& data);

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

template <tr::standard_layout T> std::size_t tr::gfx::dyn_vertex_buffer<T>::size() const
{
	return basic_dyn_vertex_buffer::size() / sizeof(T);
}

template <tr::standard_layout T> std::size_t tr::gfx::dyn_vertex_buffer<T>::capacity() const
{
	return basic_dyn_vertex_buffer::capacity() / sizeof(T);
}

template <tr::standard_layout T> void tr::gfx::dyn_vertex_buffer<T>::resize(std::size_t size)
{
	basic_dyn_vertex_buffer::resize(size * sizeof(T));
}

template <tr::standard_layout T> void tr::gfx::dyn_vertex_buffer<T>::reserve(std::size_t size)
{
	basic_dyn_vertex_buffer::reserve(size * sizeof(T));
}

template <tr::standard_layout T> template <tr::typed_contiguous_range<T> R> void tr::gfx::dyn_vertex_buffer<T>::set(R&& data)
{
	basic_dyn_vertex_buffer::set(range_bytes(data));
}

template <tr::standard_layout T>
template <tr::typed_contiguous_range<T> R>
void tr::gfx::dyn_vertex_buffer<T>::set_region(std::size_t offset, R&& data)
{
	basic_dyn_vertex_buffer::set_region(offset * sizeof(T), range_bytes(data));
}