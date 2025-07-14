#pragma once
#include <tr/utility.hpp>

namespace tr {
	class vertex_format;
}

namespace tr {
	// Concept defining a contiguous range holding a specific type.
	template <class R, class T>
	concept typed_contiguous_range = std::ranges::contiguous_range<R> && std::same_as<T, std::ranges::range_value_t<R>>;

	// Static vertex buffer class for holding immutable vertex data.
	class basic_static_vertex_buffer {
	  public:
		// Uploads vertex data into a static vertex buffer.
		basic_static_vertex_buffer(std::span<const std::byte> data) noexcept;

		// Sets the debug label of the vertex buffer.
		void set_label(std::string_view label) noexcept;

	  private:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> _id;
		// The size of the vertex buffer.
		std::intptr_t _size;

		friend struct gfx_context;
	};

	// Typed static vertex buffer class for holding immutable vertex data of a single type.
	template <standard_layout T> class static_vertex_buffer : public basic_static_vertex_buffer {
	  public:
		// Creates a static vertex buffer.
		template <typed_contiguous_range<T> R> static_vertex_buffer(R&& range) noexcept;
	};

	// Dynamic vertex buffer class.
	class basic_dyn_vertex_buffer {
	  public:
		// Creates a dynamic vertex buffer.
		basic_dyn_vertex_buffer() noexcept;

		// Gets whether the vertex buffer is empty.
		bool empty() const noexcept;
		// Gets the size of the vertex buffer contents.
		std::size_t size() const noexcept;
		// Gets the capacity of the vertex buffer.
		std::size_t capacity() const noexcept;

		// Sets the size of the vertex buffer to 0.
		void clear() noexcept;
		// Clears the buffer and resizes it, potentially resizing it.
		void resize(std::size_t size) noexcept;
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(std::size_t capacity) noexcept;
		// Sets the contents of the buffer, potentially reallocating it.
		void set(std::span<const std::byte> data) noexcept;
		// Sets a region of the buffer.
		void set_region(std::size_t offset, std::span<const std::byte> data) noexcept;

		// Sets the debug label of the vertex buffer.
		void set_label(const std::string& label) noexcept;
		// Sets the debug label of the vertex buffer.
		void set_label(std::string&& label) noexcept;

	  private:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> _id;
		// The used size of the buffer.
		std::size_t _size;
		// The capacity of the buffer.
		std::size_t _capacity;
		// The label of the vertex buffer.
		std::string _label;

		friend struct gfx_context;
	};

	// Typed dynamic vertex buffer class.
	template <standard_layout T> class dyn_vertex_buffer : public basic_dyn_vertex_buffer {
	  public:
		using basic_dyn_vertex_buffer::basic_dyn_vertex_buffer;

		// Gets the size of the vertex buffer contents.
		std::size_t size() const noexcept;
		// Gets the capacity of the vertex buffer.
		std::size_t capacity() const noexcept;

		// Clears the buffer and resizes it, potentially reallocating it.
		void resize(std::size_t size) noexcept;
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(std::size_t capacity) noexcept;
		// Sets the contents of the buffer, potentially reallocating it.
		template <typed_contiguous_range<T> R> void set(R&& data) noexcept;
		// Sets a region of the buffer.
		template <typed_contiguous_range<T> R> void set_region(std::size_t offset, R&& data) noexcept;

	  private:
		using basic_dyn_vertex_buffer::capacity;
		using basic_dyn_vertex_buffer::reserve;
		using basic_dyn_vertex_buffer::resize;
		using basic_dyn_vertex_buffer::set_region;
		using basic_dyn_vertex_buffer::size;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::standard_layout T>
template <tr::typed_contiguous_range<T> R>
tr::static_vertex_buffer<T>::static_vertex_buffer(R&& range) noexcept
	: basic_static_vertex_buffer{range_bytes(range)}
{
}

template <tr::standard_layout T> std::size_t tr::dyn_vertex_buffer<T>::size() const noexcept
{
	return basic_dyn_vertex_buffer::size() / sizeof(T);
}

template <tr::standard_layout T> std::size_t tr::dyn_vertex_buffer<T>::capacity() const noexcept
{
	return basic_dyn_vertex_buffer::capacity() / sizeof(T);
}

template <tr::standard_layout T> void tr::dyn_vertex_buffer<T>::resize(std::size_t size) noexcept
{
	basic_dyn_vertex_buffer::resize(size * sizeof(T));
}

template <tr::standard_layout T> void tr::dyn_vertex_buffer<T>::reserve(std::size_t size) noexcept
{
	basic_dyn_vertex_buffer::reserve(size * sizeof(T));
}

template <tr::standard_layout T> template <tr::typed_contiguous_range<T> R> void tr::dyn_vertex_buffer<T>::set(R&& data) noexcept
{
	basic_dyn_vertex_buffer::set(range_bytes(data));
}

template <tr::standard_layout T>
template <tr::typed_contiguous_range<T> R>
void tr::dyn_vertex_buffer<T>::set_region(std::size_t offset, R&& data) noexcept
{
	basic_dyn_vertex_buffer::set_region(offset * sizeof(T), range_bytes(data));
}