#pragma once
#include "vertex_buffer.hpp"

namespace tr {
	// Concept defining a contiguous range that can be passed to index buffer functions.
	template <class Range>
	concept index_range = typed_contiguous_range<Range, std::uint16_t>;

	// Static index buffer class for holding immutable index data.
	class static_index_buffer {
	  public:
		// Uploads index data into a static index buffer.
		static_index_buffer(std::span<const std::uint16_t> data) noexcept;

		// Sets the debug label of the index buffer.
		void set_label(std::string_view label) noexcept;

	  private:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> _id;
		// The size of the buffer.
		std::intptr_t _size;

		friend struct gfx_context;
	};

	// Dynamic index buffer class.
	class dyn_index_buffer {
	  public:
		// Creates a dynamic index buffer.
		dyn_index_buffer() noexcept;

		// Gets whether the index buffer is empty.
		bool empty() const noexcept;
		// Gets the size of the index buffer contents.
		std::size_t size() const noexcept;
		// Gets the capacity of the index buffer.
		std::size_t capacity() const noexcept;

		// Sets the size of the index buffer to 0.
		void clear() noexcept;
		// Clears the buffer and resizes it, potentially reallocating in the process.
		void resize(std::size_t size) noexcept;
		// Clears the buffer and guarantees a certain capacity for it.
		void reserve(std::size_t capacity) noexcept;
		// Sets the contents of the buffer, potentially reallocating in the process.
		void set(std::span<const std::uint16_t> data) noexcept;
		// Sets a region of the buffer.
		void set_region(std::size_t offset, std::span<const std::uint16_t> data) noexcept;

		// Sets the debug label of the vertex buffer.
		void set_label(const std::string& label) noexcept;
		// Sets the debug label of the vertex buffer.
		void set_label(std::string&& label) noexcept;

	  private:
		struct deleter {
			void operator()(unsigned int id) const noexcept;
		};

		// Handle to the OpenGL buffer.
		handle<unsigned int, 0, deleter> _id;
		// The used size of the buffer.
		std::size_t _size;
		// The capacity of the buffer.
		std::size_t _capacity;
		// The label of the index buffer.
		std::string _label;

		friend struct gfx_context;
	};
} // namespace tr