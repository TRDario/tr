/// @file
/// @brief Provides GPU buffers accessible to shaders.

#pragma once
#include "../utility/concepts.hpp"
#include "graphics_buffer.hpp"
#include "graphics_buffer_map.hpp"

//

namespace tr
{
	/// GPU buffer accessable to a shader.
	/// @details Shader buffers are divided into a fixed header block segment and a resizable array segment.
	class basic_shader_buffer : private graphics_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Allocates an uninitialized shader buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param header_size Size of the fixed header block in bytes.
		/// @param capacity Maximum capacity of the dyuamic array in bytes.
		/// @param map_type Type of map to create when mapping the buffer.
		basic_shader_buffer(graphics_context& context, usize header_size, usize capacity, map_type map_type = map_type::write_only);

		/// @}
		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the fixed header block.
		/// @return Size of the fixed header block in bytes.
		usize header_size() const;

		/// Gets the size of the dynamic array.
		/// @return Size of the dynamic array in bytes.
		usize array_size() const;

		/// Gets the maximum capacity of the dynamic array.
		/// @return Maximum capacity of the dynamic array in bytes.
		usize array_capacity() const;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the header.
		/// @param data Data to set the header with.
		/// @pre `data` must be of the same size as the buffer's header.
		void set_header(std::span<const std::byte> data);

		/// Sets the data of the dynamic array.
		/// @param data Data to set the dynamic array with.
		/// @pre `data` must be smaller than or equal to the capacity of the buffer's dynamic array.
		void set_array(std::span<const std::byte> data);

		/// Resizes the dynamic array.
		/// @param size Size of the dynamic array in bytes.
		/// @pre `size` must be less than or equal to the capacity of the buffer's dynamic array.
		void resize_array(usize size);

		/// @}
		/// @name Mapping
		/// @{

		/// Gets whether the buffer is mapped.
		/// @return `true` if the buffer is mapped, `false` otherwise.
		bool mapped() const;

		/// Maps the fixed header of the buffer.
		/// @return Map of the fixed header of the buffer.
		basic_graphics_buffer_map map_header();

		/// Maps the dynamic array of the buffer.
		/// @return Map of the dynamic array of the buffer.
		basic_graphics_buffer_map map_array();

		/// Maps the entire buffer.
		/// @return Map of the buffer.
		basic_graphics_buffer_map map();

		/// @}
		/// @name Label
		/// @{

		using graphics_buffer::label;

		using graphics_buffer::set_label;

		/// @}

	  private:
		// Map type of the buffer.
		map_type m_map_type;

		// Size of the header.
		usize m_header_size;

		// Current size of the array.
		usize m_array_size;

		// Capacity of the array.
		usize m_array_capacity;

		//

		/// Maps a range of the buffer.
		/// @param offset Starting offset of the map in bytes.
		/// @param size Size of the map in bytes.
		/// @return Buffer range map.
		basic_graphics_buffer_map map_range(usize offset, usize size);

		//

		// Accesses graphics_buffer.
		friend class shader_base;
	};

	/// Shader buffer with a typed header and array.
	/// @tparam Header Type of the header object stored at the front of the buffer.
	/// @tparam ArrayElement Type of the buffer dynamic array elements.
	template <typename Header, typename ArrayElement>
	class shader_buffer : private basic_shader_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		// Allocates an uninitialized shader buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param capacity Maximum capacity of the buffer array in elements.
		/// @param map_type Type of map to create when mapping the buffer.
		shader_buffer(graphics_context& context, usize capacity, map_type map_type = map_type::write_only);

		/// @}
		/// @name Context
		/// @{

		using basic_shader_buffer::context;

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the dynamic array.
		/// @return Size of the dynamic array in elements.
		usize array_size() const;

		/// Gets the maximum capacity of the dynamic array.
		/// @return Maximum capacity of the dynamic array in elements.
		usize array_capacity() const;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the header.
		/// @param header Header objet to copy into the buffer.
		void set_header(const Header& header);

		/// Sets the data of the dynamic array.
		/// @tparam Range Contiguous range of array elements.
		/// @param data Data to set the dynamic array to.
		template <typed_contiguous_const_range<ArrayElement> Range>
		void set_array(Range&& data);

		/// Resizes the dynamic array.
		/// @param size Size of the array in elements.
		/// @pre `size` must be less than or equal to the capacity of the array.
		void resize_array(usize size);

		/// @}
		/// @name Mapping
		/// @{

		using basic_shader_buffer::mapped;

		/// Maps the fixed header of the buffer.
		/// @return Map of the fixed header of the buffer.
		graphics_buffer_object_map<Header> map_header();

		/// Maps the dynamic array of the buffer.
		/// @return Map of the dynamic array of the buffer.
		graphics_buffer_span_map<ArrayElement> map_array();

		/// @}
		/// @name Label
		/// @{

		using basic_shader_buffer::label;

		using basic_shader_buffer::set_label;

		/// @}
	};

	/// Specialized shader buffer with no header before the array.
	/// @tparam Element Type of the array elements.
	template <typename Element>
	class shader_array : private basic_shader_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Allocates an uninitialized shader array.
		/// @param context Graphics context to create the array on.
		/// @param capacity Maximum capacity of the array in elements.
		/// @param map_type Type of map to create when mapping the array.
		shader_array(graphics_context& context, usize capacity, map_type map_type = map_type::write_only);

		/// @}
		/// @name Context
		/// @{

		using basic_shader_buffer::context;

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the array.
		/// @return Size of the array in elements.
		usize size() const;

		/// Gets the maximum capacity of the array.
		/// @return Maximum capacity of the array in elements.
		usize capacity() const;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the array.
		/// @tparam Range Contiguous range of elements.
		/// @param data Data to set the array to.
		template <typed_contiguous_const_range<Element> Range>
		void set(Range&& data);

		/// Resizes the array.
		/// @param size Size of the array in elements.
		/// @pre `size` must be less than or equal to the capacity of the array.
		void resize(usize size);

		/// @}
		/// @name Mapping
		/// @{

		using basic_shader_buffer::mapped;

		/// Maps the array.
		/// @return Map of the array.
		graphics_buffer_span_map<Element> map();

		/// @}
		/// @name Label
		/// @{

		using basic_shader_buffer::label;

		using basic_shader_buffer::set_label;

		/// @}
	};
} // namespace tr

#include "impl/shader_buffer.hpp" // IWYU pragma: export