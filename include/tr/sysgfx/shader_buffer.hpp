#pragma once
#include <tr/utility.hpp>

namespace tr {
	// RAII wrapper over a shader buffer map.
	class shader_buffer_map {
	  public:
		// Casts the map into a regular span.
		template <standard_layout T> operator std::span<T>() const noexcept;

	  private:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Reference to the buffer.
		handle<unsigned int, 0, deleter> _buffer;
		// The actual span.
		std::span<std::byte> _span;

		shader_buffer_map(unsigned int buffer, std::span<std::byte> span) noexcept;

		friend class shader_buffer;
	};

	// GPU buffer accessable to a shader.
	class shader_buffer {
	  public:
		// Shader buffer map access type.
		enum class access : std::uint32_t {
			READ_ONLY = 1,  // Buffer maps are read-only.
			WRITE_ONLY = 2, // Buffer maps are write-only.
			READ_WRITE = 3  // Buffer maps are readable and writable.
		};

		// Allocates an uninitialized shader buffer.
		shader_buffer(std::intptr_t header_size, std::intptr_t capacity, access access) noexcept;

		// Gets the size of the fixed header block.
		std::intptr_t header_size() const noexcept;
		// Gets the size of the dynamic array.
		std::intptr_t array_size() const noexcept;
		// Gets the maximum capacity of the dynamic array.
		std::intptr_t array_capacity() const noexcept;

		// Sets the data of the header.
		void set_header(std::span<const std::byte> data) noexcept;
		// Sets the data of the header.
		template <std::ranges::contiguous_range R> void set_header(R&& range) noexcept;
		// Sets the data of the header.
		template <class T> void set_header(const T& value) noexcept;
		// Sets the data of the dynamic array.
		void set_array(std::span<const std::byte> data) noexcept;
		// Sets the data of the dynamic array.
		template <std::ranges::contiguous_range R> void set_array(R&& range) noexcept;
		// Resizes the dynamic array.
		void resize_array(std::intptr_t size) noexcept;

		// Gets whether the buffer is mapped.
		bool mapped() const noexcept;
		// Maps the fixed header.
		shader_buffer_map map_header() noexcept;
		// Maps the dynamic array.
		shader_buffer_map map_array() noexcept;
		// Maps the entire buffer.
		shader_buffer_map map() noexcept;

		// Sets the debug label of the shader buffer.
		void set_label(std::string_view label) noexcept;

	  private:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> _id;
		// The access type of the buffer.
		access _access;
		// The size of the header.
		std::intptr_t _header_size;
		// The current size of the array.
		std::intptr_t _array_size;
		// The capacity of the array.
		std::intptr_t _array_capacity;

		friend class shader_base;
	};
} // namespace tr

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <tr::standard_layout T> tr::shader_buffer_map::operator std::span<T>() const noexcept
{
	return as_mut_objects<T>(_span);
}

template <std::ranges::contiguous_range R> void tr::shader_buffer::set_header(R&& range) noexcept
{
	set_header(std::span<const std::byte>{range_bytes(range)});
}

template <class T> void tr::shader_buffer::set_header(const T& value) noexcept
{
	set_header(std::span<const std::byte>{as_bytes(value)});
}

template <std::ranges::contiguous_range R> void tr::shader_buffer::set_array(R&& range) noexcept
{
	set_array(std::span<const std::byte>{range_bytes(range)});
}