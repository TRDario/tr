#include "../../include/tr/sysgfx/atlas.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/impl.hpp"

namespace tr {
	// Alias for a map of named bitmaps.
	using _named_bitmaps = tr::string_hash_map<tr::bitmap>;
	// Alias to an iterator to named bitmaps.
	using _named_bitmaps_it = _named_bitmaps::const_iterator;

	// Reasonable first guess at an atlas size.
	glm::ivec2 _initial_size(const _named_bitmaps& bitmaps) noexcept;
	// Creates a vector of iterators to bitmaps sorted by area.
	std::vector<_named_bitmaps_it> _sort_by_area(const _named_bitmaps& bitmaps);
	// Tries to pack all of the bitmaps into a rectangle.
	std::optional<string_hash_map<irect2>> _try_packing(glm::ivec2 size, const _named_bitmaps& bitmaps);
	// Attempts to pack until a non-nullopt result is reached.
	std::pair<glm::ivec2, string_hash_map<irect2>> _pack(const _named_bitmaps& bitmaps);
} // namespace tr

glm::ivec2 tr::_initial_size(const _named_bitmaps& bitmaps) noexcept
{
	glm::ivec2 size{};
	int bitmap_area{};
	for (auto& [name, bitmap] : bitmaps) {
		glm::ivec2 bitmap_size{bitmap.size()};
		if (bitmap_size.x > size.x) {
			size.x = std::bit_ceil((unsigned int)(bitmap_size.x));
		}
		if (bitmap_size.y > size.y) {
			size.y = std::bit_ceil((unsigned int)(bitmap_size.y));
		}
		bitmap_area += _area(bitmap.size());
	}
	while (bitmap_area > _area(size)) {
		size = _double_smaller_component(size);
	}
	return size;
}

std::vector<tr::_named_bitmaps_it> tr::_sort_by_area(const _named_bitmaps& bitmaps)
{
	constexpr auto GREATER_AREA{[](auto& l, auto& r) { return _area(l->second.size()) > _area(r->second.size()); }};

	const auto iterators{std::views::iota(bitmaps.begin(), bitmaps.end())};
	std::vector<_named_bitmaps_it> list{iterators.begin(), iterators.end()};
	std::ranges::sort(list, GREATER_AREA);
	return list;
}

std::optional<tr::string_hash_map<tr::irect2>> tr::_try_packing(glm::ivec2 size, const _named_bitmaps& bitmaps)
{
	tr::string_hash_map<irect2> rects;
	std::forward_list<irect2> free_rects{{{}, size}};
	for (auto& [name, bitmap] : _sort_by_area(bitmaps) | deref) {
		auto prev{_find_free_rect_prev(free_rects, bitmap.size())};
		if (prev == free_rects.end()) {
			return std::nullopt;
		}
		rects.emplace(name, irect2{std::next(prev)->tl, bitmap.size()});
		_shrink_free_rect(free_rects, prev, bitmap.size());
	}
	return rects;
}

std::pair<glm::ivec2, tr::string_hash_map<tr::irect2>> tr::_pack(const _named_bitmaps& bitmaps)
{
	glm::ivec2 size{_initial_size(bitmaps)};
	std::optional<string_hash_map<irect2>> rects{_try_packing(size, bitmaps)};
	while (!rects.has_value()) {
		size = _double_smaller_component(size);
		rects = _try_packing(size, bitmaps);
	}
	return {size, *std::move(rects)};
}

////////////////////////////////////////////////////////////// STATIC ATLAS ///////////////////////////////////////////////////////////////

tr::atlas_bitmap tr::build_atlas_bitmap(const _named_bitmaps& bitmaps, pixel_format format) noexcept
{
	try {
		auto [size, rects]{_pack(bitmaps)};
		bitmap atlas{size, format};
		for (auto& [name, bitmap] : bitmaps) {
			atlas.blit(rects.at(name).tl, bitmap);
		}
		return {std::move(atlas), std::move(rects)};
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while building an atlas bitmap.");
	}
}

