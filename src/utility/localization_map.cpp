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
	m_pool = rhs.m_pool;
	m_keys.clear();
	for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::advance(it, 2)) {
		m_keys.insert(it);
	}
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
	const auto it{m_keys.find(key)};
	if (it == m_keys.end()) {
		return key;
	}
	return std::string_view{*std::next(*it)};
}

//

void tr::localization_map::clear() noexcept
{
	m_pool.clear();
	m_keys.clear();
}

void tr::localization_map::update(std::string_view key, std::string_view value)
{
	const auto old_key_it{m_keys.find(key)};
	const string_pool::iterator new_key_it{m_pool.end()};
	const bool replaced_key{old_key_it != m_keys.end()};
	const usize old_pool_capacity{m_pool.capacity()};

	if (replaced_key) {
		m_pool.erase(*old_key_it, std::next(*old_key_it, 2));
	}
	m_pool.append(key);
	m_pool.append(value);

	if (replaced_key || m_pool.capacity() > old_pool_capacity) {
		m_keys.clear();
		for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::advance(it, 2)) {
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