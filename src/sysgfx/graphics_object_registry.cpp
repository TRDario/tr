/// @file
/// @brief Implements internal/graphics_object_registry.hpp.

#include <tr/sysgfx/internal/graphics_object_registry.hpp>

//

tr::internal::graphics_object_id_handle::graphics_object_id_handle() noexcept
{
	static std::atomic<unsigned int> id{1};
	m_id = graphics_object_id{id++};
}

tr::internal::graphics_object_id_handle::graphics_object_id_handle(graphics_object_id_handle&& rhs) noexcept
	: m_id{std::exchange(rhs.m_id, graphics_object_id::invalid)}
{
}

//

tr::internal::graphics_object_id_handle& tr::internal::graphics_object_id_handle::operator=(graphics_object_id_handle&& rhs) noexcept
{
	m_id = std::exchange(rhs.m_id, graphics_object_id::invalid);
	return *this;
}

//

tr::internal::graphics_object_id_handle::operator graphics_object_id() const noexcept
{
	return m_id;
}