#pragma once
#include "../utility/handle.hpp"

namespace tr {
	class static_index_buffer;
	class dyn_index_buffer;
	namespace gfx_context {
		void set_index_buffer(const static_index_buffer& buffer);
		void set_index_buffer(const dyn_index_buffer& buffer);
	} // namespace gfx_context

	// Concept defining a contiguous range that can be passed to index buffer functions.
	template <class Range>
	concept index_range = typed_contiguous_range<Range, std::uint16_t>;

	// Static index buffer class for holding immutable index data.
	class static_index_buffer {
	  public:
		// Uploads index data into a static index buffer.
		static_index_buffer(std::span<const std::uint16_t> data);

		// Sets the debug label of the index buffer.
		void set_label(std::string_view label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> ibo;
		// The size of the buffer.
		std::intptr_t size;

		friend void gfx_context::set_index_buffer(const static_index_buffer& buffer);
	};

	// Dynamic index buffer class.
	class dyn_index_buffer {
	  public:
		// Creates a dynamic index buffer.
		dyn_index_buffer();

		// Gets whether the index buffer is empty.
		bool empty() const;
		// Gets the size of the index buffer contents.
		std::size_t size() const;
		// Gets the capacity of the index buffer.
		std::size_t capacity() const;

		// Sets the size of the index buffer to 0.
		void clear();
		// Clears the buffer and resizes it, potentially reallocating in the process.
		void resize(std::size_t size);
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(std::size_t capacity);
		// Sets the contents of the buffer, potentially reallocating in the process.
		void set(std::span<const std::uint16_t> data);
		// Sets a region of the buffer.
		void set_region(std::size_t offset, std::span<const std::uint16_t> data);

		// Sets the debug label of the vertex buffer.
		void set_label(const std::string& label);
		// Sets the debug label of the vertex buffer.
		void set_label(std::string&& label);

	  private:
		struct deleter {
			void operator()(unsigned int id) const;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> ibo;
		// The used size of the buffer.
		std::size_t size_;
		// The capacity of the buffer.
		std::size_t capacity_;
		// The label of the index buffer.
		std::string label;

		friend void gfx_context::set_index_buffer(const dyn_index_buffer& buffer);
	};
} // namespace tr