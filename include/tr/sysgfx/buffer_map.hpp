#pragma once
#include "../utility/handle.hpp"

namespace tr::gfx {
	// RAII wrapper over a buffer map.
	class basic_buffer_map {
	  public:
		// Casts the map into a regular span.
		operator std::span<std::byte>() const;

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Reference to the buffer.
		handle<unsigned int, 0, deleter> m_bo;
		// The actual span.
		std::span<std::byte> m_span;

		basic_buffer_map(unsigned int buffer, std::span<std::byte> span);

		friend class basic_shader_buffer;
		friend class basic_uniform_buffer;
	};

	// Mapped buffer object.
	template <class T> class buffer_object_map : basic_buffer_map {
	  public:
		// Casts the map into a reference to the object.
		operator T&() const;
		// Pointer access to the mapped object.
		T* operator->() const;
		// Assigns the object.
		template <class T1> T& operator=(T1&& r) const;

	  private:
		buffer_object_map(basic_buffer_map&& map);

		friend class basic_shader_buffer;
	};

	// Mapped buffer span.
	template <class T> class buffer_span_map : basic_buffer_map {
	  public:
		// Casts the map into a regular span.
		operator std::span<T>() const;

	  private:
		buffer_span_map(basic_buffer_map&& map);

		friend class basic_shader_buffer;
		friend class basic_uniform_buffer;
	};
}; // namespace tr::gfx

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

template <class T> tr::gfx::buffer_object_map<T>::operator T&() const
{
	return *(T*)std::span<std::byte>{*this}.data();
}

template <class T> T* tr::gfx::buffer_object_map<T>::operator->() const
{
	return (T*)std::span<std::byte>{*this}.data();
}

template <class T> template <class T1> T& tr::gfx::buffer_object_map<T>::operator=(T1&& r) const
{
	return ((T&)*this) = std::forward<T1>(r);
}

template <class T>
tr::gfx::buffer_object_map<T>::buffer_object_map(basic_buffer_map&& map)
	: basic_buffer_map{std::move(map)}
{
}

template <class T> tr::gfx::buffer_span_map<T>::operator std::span<T>() const
{
	return as_mut_objects<T>(*this);
}

template <class T>
tr::gfx::buffer_span_map<T>::buffer_span_map(basic_buffer_map&& map)
	: basic_buffer_map{std::move(map)}
{
}