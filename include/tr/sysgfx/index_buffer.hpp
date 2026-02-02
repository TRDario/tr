///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides index buffer classes.                                                                                                        //
//                                                                                                                                       //
// Index buffers are an abstraction over OpenGL EBOs. tr does not currently support 32-bit indices.                                      //
//                                                                                                                                       //
// The index buffer comes in two variants: tr::gfx::static_index_buffer is initialized once and is immutable, while                      //
// tr::gfx::dyn_index_buffer can be resized and modified at will.                                                                        //
//                                                                                                                                       //
// Static index buffers are constructed with a span of data that will be copied into the buffer:                                         //
//     - tr::gfx::static_index_buffer buffer{data} -> copies 'data' into the buffer                                                      //
//                                                                                                                                       //
// Dynamic index buffers are constructed empty. Whether a buffer is empty can be checked with the .empty() method. Much like std::vector,//
// dynamic index buffers distinguish between buffer size and capacity, both of which can be queried with the respective method.          //
// The buffer automatically reallocates itself if its current capacity is insufficient, but the user can reserve a capacity in           //
// advance with the .advance() method. Note that unlike std::vector, this clears any previous buffer data. The buffer can be resized     //
// with .resize() or set to a copy of a span with .set(), which automatically sets its size to match that of the copied buffer.          //
// The .set_region() method can be used to set a region of the buffer; this function never affects the size or capacity of the buffer.   //
// The buffer can be cleared with the .clear() method, but like with std::vector, the allocated capacity is retained:                    //
//     - tr::gfx::dyn_index_buffer buffer; buffer.empty() -> true                                                                        //
//     - buffer.reserve(100); buffer.resize(50) -> buffer now has size 50, capacity 128                                                  //
//     - buffer.size(); buffer.capacity() -> 50, 128                                                                                     //
//     - std::array<u16, 500> data; buffer.set(data) -> buffer now stores a copy of 'data', has size 500, capacity 512                   //
//     - std::array<u16, 100> data2; buffer.set_region(400, data2) -> a copy of data2 is now in buffer[400-499]                          //
//     - buffer.clear() -> buffer now has size 0, capacity 512                                                                           //
//                                                                                                                                       //
// The label of a index buffer can be set with TR_SET_LABEL(ibuf, label):                                                                //
//     - TR_SET_LABEL(ibuf, "Example buffer") -> 'ibuf' is now labelled "Example buffer"                                                 //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utility/handle.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr::gfx {
	// Concept defining a contiguous range that can be passed to index buffer functions.
	template <class Range>
	concept index_range = typed_contiguous_range<Range, u16>;

	// Static index buffer class for holding immutable index data.
	class static_index_buffer {
	  public:
		// Uploads index data into a static index buffer.
		static_index_buffer(std::span<const u16> data);

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the index buffer.
		void set_label(std::string_view label);
		// Gets the debug label of the index buffer.
		std::string label() const;
#endif

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_ibo;
		// The size of the buffer.
		ssize m_size;

		friend void set_index_buffer(const static_index_buffer& buffer);
	};

	// Dynamic index buffer class.
	class dyn_index_buffer {
	  public:
		// Creates a dynamic index buffer.
		dyn_index_buffer();

		// Gets whether the index buffer is empty.
		bool empty() const;
		// Gets the size of the index buffer contents.
		usize size() const;
		// Gets the capacity of the index buffer.
		usize capacity() const;

		// Sets the size of the index buffer to 0.
		void clear();
		// Clears the buffer and resizes it, potentially reallocating in the process.
		void resize(usize size);
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(usize capacity);
		// Sets the contents of the buffer, potentially reallocating in the process.
		void set(std::span<const u16> data);
		// Sets a region of the buffer.
		void set_region(usize offset, std::span<const u16> data);

#ifdef TR_ENABLE_ASSERTS
		// Sets the debug label of the index buffer.
		void set_label(std::string_view label);
		// Gets the debug label of the index buffer.
		std::string label() const;
#endif

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_ibo;
		// The used size of the buffer.
		usize m_size;
		// The capacity of the buffer.
		usize m_capacity;

		friend void set_index_buffer(const dyn_index_buffer& buffer);
	};
} // namespace tr::gfx