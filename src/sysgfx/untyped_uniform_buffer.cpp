/// @file
/// @brief Implements untyped_uniform_buffer.hpp.

#include "internal/opengl_definitions.hpp"
#include <tr/sysgfx/graphics_context.hpp>
#include <tr/sysgfx/mapped_untyped_graphics_buffer_span.hpp>
#include <tr/sysgfx/untyped_uniform_buffer.hpp>
#include <tr/utility/exception.hpp>

//

namespace
{
	/// @brief Uniform buffer storage flags.
	constexpr unsigned int storage_flags{GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT};
} // namespace

//

tr::untyped_uniform_buffer::untyped_uniform_buffer(graphics_context& context, usize size, std::string_view label)
	: graphics_buffer{context, label}
	, m_size{size}
{
	const internal::opengl& gl{context.gl()};
	gl.allocate_buffer_storage(handle(), size, nullptr, storage_flags);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"allocation of uniform buffer {}", *this};
	}
}

//

void tr::untyped_uniform_buffer::set(std::span<const std::byte> data) noexcept
{
	TR_ASSERT(valid(), "Tried to set a uniform buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to set the header of mapped uniform buffer {}.", *this);
	TR_ASSERT(data.size() == size(), "Tried to set uniform buffer {} of size {} with data of size {}.", *this, size(), data.size());

	context().gl().set_buffer_sub_data(handle(), 0, data.size(), data.data());
}

tr::mapped_untyped_graphics_buffer_span tr::untyped_uniform_buffer::map()
{
	TR_ASSERT(valid(), "Tried to map a uniform buffer in an invalid state.");
	TR_ASSERT(!mapped(), "Tried to map already-mapped uniform buffer {}.", *this);

	const internal::opengl& gl{context().gl()};
	std::byte* const map_pointer{static_cast<std::byte*>(gl.map_buffer_range(handle(), 0, m_size, storage_flags))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"mapping of uniform buffer {}", *this};
	}
	return mapped_untyped_graphics_buffer_span{context(), handle(), std::span{map_pointer, m_size}};
}

//

tr::usize tr::untyped_uniform_buffer::size() const noexcept
{
	TR_ASSERT(valid(), "Tried to get the size of a uniform buffer in an invalid state.");

	return m_size;
}