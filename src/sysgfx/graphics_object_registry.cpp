/// @file
/// @brief Implements graphics_object_registry.hpp.

#include "../../include/tr/sysgfx/graphics_object_registry.hpp"
#include "../../include/tr/utility/macro.hpp"

//

tr::graphics_object_id_handle::graphics_object_id_handle()
{
	static std::atomic<unsigned int> id{1};
	m_id = graphics_object_id{id++};
}

tr::graphics_object_id_handle::graphics_object_id_handle(graphics_object_id_handle&& rhs)
	: m_id{std::exchange(rhs.m_id, graphics_object_id::invalid)}
{
}

tr::graphics_object_id_handle& tr::graphics_object_id_handle::operator=(graphics_object_id_handle&& rhs)
{
	m_id = std::exchange(rhs.m_id, graphics_object_id::invalid);
	return *this;
}

tr::graphics_object_id_handle::operator graphics_object_id() const
{
	return m_id;
}

//

tr::usize tr::graphics_object_registry::registered_shader_count() const
{
	return m_shaders.size();
}

bool tr::graphics_object_registry::is_shader_valid(graphics_object_id id) const
{
	return m_shaders.contains(id);
}

void tr::graphics_object_registry::register_shader(graphics_object_id id, unsigned int glid)
{
	TR_ASSERT(!m_shaders.contains(id), "Tried to register duplicate shader (ID: '{}')", std::to_underlying(id));

	m_shaders.emplace(id, glid);
}

void tr::graphics_object_registry::unregister_shader(graphics_object_id id)
{
	TR_ASSERT(m_shaders.contains(id), "Tried to unregister nonexistant shader (ID: '{}')", std::to_underlying(id));

	m_shaders.erase(id);
}

//

tr::usize tr::graphics_object_registry::registered_shader_pipeline_count() const
{
	return m_shader_pipelines.size();
}

bool tr::graphics_object_registry::is_shader_pipeline_valid(graphics_object_id id) const
{
	return m_shader_pipelines.contains(id);
}

void tr::graphics_object_registry::register_shader_pipeline(graphics_object_id id, unsigned int glid)
{
	TR_ASSERT(!m_shader_pipelines.contains(id), "Tried to register duplicate shader pipeline (ID: '{}')", std::to_underlying(id));

	m_shader_pipelines.emplace(id, glid);
}

void tr::graphics_object_registry::unregister_shader_pipeline(graphics_object_id id)
{
	TR_ASSERT(m_shader_pipelines.contains(id), "Tried to unregister nonexistant shader pipeline (ID: '{}')", std::to_underlying(id));

	m_shader_pipelines.erase(id);
}

//

tr::usize tr::graphics_object_registry::registered_vertex_format_count() const
{
	return m_vertex_formats.size();
}

bool tr::graphics_object_registry::is_vertex_format_valid(graphics_object_id id) const
{
	return m_vertex_formats.contains(id);
}

void tr::graphics_object_registry::register_vertex_format(graphics_object_id id, unsigned int glid)
{
	TR_ASSERT(!m_vertex_formats.contains(id), "Tried to register duplicate vertex format (ID: '{}')", std::to_underlying(id));

	m_vertex_formats.emplace(id, glid);
}

void tr::graphics_object_registry::unregister_vertex_format(graphics_object_id id)
{
	TR_ASSERT(m_vertex_formats.contains(id), "Tried to unregister nonexistant vertex format (ID: '{}')", std::to_underlying(id));

	m_vertex_formats.erase(id);
}