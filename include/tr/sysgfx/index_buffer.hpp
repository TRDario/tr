#pragma once
#include "../utility/handle.hpp"

namespace tr::gfx {
	// Concept defining a contiguous range that can be passed to index buffer functions.
	template <class Range>
	concept index_range = typed_contiguous_range<Range, u16>;

	// Static index buffer class for holding immutable index data.
	class static_index_buffer {
	  public:
		// Uploads index data into a static index buffer.
		static_index_buffer(std::span<const u16> data);

		// Sets the debug label of the index buffer.
		void set_label(std::string_view label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> m_ibo;
		// The size of the buffer.
		std::intptr_t m_size;

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

		// Sets the debug label of the vertex buffer.
		void set_label(std::string_view label);

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