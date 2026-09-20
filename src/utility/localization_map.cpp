/// @file
/// @brief Implements localization_map.hpp.

#include <tr/utility/localization_map.hpp>

//

tr::localization_map::localization_map(const localization_map& rhs)
	: m_string_pool{rhs.m_string_pool}
{
	build_keys();
}

//

tr::localization_map& tr::localization_map::operator=(const localization_map& rhs)
{
	m_string_pool = rhs.m_string_pool;
	m_keys.clear();
	build_keys();
	return *this;
}

//

tr::usize tr::localization_map::size() const noexcept
{
	return m_keys.size();
}

bool tr::localization_map::contains(std::string_view key) const noexcept
{
	return m_keys.contains(key);
}

std::string_view tr::localization_map::operator[](std::string_view key) const noexcept
{
	const boost::unordered_flat_set<zstring_view, string_hash, string_eq>::iterator it{m_keys.find(key)};
	if (it == m_keys.end()) {
		return key;
	}
	return std::string_view{it->c_str() + it->length() + 1};
}

//

void tr::localization_map::clear() noexcept
{
	m_string_pool.clear();
	m_keys.clear();
}

void tr::localization_map::reserve(usize capacity)
{
	const usize old_pool_capacity{m_string_pool.capacity()};
	m_string_pool.reserve(capacity * 64); // Heuristic based on Bodge localization.
	m_keys.reserve(capacity);
	if (m_string_pool.capacity() > old_pool_capacity) {
		m_keys.clear();
		build_keys();
	}
}

void tr::localization_map::insert(std::string_view key, std::string_view value)
{
	TR_ASSERT(!contains(key), "Tried to insert duplicate key {} into a localization map.", key);

	const usize new_key_begin_index{m_string_pool.size()};
	const usize old_pool_capacity{m_string_pool.capacity()};
	m_string_pool.append_range(key);
	m_string_pool.push_back('\0');
	m_string_pool.append_range(value);
	m_string_pool.push_back('\0');
	if (m_string_pool.capacity() > old_pool_capacity) {
		m_keys.clear();
		build_keys();
	}
	else {
		m_keys.emplace(&m_string_pool[new_key_begin_index]);
	}
}

//

std::vector<char>::const_iterator tr::localization_map::find_next_key(std::vector<char>::const_iterator it) const noexcept
{
	namespace rs = std::ranges;

	it = rs::next(rs::find(it, m_string_pool.end(), '\0'));
	return rs::next(rs::find(it, m_string_pool.end(), '\0'));
}

void tr::localization_map::build_keys()
{
	for (std::vector<char>::const_iterator it{m_string_pool.begin()}; it != m_string_pool.end(); it = find_next_key(it)) {
		m_keys.emplace(std::to_address(it));
	}
}