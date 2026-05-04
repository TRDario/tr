///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides uniform buffer classes.                                                                                                      //
//                                                                                                                                       //
// Uniform buffers are an abstraction over OpenGL UBOs.                                                                                  //
//                                                                                                                                       //
// The uniform buffer comes in two variants: the untyped tr::gfx::basic_uniform_buffer, which works with bytes and byte buffers, and the //
// typed tr::gfx::uniform_buffer<T>, which acts as a typed container.                                                                    //
//                                                                                                                                       //
// Uniform buffers are allocated once at construction and cannot be resized. The size of the data may be queried with the .size() method.//
// To update the data in the buffer, either the .set() method to directly set data, or .map() to map the buffer (write-only!) to memory  //
// and writing to said map are possible. Note that when the buffer is mapped, no other action may be performed on it; the map status of  //
// a buffer can be checked with the .mapped() method:                                                                                    //
//     - tr::gfx::uniform_buffer<std::array<float, 32>> unibuf{} -> allocates a buffer for a 32-float array                              //
//     - unibuf.size() -> 32                                                                                                             //
//     - unibuf.set(data) -> directly sets the data of the buffer                                                                        //
//     - tr::gfx::buffer_object_map map{unibuf.map()}; (*map)[10] = 5.0f -> maps the buffer and sets a portion of it                     //
//     - unibuf.mapped() -> true                                                                                                         //
//                                                                                                                                       //
// The label of a uniform buffer can be set with TR_SET_LABEL(unibuf, label):                                                            //
//     - TR_SET_LABEL(unibuf, "Example buffer") -> 'unibuf' is now labelled "Example buffer"                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "buffer.hpp"
#include "buffer_map.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Shader uniform buffer.
	class basic_uniform_buffer : private buffer {
	  public:
		// Allocates an uninitialized uniform buffer.
		basic_uniform_buffer(usize size);

		// Gets the size of the buffer.
		usize size() const;

		// Sets the data of the buffer.
		void set(std::span<const std::byte> data);

		// Gets whether the buffer is mapped.
		bool mapped() const;
		// Maps the buffer.
		// This map is write-only!
		basic_buffer_map map();

#ifdef TR_ENABLE_ASSERTS
		// Gets the debug label of the uniform buffer.
		using buffer::label;
		// Sets the debug label of the uniform buffer.
		using buffer::set_label;
#endif

	  private:
		// The size of the buffer.
		usize m_size;

		friend class shader_base;
	};

	// Typed shader uniform buffer.
	template <typename Object> class uniform_buffer : public basic_uniform_buffer {
	  public:
		// Allocates an uninitialized uniform buffer.
		uniform_buffer();

		// Sets the contents of the buffer.
		void set(const Object& data);

		// Maps the buffer.
		// This map is write-only!
		buffer_object_map<Object> map();

	  private:
		using basic_uniform_buffer::map;
		using basic_uniform_buffer::set;
		using basic_uniform_buffer::size;
	};
} // namespace tr::gfx

#include "impl/uniform_buffer.hpp" // IWYU pragma: export