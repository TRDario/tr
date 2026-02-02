///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides RAII wrappers over GPU buffer maps.                                                                                          //
//                                                                                                                                       //
// Some GPU buffers can be mapped; mapping a buffer returns one of the buffer map types:                                                 //
//     - shader_buffer.map_header() -> tr::gfx::buffer_object_map<Header>                                                                //
//     - shader_buffer.map_array() -> tr::gfx::buffer_span_map<ArrayElement>                                                             //
//     - uniform_buffer.map() -> tr::gfx::buffer_object_map<T>                                                                           //
//                                                                                                                                       //
// The type of access a map allows can be specified for certain buffers, and may be read/write only or read+write.                       //
//                                                                                                                                       //
// tr::gfx::basic_buffer_map is primarily an implementation detail, wrapping a map and being able to return it as a span of bytes, but   //
// otherwise not further abstracting it. The buffer is unmapped once the map goes out of scope.                                          //
//                                                                                                                                       //
// tr::gfx::buffer_object_map represents the map as a reference to an object in the buffer (the header of a shader buffer, for example). //
// A reference to the object can be taken, its members can be accessed, or it can be assigned:                                           //
//     - tr::gfx::buffer_object_map<tr::rgba8> map{uniform_buffer.map()}                                                                 //
//     - tr::rgba8 copy{map} -> using tr::rgba8&                                                                                         //
//     - tr::u8 red{map->r} -> gets the red component of the mapped color                                                                //
//     - map = "FFFFFF"_rgba8 -> sets the mapped color to white                                                                          //
//                                                                                                                                       //
// tr::gfx::buffer_span_map represents the map as a span of objects in the buffer (the array of a shader buffer, for example).           //
// The map can be converted to a regular span, indexed into, or iterated through:                                                        //
//     - tr::gfx::buffer_span_map<int> map{shader_buffer.map_array()}                                                                    //
//     - std::span<int> span{map} -> conversion to span                                                                                  //
//     - map[3] -> gets the int at index 3 of the map                                                                                    //
//     - map.data() -> gets a pointer to the map data                                                                                    //
//     - for (int& v : map) { ++v } -> iterating through the map                                                                         //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Buffer map access type.
	enum class map_type : u32 {
		read_only = 1,  // Buffer maps are read-only.
		write_only = 2, // Buffer maps are write-only.
		read_write = 3  // Buffer maps are readable and writable.
	};

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

		// Wraps a raw buffer map.
		basic_buffer_map(unsigned int buffer, std::span<std::byte> span);

		friend class basic_shader_buffer;
		friend class basic_uniform_buffer;
	};

	// Mapped buffer object.
	template <class T> class buffer_object_map : basic_buffer_map {
	  public:
		// Gets a reference to the object.
		operator T&() const;
		//  Gets a reference to the object.
		T& operator*() const;
		// Pointer access to the mapped object.
		T* operator->() const;
		// Assigns the object.
		template <class T1> T& operator=(T1&& r) const;

	  private:
		// Wraps a basic buffer map.
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

		// Casts the map into a regular span.
		operator std::span<T>() const;
		// Indexes into the map.
		reference operator[](usize index) const;
		// Gets a pointer to the data of the map.
		pointer data() const;
		// Gets the size of the map in elements.
		size_type size() const;
		// Gets an iterator to the beginning of the map.
		iterator begin() const;
		// Gets an iterator to the end of the map.
		iterator end() const;

	  private:
		// Wraps a basic buffer map.
		buffer_span_map(basic_buffer_map&& map);

		template <class Header, class ArrayElement> friend class shader_buffer;
		template <class T1> class uniform_buffer;
	};
}; // namespace tr::gfx

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "buffer_map_impl.hpp" // IWYU pragma: export