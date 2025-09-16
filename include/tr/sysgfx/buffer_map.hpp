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

		template <class Header, class ArrayElement> friend class shader_buffer;
	};

	// Mapped buffer span.
	template <class T> class buffer_span_map : basic_buffer_map {
	  public:
		using element_type = T;
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = std::span<T>::size_type;
		using difference_type = std::span<T>::difference_type;
		using iterator = std::span<T>::iterator;

		operator std::span<T>() const;
		reference operator[](usize index) const;
		pointer data() const;
		size_type size() const;
		iterator begin() const;
		iterator end() const;

	  private:
		buffer_span_map(basic_buffer_map&& map);

		template <class Header, class ArrayElement> friend class shader_buffer;
		template <class T1> class uniform_buffer;
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
	return as_mut_objects<T>(std::span<std::byte>{*this});
}

template <class T> tr::gfx::buffer_span_map<T>::reference tr::gfx::buffer_span_map<T>::operator[](usize index) const
{
	return std::span<T>{*this}[index];
}

template <class T> tr::gfx::buffer_span_map<T>::pointer tr::gfx::buffer_span_map<T>::data() const
{
	return std::span<T>{*this}.data();
}

template <class T> tr::gfx::buffer_span_map<T>::size_type tr::gfx::buffer_span_map<T>::size() const
{
	return std::span<T>{*this}.size();
}

template <class T> tr::gfx::buffer_span_map<T>::iterator tr::gfx::buffer_span_map<T>::begin() const
{
	return std::span<T>{*this}.begin();
}

template <class T> tr::gfx::buffer_span_map<T>::iterator tr::gfx::buffer_span_map<T>::end() const
{
	return std::span<T>{*this}.end();
}

template <class T>
tr::gfx::buffer_span_map<T>::buffer_span_map(basic_buffer_map&& map)
	: basic_buffer_map{std::move(map)}
{
}