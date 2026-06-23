///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements the non-templated parts of shader_buffer.hpp.                                                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/gl_defines.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"

/////////////////////////////////////////////////////////// BASIC SHADER BUFFER ///////////////////////////////////////////////////////////

tr::basic_shader_buffer::basic_shader_buffer(graphics_context& context, usize header_size, usize capacity, map_type map_type)
	: graphics_buffer{context}
	, m_map_type{map_type}
	, m_header_size{header_size}
	, m_array_size{0}
	, m_array_capacity{capacity}
{
	const graphics_context::functions& gl{context.make_current_and_return_functions()};

	gl.named_buffer_storage(id(), header_size + capacity, nullptr, (unsigned int)(map_type) | GL_DYNAMIC_STORAGE_BIT);
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
		throw out_of_memory{"shader buffer allocation"};
	}
}

tr::usize tr::basic_shader_buffer::header_size() const
{
	return m_header_size;
}

tr::usize tr::basic_shader_buffer::array_size() const
{
	return m_array_size;
}

tr::usize tr::basic_shader_buffer::array_capacity() const
{
	return m_array_capacity;
}

void tr::basic_shader_buffer::set_header(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the header of mapped shader buffer '{}'.", label());
	TR_ASSERT(header_size() != 0, "Tried to set the header for shader buffer '{}' without one.", label());
	TR_ASSERT(data.size() == header_size(), "Tried to set header of shader buffer '{}' of size {} with data of size {}.", label(),
			  header_size(), data.size());

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	gl.named_buffer_sub_data(id(), 0, data.size(), data.data());
}

void tr::basic_shader_buffer::set_array(std::span<const std::byte> data)
{
	TR_ASSERT(!mapped(), "Tried to set the array of a mapped shader buffer.");
	TR_ASSERT(array_capacity() != 0, "Tried to set the array for a shader buffer without one.");
	TR_ASSERT(data.size() <= array_capacity(), "Tried to set a shader buffer array of capacity {} with data of size {}.", array_capacity(),
			  data.size());

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	if (!data.empty()) {
		gl.named_buffer_sub_data(id(), m_header_size, data.size(), data.data());
	}
	m_array_size = data.size();
}

void tr::basic_shader_buffer::resize_array(usize size)
{
	TR_ASSERT(!mapped(), "Tried to resize the array of mapped shader buffer '{}'.", label());
	TR_ASSERT(size <= array_capacity(), "Tried to resize array of shader buffer '{}' of capacity {} to size {}.", label(), array_capacity(),
			  size);

	m_array_size = size;
}

bool tr::basic_shader_buffer::mapped() const
{
	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	int mapped;
	gl.get_named_buffer_parameter_iv(id(), GL_BUFFER_MAPPED, &mapped);
	return mapped;
}

tr::basic_graphics_buffer_map tr::basic_shader_buffer::map_range(usize offset, usize size)
{
	TR_ASSERT(!mapped(), "Tried to map the header of already-mapped shader buffer '{}'.", label());

	const graphics_context::functions& gl{context().make_current_and_return_functions()};

	std::byte* const map_pointer{(std::byte*)gl.map_named_buffer_range(id(), offset, size, (unsigned int)(m_map_type))};
	if (gl.get_error() == GL_OUT_OF_MEMORY) {
#ifdef TR_ENABLE_ASSERTS
		throw out_of_memory{"mapping of shader buffer '{}'", label()};
#else
		throw out_of_memory{"shader buffer mapping"};
#endif
	}
	return basic_graphics_buffer_map{context(), id(), std::span{map_pointer, size}};
}

tr::basic_graphics_buffer_map tr::basic_shader_buffer::map_header()
{
	TR_ASSERT(header_size() != 0, "Tried to map the header of shader buffer '{}' that doesn't have one.", label());

	return map_range(0, m_header_size);
}

tr::basic_graphics_buffer_map tr::basic_shader_buffer::map_array()
{
	TR_ASSERT(array_size() != 0, "Tried to map the array of shader buffer '{}' that doesn't have one.", label());

	return map_range(m_array_size, m_array_size);
}

tr::basic_graphics_buffer_map tr::basic_shader_buffer::map()
{
	return map_range(0, m_header_size + m_array_size);
}