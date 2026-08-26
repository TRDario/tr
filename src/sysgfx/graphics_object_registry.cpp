/// @file
/// @brief Implements graphics_object_registry.hpp.

#include "../../include/tr/sysgfx/graphics_object_registry.hpp"

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

//

tr::graphics_object_id_handle& tr::graphics_object_id_handle::operator=(graphics_object_id_handle&& rhs)
{
	m_id = std::exchange(rhs.m_id, graphics_object_id::invalid);
	return *this;
}

//

tr::graphics_object_id_handle::operator graphics_object_id() const
{
	return m_id;
}