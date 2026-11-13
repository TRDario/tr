/// @file
/// @brief Implements internal/graphics_object_id.hpp.

#include <tr/sysgfx/internal/graphics_object_id.hpp>

//

tr::internal::graphics_object_id tr::internal::generate_graphics_object_id() noexcept
{
	static std::atomic<unsigned int> id{1};
	return graphics_object_id{id++};
}