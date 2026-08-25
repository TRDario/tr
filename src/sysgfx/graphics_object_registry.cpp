/// @file
/// @brief Implements graphics_object_registry.hpp.

#include "../../include/tr/sysgfx/graphics_object_registry.hpp"
#include "../../include/tr/utility/macro.hpp"

//

tr::graphics_object_id tr::generate_graphics_object_id()
{
	static std::atomic<unsigned int> id{1};
	return graphics_object_id{id++};
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