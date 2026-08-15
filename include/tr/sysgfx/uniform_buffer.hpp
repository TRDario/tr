/// @file
/// @brief Provides uniform buffer classes.

#pragma once
#include "graphics_buffer.hpp"
#include "graphics_buffer_map.hpp"

//

namespace tr
{
	/// Basic shader uniform buffer.
	class basic_uniform_buffer : private graphics_buffer
	{
	  public:
		/// Allocates an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		/// @param size Initial size of the buffer.
		basic_uniform_buffer(graphics_context& context, usize size);

		/// @name Context
		/// @{

		using graphics_buffer::context;

		/// @}
		/// @name Size
		/// @{

		/// Gets the size of the buffer.
		/// @return Size of the buffer in bytes.
		usize size() const;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the data of the buffer.
		/// @param data Data to copy into the buffer.
		void set(std::span<const std::byte> data);

		/// @}
		/// @name Mapping
		/// @{

		/// Gets whether the buffer is mapped.
		/// @return `true` if the buffer is mapped, `false` otherwise.
		bool mapped() const;

		/// Maps the buffer.
		/// @return Write-only map of the buffer.
		basic_graphics_buffer_map map();

		/// @}
		/// @name Label
		/// @{

		using graphics_buffer::label;

		using graphics_buffer::set_label;

		/// @}
		/// @name ID
		/// @{

		using graphics_buffer::id;

		/// @}

	  private:
		/// Size of the buffer.
		usize m_size;
	};

	//

	/// Typed shader uniform buffer.
	/// @tparam Object Objet contained in the buffer.
	template <typename Object>
	class uniform_buffer : private basic_uniform_buffer
	{
	  public:
		/// @name Constructors
		/// @{

		/// Allocates an uninitialized uniform buffer.
		/// @param context Graphics context to create the buffer on.
		uniform_buffer(graphics_context& context);

		/// @}
		/// @name Context
		/// @{

		using basic_uniform_buffer::context;

		/// @}
		/// @name Setting
		/// @{

		/// Sets the contents of the buffer.
		/// @param data Object to copy into the buffer.
		void set(const Object& data);

		/// @}
		/// @name Mapping
		/// @{

		using basic_uniform_buffer::mapped;

		/// Maps the buffer.
		/// @return Write-only map of the buffer object.
		graphics_buffer_object_map<Object> map();

		/// @}
		/// @name Label
		/// @{

		using basic_uniform_buffer::label;

		using basic_uniform_buffer::set_label;

		/// @}
	};
} // namespace tr

#include "impl/uniform_buffer.hpp" // IWYU pragma: export