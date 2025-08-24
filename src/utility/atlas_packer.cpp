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
	std::vector<glm::u16vec2>::iterator best_begin{m_skyline.end()};
	std::vector<glm::u16vec2>::iterator best_end{m_skyline.end()};
	glm::u16vec2 best{UINT16_MAX};
	for (std::vector<glm::u16vec2>::iterator it = m_skyline.begin(); it != m_skyline.end(); ++it) {
		glm::u16vec2 p{*it};

		if (size.x > texture_size.x - p.x) {
			break;
		}
		if (p.y >= best.y) {
			continue;
		}

		const u16 x_max = p.x + size.x;
		std::vector<glm::u16vec2>::iterator jt;
		for (jt = std::next(it); jt < m_skyline.end(); ++jt) {
			if (x_max <= jt->x) {
				break;
			}
			if (p.y < jt->y) {
				p.y = jt->y;
			}
		}

		if (p.y >= best.y || size.y > texture_size.y - p.y) {
			continue;
		}

		best_begin = it;
		best_end = jt;
		best = p;
	}

	if (best_begin == m_skyline.end()) {
		return std::nullopt;
	}

	const glm::u16vec2 tl{best.x, best.y + size.y};
	const glm::u16vec2 br{best.x + size.x, std::prev(best_end)->y};
	best_begin = m_skyline.erase(best_begin, best_end);
	best_end = ++m_skyline.emplace(best_begin, tl);
	if ((best_end != m_skyline.end() && br.x < best_end->x) || (best_end == m_skyline.end() && br.x < texture_size.x)) {
		m_skyline.emplace(best_end, br);
	}

	return best;
}