tr::static_atlas::static_atlas(const atlas_bitmap& atlas_bitmap) noexcept
	: texture{atlas_bitmap.bitmap, true}
{
	try {
		const glm::vec2 size{glm::vec2(atlas_bitmap.bitmap.size())};
		for (auto& [name, rect] : atlas_bitmap.entries) {
			const frect2 frect{static_cast<frect2>(rect)};
			_entries.emplace(name, frect2{frect.tl / size, frect.size / size});
		}
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while creating a static atlas.");
	}
}

tr::static_atlas::static_atlas(const string_hash_map<bitmap>& bitmaps) noexcept
	: static_atlas{build_atlas_bitmap(bitmaps)}
{
}

bool tr::static_atlas::contains(std::string_view name) const noexcept
{
	return _entries.contains(name);
}

const tr::frect2& tr::static_atlas::operator[](std::string_view name) const noexcept
{
	TR_ASSERT(contains(name), "Tried to get nonexistent static atlas entry \"{}\".", name);

	return _entries.find(name)->second;
}

//////////////////////////////////////////////////////////////// DYN ATLAS ////////////////////////////////////////////////////////////////

tr::dyn_atlas::dyn_atlas() noexcept {}

tr::dyn_atlas::dyn_atlas(glm::ivec2 size) noexcept
	: _tex{size, true}, _free_rects{{{}, size}}
{
}

tr::dyn_atlas::operator const tr::texture&() const noexcept
{
	return _tex;
}

glm::ivec2 tr::dyn_atlas::size() const noexcept
{
	return _tex.size();
}

void tr::dyn_atlas::set_filtering(min_filter min_filter, mag_filter mag_filter) noexcept
{
	_tex.set_filtering(min_filter, mag_filter);
}

bool tr::dyn_atlas::contains(std::string_view name) const noexcept
{
	return _entries.contains(name);
}

std::size_t tr::dyn_atlas::entries() const noexcept
{
	return _entries.size();
}

tr::frect2 tr::dyn_atlas::operator[](std::string_view name) const noexcept
{
	TR_ASSERT(contains(name), "Tried to get nonexistent dynamic atlas entry \"{}\".", name);

	frect2 rect{_entries.find(name)->second};
	rect.tl /= _tex.size();
	rect.size /= _tex.size();
	return rect;
}

const tr::irect2& tr::dyn_atlas::raw_at(std::string_view name) const noexcept
{
	TR_ASSERT(contains(name), "Tried to get nonexistent dynamic atlas entry \"{}\".", name);

	return _entries.find(name)->second;
}

void tr::dyn_atlas::_raw_reserve(glm::ivec2 new_size) noexcept
{
	if (_tex.size() == glm::ivec2{0}) {
		_tex._allocate(new_size, true, pixel_format::RGBA32);
	}
	else {
		glm::ivec2 old_size{_tex.size()};
		if (new_size.x <= old_size.x && new_size.y <= old_size.y) {
			return;
		}
		texture new_tex{new_size};
		new_tex.copy_region({}, _tex, {{}, _tex.size()});
		for (auto& unit : _tex_unit_textures) {
			if (unit == _tex) {
				unit = new_tex;
			}
		}
		_tex = std::move(new_tex);
		if (!_label.empty()) {
			_tex.set_label(_label);
		}
	}
}

void tr::dyn_atlas::reserve(glm::ivec2 new_size) noexcept
{
	try {
		if (_tex.size() == glm::ivec2{0}) {
			_raw_reserve(new_size);
			_free_rects.emplace_front(glm::ivec2{0, 0}, new_size);
		}
		else {
			glm::ivec2 old_size{_tex.size()};
			_raw_reserve(new_size);
			_free_rects.emplace_front(glm::ivec2{old_size.x, 0}, glm::ivec2{new_size.x - old_size.x, old_size.y});
			_free_rects.emplace_front(glm::ivec2{0, old_size.y}, glm::ivec2{new_size.x, new_size.y - old_size.y});
		}
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while reserving space in a dynamic atlas.");
	}
}

