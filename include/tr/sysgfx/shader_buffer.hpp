#pragma once
#include "../utility/handle.hpp"

namespace tr::gfx {
	// RAII wrapper over a shader buffer map.
	class basic_shader_buffer_map {
	  public:
		// Casts the map into a regular span.
		operator std::span<std::byte>() const;

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Reference to the buffer.
		handle<unsigned int, 0, deleter> m_sbo;
		// The actual span.
		std::span<std::byte> m_span;

		basic_shader_buffer_map(unsigned int buffer, std::span<std::byte> span);

		friend class basic_shader_buffer;
	};

	// Mapped shader buffer object.
	template <class T> class shader_buffer_object_map : basic_shader_buffer_map {
	  public:
		// Casts the map into a reference to the object.
		operator T&() const;
		// Pointer access to the mapped object.
		T* operator->() const;
		// Assigns the object.
		template <class T1> T& operator=(T1&& r) const;

	  private:
		shader_buffer_object_map(basic_shader_buffer_map&& map);

		friend class basic_shader_buffer;
	};

	// Mapped shader buffer span.
	template <class T> class shader_buffer_span_map : basic_shader_buffer_map {
	  public:
		// Casts the map into a regular span.
		operator std::span<T>() const;

	  private:
		shader_buffer_span_map(basic_shader_buffer_map&& map);

		friend class basic_shader_buffer;
	};

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
		basic_shader_buffer(ssize header_size, ssize capacity, access access);

		// Gets the size of the fixed header block.
		ssize header_size() const;
		// Gets the size of the dynamic array.
		ssize array_size() const;
		// Gets the maximum capacity of the dynamic array.
		ssize array_capacity() const;

		// Sets the data of the header.
		void set_header(std::span<const std::byte> data);
		// Sets the data of the dynamic array.
		void set_array(std::span<const std::byte> data);
		// Resizes the dynamic array.
		void resize_array(ssize size);

		// Gets whether the buffer is mapped.
		bool mapped() const;
		// Maps the fixed header.
		basic_shader_buffer_map map_header();
		// Maps the dynamic array.
		basic_shader_buffer_map map_array();
		// Maps the entire buffer.
		basic_shader_buffer_map map();

		// Sets the debug label of the shader buffer.
		void set_label(std::string_view label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_sbo;
		// The access type of the buffer.
		access m_access;
		// The size of the header.
		ssize m_header_size;
		// The current size of the array.
		ssize m_array_size;
		// The capacity of the array.
		ssize m_array_capacity;

		friend class shader_base;
	};
} // namespace tr::gfx

///////////////////////////////////////////////////////////// IMPLEMENTATION //////////////////////////////////////////////////////////////

template <class T> tr::gfx::shader_buffer_object_map<T>::operator T&() const
{
	return *(T*)std::span<std::byte>{*this}.data();
}

template <class T> T* tr::gfx::shader_buffer_object_map<T>::operator->() const
{
	return (T*)std::span<std::byte>{*this}.data();
}

template <class T> template <class T1> T& tr::gfx::shader_buffer_object_map<T>::operator=(T1&& r) const
{
	return ((T&)*this) = std::forward<T1>(r);
}

template <class T>
tr::gfx::shader_buffer_object_map<T>::shader_buffer_object_map(basic_shader_buffer_map&& map)
	: basic_shader_buffer_map{std::move(map)}
{
}

template <class T> tr::gfx::shader_buffer_span_map<T>::operator std::span<T>() const
{
	return as_mut_objects<T>(*this);
}

template <class T>
tr::gfx::shader_buffer_span_map<T>::shader_buffer_span_map(basic_shader_buffer_map&& map)
	: basic_shader_buffer_map{std::move(map)}
{
}