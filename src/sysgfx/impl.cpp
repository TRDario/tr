#include "../../include/tr/sysgfx/impl.hpp"

glm::ivec2 tr::_double_smaller_component(glm::ivec2 size) noexcept
{
	size.y < size.x ? size.y *= 2 : size.x *= 2;
	return size;
}

int tr::_area(glm::ivec2 size) noexcept
{
	return size.x * size.y;
}

tr::_free_rect_it tr::_find_free_rect_prev(_free_rect_list& free_rects, glm::ivec2 size) noexcept
{
	_free_rect_it min_prev{free_rects.end()};
	int min_area{std::numeric_limits<int>::max()};
	for (auto [prev, it] = std::pair(free_rects.before_begin(), free_rects.begin()); it != free_rects.end(); prev = it++) {
		if (it->size.x < size.x || it->size.y < size.y) {
			continue;
		}
		int cur_area{_area(it->size)};
		if (min_prev == free_rects.end() || cur_area < min_area) {
			min_prev = prev;
			min_area = cur_area;
		}
	}
	return min_prev;
}

void tr::_shrink_free_rect(_free_rect_list& free_rects, _free_rect_it prev, glm::ivec2 size)
{
	const _free_rect_it rect{std::next(prev)};

	if (rect->size == size) {
		free_rects.erase_after(prev);
		return;
	}
	if (rect->size.x > size.x) {
		if (rect->size.y > size.y) {
			const glm::vec2 new_free_rect_tl{rect->tl.x, rect->tl.y + size.y};
			const glm::vec2 new_free_rect_size{size.x, rect->size.y - size.y};
			free_rects.emplace_front(new_free_rect_tl, new_free_rect_size);
		}
		rect->tl.x += size.x;
		rect->size.x -= size.x;
		return;
	}
	if (rect->size.y > size.y) {
		if (rect->size.x > size.x) {
			const glm::vec2 new_free_rect_size{rect->size.x, size.y};
			free_rects.emplace_front(rect->tl, new_free_rect_size);
		}
		rect->tl.y += size.y;
		rect->size.y -= size.y;
	}
}