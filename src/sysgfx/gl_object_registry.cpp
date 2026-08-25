/// @file
/// @brief Implements gl_object_registry.hpp.

#include "../../include/tr/sysgfx/gl_object_registry.hpp"
#include "../../include/tr/utility/macro.hpp"

//

tr::graphics_object_id tr::generate_graphics_object_id()
{
	static std::atomic<unsigned int> id{1};
	return graphics_object_id{id++};
}

//

tr::usize tr::gl_object_registry::registered_shader_count() const
{
	return m_shaders.size();
}

bool tr::gl_object_registry::is_shader_valid(graphics_object_id id) const
{
	return m_shaders.contains(id);
}

void tr::gl_object_registry::register_shader(graphics_object_id id, unsigned int glid)
{
	TR_ASSERT(!m_shaders.contains(id), "Tried to register duplicate shader (ID: '{}')", std::to_underlying(id));

	m_shaders.emplace(id, glid);
}

void tr::gl_object_registry::unregister_shader(graphics_object_id id)
{
	TR_ASSERT(m_shaders.contains(id), "Tried to unregister nonexistant shader (ID: '{}')", std::to_underlying(id));

	m_shaders.erase(id);
}

void tr::gl_object_registry::register_shader(unsigned int tid, unsigned int gid)
{
	TR_ASSERT(!m_shaders.contains(tid), "Tried to reg9ister duplicate shader (ID: '{}')", tid);

	m_shaders.emplace(tid, gid);
}

void tr::gl_object_registry::unregister_shader(unsigned int tid)
{
	TR_ASSERT(m_shaders.contains(tid), "Tried to unregister nonexistant shader (ID: '{}')", tid);

	m_shaders.erase(tid);
}