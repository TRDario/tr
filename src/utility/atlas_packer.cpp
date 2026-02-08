#include "../../include/tr/utility/atlas_packer.hpp"

tr::atlas_packer::atlas_packer()
	: m_skyline{{0, 0}}
{
}

void tr::atlas_packer::clear()
{
	m_skyline.clear();
	m_skyline.push_back({});
}

std::optional<glm::u16vec2> tr::atlas_packer::try_insert(glm::u16vec2 size, glm::u16vec2 texture_size)
{
	// Add a little buffer space to prevent filtering artifacts.
	size += 1;

	std::vector<glm::u16vec2>::iterator best_begin_it{m_skyline.end()};
	std::vector<glm::u16vec2>::iterator best_end_it{m_skyline.end()};
	glm::u16vec2 best{UINT16_MAX};
	for (std::vector<glm::u16vec2>::iterator begin_it = m_skyline.begin(); begin_it != m_skyline.end(); ++begin_it) {
		glm::u16vec2 p{*begin_it};

		if (size.x > texture_size.x - p.x) {
			break;
		}
		if (p.y >= best.y) {
			continue;
		}

		const u16 x_max = p.x + size.x;
		std::vector<glm::u16vec2>::iterator end_it;
		for (end_it = std::next(begin_it); end_it < m_skyline.end(); ++end_it) {
			if (x_max <= end_it->x) {
				break;
			}
			if (p.y < end_it->y) {
				p.y = end_it->y;
			}
		}

		if (p.y >= best.y || size.y > texture_size.y - p.y) {
			continue;
		}

		best_begin_it = begin_it;
		best_end_it = end_it;
		best = p;
	}

	if (best_begin_it == m_skyline.end()) {
		return std::nullopt;
	}

	const glm::u16vec2 tl{best.x, best.y + size.y};
	const glm::u16vec2 br{best.x + size.x, std::prev(best_end_it)->y};
	best_begin_it = m_skyline.erase(best_begin_it, best_end_it);
	best_end_it = ++m_skyline.emplace(best_begin_it, tl);
	if ((best_end_it != m_skyline.end() && br.x < best_end_it->x) || (best_end_it == m_skyline.end() && br.x < texture_size.x)) {
		m_skyline.emplace(best_end_it, br);
	}

	return best;
}