/// @file
/// @brief Provides RAII wrappers over graphics buffer maps.

#pragma once
#include "../utility/handle.hpp"
#include "../utility/integer.hpp"

namespace tr
{
	class graphics_context;
}

//

namespace tr
{
	/// Buffer map access type.
	enum class map_type : u32
	{
		/// Buffer maps are read-only.
		read_only = 1,
		/// Buffer maps are write-only.
		write_only = 2,
		/// Buffer maps are readable and writable.
		read_write = 3
	};

	/// RAII wrapper over a buffer map.
	class basic_graphics_buffer_map
	{
	  public:
		/// @name Conversion operators
		/// @{

		/// Casts the map into a regular span.
		/// @return Span of bytes covering the buffer map.
		operator std::span<std::byte>() const;

		/// @}

	  private:
		/// Buffer unmapper.
		struct unmapper
		{
			/// Reference to the context the buffer object is on.
			graphics_context& context;

			//

			/// Unmaps a buffer.
			/// @param id OpenGL buffer ID.
			void operator()(unsigned int id) const;
		};

		//

		/// Reference to the mapped buffer.
		handle<unsigned int, 0, unmapper> m_handle;

		/// Span of the buffer map.
		std::span<std::byte> m_span;

		//

		/// Wraps a raw buffer map.
		/// @param context Reference to the graphics context the buffer is on.
		/// @param buffer ID of the mapped buffer.
		/// @param span Span of the buffer map.
		basic_graphics_buffer_map(graphics_context& context, unsigned int buffer, std::span<std::byte> span);

		//

		// Uses the private constructor.
		friend class basic_shader_buffer;

		// Uses the private constructor.
		friend class basic_uniform_buffer;
	};

	/// Mapped buffer object.
	/// @tparam Object Object type contained in the map.
	template <typename Object>
	class graphics_buffer_object_map : basic_graphics_buffer_map
	{
	  public:
		/// @name Access
		/// @{

		/// Gets a reference to the object.
		/// @return Reference to the contained object.
		operator Object&() const;

		/// Gets a reference to the object.
		/// @return Reference to the contained object.
		Object& operator*() const;

		/// Pointer access to the mapped object.
		/// @return Pointer to the contained object.
		Object* operator->() const;

		/// Assigns the object.
		/// @tparam T Type assignable to `Object`.
		/// @param rhs Value to assign to the contained object.
		/// @return Reference to the contained object.
		template <std::assignable_from<Object> T>
		Object& operator=(T&& rhs) const;

		/// @}

	  private:
		/// Wraps over a basic buffer map.
		/// @param map Base buffer map.
		graphics_buffer_object_map(basic_graphics_buffer_map&& map);

		//

		// Uses the private constructor.
		template <typename Header, typename ArrayElement>
		friend class shader_buffer;
	};

	/// Mapped buffer span.
	/// @tparam Element Type of the elements of the span.
	template <typename Element>
	class graphics_buffer_span_map : basic_graphics_buffer_map
	{
	  public:
		/// Element type used by the span.
		using element_type = Element;

		/// Value type used by the span.
		using value_type = Element;

		/// Pointer type used by the span.
		using pointer = Element*;

		/// Constant pointer type used by the span.
		using const_pointer = const Element*;

		/// Reference type used by the span.
		using reference = Element&;

		/// Constant reference type used by the span.
		using const_reference = const Element&;

		/// Size type used by the span.
		using size_type = std::span<Element>::size_type;

		/// Difference type used by the span.
		using difference_type = std::span<Element>::difference_type;

		/// Iterator type used by the span.
		using iterator = std::span<Element>::iterator;

		/// @name Access
		/// @{

		/// Casts the map into a regular span.
		/// @return Base span of the map.
		operator std::span<Element>() const;

		/// Indexes into the map.
		/// @param index Index of the element within the map to access.
		/// @return Reference to an element of the map.
		reference operator[](usize index) const;

		/// Gets a pointer to the data of the map.
		/// @return Pointer to the data of the map.
		pointer data() const;

		/// Gets the size of the map.
		/// @return Number of elements in the map.
		size_type size() const;

		/// Gets an iterator to the beginning of the map.
		/// @return Iterator to the beginning of the map.
		iterator begin() const;

		/// Gets an iterator to the end of the map.
		/// @return Iterator to the end of the map.
		iterator end() const;

		/// @}

	  private:
		/// Wraps over a basic buffer map.
		/// @param map Base buffer map.
		graphics_buffer_span_map(basic_graphics_buffer_map&& map);

		//

		// Uses the private constructor.
		template <typename Header, typename ArrayElement>
		friend class shader_buffer;

		// Uses the private constructor.
		template <typename Object>
		class uniform_buffer;
	};
}; // namespace tr

#include "impl/graphics_buffer_map.hpp" // IWYU pragma: export