/// @file
/// @brief Implements gl_object_registry.hpp.

#include "../../include/tr/sysgfx/gl_object_registry.hpp"
#include "../../include/tr/utility/macro.hpp"

//

bool tr::gl_object_registry::check_shader(unsigned int tid, unsigned gid) const
{
	auto it{m_shaders.find(tid)};
	return it != m_shaders.end() && it->second == gid;
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