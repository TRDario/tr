#pragma once
#include "buffer_map.hpp"

namespace tr::gfx {
	// Shader uniform buffer.
	class basic_uniform_buffer {
	  public:
		// Allocates an uninitialized uniform buffer.
		basic_uniform_buffer(ssize size);

		// Gets the size of the buffer.
		ssize size() const;

		// Sets the data of the buffer.
		void set(std::span<const std::byte> data);

		// Gets whether the buffer is mapped.
		bool mapped() const;
		// Maps the buffer.
		// This map is write-only!
		basic_buffer_map map();

		// Sets the debug label of the uniform buffer.
		void set_label(std::string_view label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_ubo;
		// The size of the buffer.
		ssize m_size;

		friend class shader_base;
	};

	// Typed shader uniform buffer.
	template <class T> class uniform_buffer : public basic_uniform_buffer {
	  public:
		// Allocates an uninitialized uniform buffer.
		uniform_buffer();

		// Sets the contents of the buffer.
		void set(const T& data);

		// Maps the buffer.
		// This map is write-only!
		buffer_object_map<T> map();

	  private:
		using basic_uniform_buffer::map;
		using basic_uniform_buffer::set;
		using basic_uniform_buffer::size;
	};
} // namespace tr::gfx

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class T>
tr::gfx::uniform_buffer<T>::uniform_buffer()
	: basic_uniform_buffer{sizeof(T)}
{
}

template <class T> void tr::gfx::uniform_buffer<T>::set(const T& data)
{
	basic_uniform_buffer::set(as_bytes(data));
}

template <class T> tr::gfx::buffer_object_map<T> tr::gfx::uniform_buffer<T>::map()
{
	return basic_uniform_buffer::map();
}