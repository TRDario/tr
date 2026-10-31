/// @file
/// @brief Implements localization_map.hpp.

#include <tr/utility/localization_map.hpp>

//

tr::localization_map::localization_map(const localization_map& rhs)
	: m_pool{rhs.m_pool}
{
	for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::advance(it, 2)) {
		m_keys.insert(it);
	}
}

//

tr::localization_map& tr::localization_map::operator=(const localization_map& rhs)
{
	try {
		m_pool = rhs.m_pool;
		m_keys.clear();
		for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::advance(it, 2)) {
			m_keys.insert(it);
		}
		return *this;
	}
	catch (std::exception& error) {
		clear();
		throw error;
	}
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
	const decltype(m_keys)::const_iterator it{m_keys.find(key)};
	if (it == m_keys.end()) {
		return key;
	}
	return std::string_view{*std::ranges::next(*it)};
}

std::string_view tr::localization_map::value_or(std::string_view key, std::string_view default_value) const noexcept
{
	const decltype(m_keys)::const_iterator it{m_keys.find(key)};
	if (it == m_keys.end()) {
		return default_value;
	}
	return std::string_view{*std::ranges::next(*it)};
}

//

void tr::localization_map::clear() noexcept
{
	m_pool.clear();
	m_keys.clear();
}

void tr::localization_map::update(std::string_view key, std::string_view value)
{
	const usize old_size{size()};
	const decltype(m_keys)::iterator key_it_it{m_keys.find(key)};
	const bool replaced_key{key_it_it != m_keys.end()};
	string_pool::iterator key_it{replaced_key ? *key_it_it : m_pool.end()};

	const usize erased_bytes{replaced_key ? key.size() + std::ranges::distance(*std::ranges::next(key_it)) + 2uz : 0uz};
	const usize appended_bytes{key.size() + value.size() + 2};
	const usize new_pool_size{m_pool.size_bytes() - erased_bytes + appended_bytes};
	const bool reallocated_pool{m_pool.capacity_bytes() < new_pool_size};

	if (!replaced_key) {
		m_keys.reserve(old_size + 1);
	}
	if (reallocated_pool) {
		const ssize raw_key_distance{std::ranges::distance(m_pool.begin().base(), key_it.base())};
		m_pool.reserve_bytes(std::bit_ceil(new_pool_size));
		m_keys.clear();
		key_it = string_pool::iterator{m_pool.begin().base(), m_pool.begin().base() + raw_key_distance};
	}

	if (replaced_key) {
		m_keys.clear();
		m_pool.erase(key_it, std::ranges::next(key_it, 2));
	}
	const string_pool::iterator new_key_it{m_pool.append(key)};
	m_pool.append(value);

	if (replaced_key || reallocated_pool) {
		for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::ranges::advance(it, 2)) {
			m_keys.insert(it);
		}
	}
	else {
		m_keys.insert(new_key_it);
	}
}

//

tr::usize tr::localization_map::string_pool_iterator_hash::operator()(string_pool::iterator it) const noexcept
{
	return (*this)(*it);
}

bool tr::localization_map::string_pool_iterator_eq::operator()(string_pool::iterator lhs, std::string_view rhs) const noexcept
{
	return *lhs == rhs;
}

bool tr::localization_map::string_pool_iterator_eq::operator()(std::string_view lhs, string_pool::iterator rhs) const noexcept
{
	return lhs == *rhs;
}