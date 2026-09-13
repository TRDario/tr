/// @file
/// @brief Implements untyped_uniform_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_uniform_buffer.hpp>
#include <tr/utility/exception.hpp>

//

tr::untyped_uniform_buffer::untyped_uniform_buffer(graphics_context& context, usize size)
	: graphics_buffer{context}
	, m_size{size}
{
	const internal::opengl& gl{context.gl()};
	gl.allocate_buffer_storage(unwrap(), size, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"uniform buffer allocation"};
	}
}

tr::usize tr::untyped_uniform_buffer::size() const noexcept
{
	return m_size;
}

void tr::untyped_uniform_buffer::set(std::span<const std::byte> data) noexcept
{
	TR_ASSERT(!mapped(), "Tried to set the header of mapped uniform buffer '{}'.", label());
	TR_ASSERT(data.size() == size(), "Tried to set uniform buffer '{}' of size {} with data of size {}.", label(), size(), data.size());

	context().gl().set_buffer_sub_data(unwrap(), 0, data.size(), data.data());
}

bool tr::untyped_uniform_buffer::mapped() const noexcept
{
	int mapped;
	context().gl().get_buffer_parameter_iv(unwrap(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_uniform_buffer::map()
{
	TR_ASSERT(!mapped(), "Tried to map already-mapped uniform buffer '{}'.", label());

	const internal::opengl& gl{context().gl()};
	const unsigned int flags{GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT};
	std::byte* const map_pointer{static_cast<std::byte*>(gl.map_buffer_range(unwrap(), 0, m_size, flags))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"mapping of uniform buffer '{}'", label()};
	}
	return mapped_untyped_graphics_buffer_span{context(), unwrap(), std::span{map_pointer, m_size}};
}