std::forward_list<tr::irect2>::iterator tr::dyn_atlas::_find_free_rect_prev(glm::ivec2 size) noexcept
{
	try {
		if (_tex.size() == glm::ivec2{0, 0}) {
			const glm::uvec2 usize{static_cast<glm::uvec2>(size)};
			const glm::uvec2 new_size{std::bit_ceil(usize.x), std::bit_ceil(usize.y)};
			_raw_reserve(new_size);
			_free_rects.emplace_front(glm::ivec2{0, 0}, new_size);
			return _free_rects.before_begin();
		}
		else {
			_free_rect_it it{tr::_find_free_rect_prev(_free_rects, size)};
			if (it == _free_rects.end()) {
				const glm::ivec2 old_size{_tex.size()};
				glm::ivec2 new_size{_double_smaller_component(_tex.size())};
				const glm::ivec2 nrect1_tl{old_size.x, 0};
				const glm::ivec2 nrect2_tl{0, old_size.y};
				irect2& nrect1{_free_rects.emplace_front(nrect1_tl, glm::ivec2{new_size.x - old_size.x, old_size.y})};
				irect2& nrect2{_free_rects.emplace_front(nrect2_tl, glm::ivec2{new_size.x, new_size.y - old_size.y})};
				it = tr::_find_free_rect_prev(_free_rects, size);
				while (it == _free_rects.end()) {
					new_size = _double_smaller_component(new_size);
					nrect1 = {nrect1_tl, glm::ivec2{new_size.x - old_size.x, old_size.y}};
					nrect2 = {nrect2_tl, glm::ivec2{new_size.x, new_size.y - old_size.y}};
					it = tr::_find_free_rect_prev(_free_rects, size);
				}
				_raw_reserve(new_size);
			}
			return it;
		}
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while finding a free rectangle in a dynamic atlas.");
	}
}

void tr::dyn_atlas::add(const std::string& name, const tr::sub_bitmap& bitmap) noexcept
{
	const _free_rect_it prev{_find_free_rect_prev(bitmap.size())};
	const _free_rect_it it{std::next(prev)};

	try {
		_entries.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(it->tl, bitmap.size()));
		_tex.set_region(it->tl, bitmap);
		_shrink_free_rect(_free_rects, prev, bitmap.size());
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while adding a rectangle in a dynamic atlas.");
	}
}

void tr::dyn_atlas::add(std::string&& name, const tr::sub_bitmap& bitmap) noexcept
{
	const _free_rect_it prev{_find_free_rect_prev(bitmap.size())};
	const _free_rect_it it{std::next(prev)};

	try {
		_entries.emplace(std::piecewise_construct, std::forward_as_tuple(std::move(name)), std::forward_as_tuple(it->tl, bitmap.size()));
		_tex.set_region(it->tl, bitmap);
		_shrink_free_rect(_free_rects, prev, bitmap.size());
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while adding a rectangle in a dynamic atlas.");
	}
}

void tr::dyn_atlas::remove(std::string_view name) noexcept
{
	try {
		decltype(_entries)::const_iterator it{_entries.find(name)};
		if (it != _entries.end()) {
			_entries.erase(it);
			_free_rects.emplace_front(it->second.tl, it->second.size);
		}
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while removing a rectangle in a dynamic atlas.");
	}
}

void tr::dyn_atlas::clear() noexcept
{
	if (size() != glm::ivec2{0}) {
		_entries.clear();
		_free_rects.clear();
		_free_rects.emplace_front(glm::ivec2{}, _tex.size());
		_tex.clear({});
	}
}

void tr::dyn_atlas::set_label(const std::string& label) noexcept
{
	try {
		_label = label;
		_tex.set_label(label);
	}
	catch (std::bad_alloc&) {
		terminate("Out of memory", "Exception occurred while setting the label of a dynamic atlas.");
	}
}

void tr::dyn_atlas::set_label(std::string&& label) noexcept
{
	_label = std::move(label);
	_tex.set_label(_label);
}
