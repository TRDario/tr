/// @file
/// @brief Implements string_pool.hpp.

#include <tr/utility/string_pool.hpp>

//

tr::string_pool::iterator::iterator(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator base) noexcept
	: m_begin{begin}
	, m_base{base}
{
}

tr::string_pool::iterator& tr::string_pool::iterator::operator++() noexcept
{
	while (*m_base++ != '\0') {}
	return *this;
}

tr::string_pool::iterator& tr::string_pool::iterator::operator--() noexcept
{
	--m_base;
	while (m_base != m_begin && *(m_base - 1) != '\0') {
		--m_base;
	}
	return *this;
}

tr::string_pool::iterator::value_type tr::string_pool::iterator::operator*() const noexcept
{
	return zstring_view{std::to_address(m_base)};
}

std::vector<char>::const_iterator tr::string_pool::iterator::base() const noexcept
{
	return m_base;
}

//

tr::string_pool::string_builder_t::string_builder_t(string_pool& pool) noexcept
	: m_pool{pool}
{
}

tr::string_pool::string_builder_t::string_builder_t(string_builder_t&& rhs) noexcept
	: m_pool{std::exchange(rhs.m_pool, std::nullopt)}
{
}

tr::string_pool::string_builder_t::~string_builder_t() noexcept
{
	if (m_pool.has_value()) {
		m_pool->m_data.push_back('\0');
	}
}

tr::string_pool::string_builder_t& tr::string_pool::string_builder_t::operator=(string_builder_t&& rhs) noexcept
{
	if (m_pool.has_value()) {
		m_pool->m_data.push_back('\0');
	}
	m_pool = std::exchange(rhs.m_pool, std::nullopt);
	return *this;
}

std::back_insert_iterator<std::vector<char>> tr::string_pool::string_builder_t::out() const noexcept
{
	TR_ASSERT(m_pool.has_value(), "Tried to get output iterator from a moved-from string builder.");

	return std::back_inserter(m_pool->m_data);
}

//

tr::string_pool::iterator tr::string_pool::begin() const noexcept
{
	return iterator{m_data.begin(), m_data.begin()};
}

tr::string_pool::iterator tr::string_pool::end() const noexcept
{
	return iterator{m_data.begin(), m_data.end()};
}

//

bool tr::string_pool::empty() const noexcept
{
	return m_data.empty();
}

tr::usize tr::string_pool::capacity() const noexcept
{
	return m_data.capacity();
}

void tr::string_pool::reserve(usize capacity)
{
	m_data.reserve(capacity);
}

//

void tr::string_pool::clear() noexcept
{
	m_data.clear();
}

tr::string_pool::iterator tr::string_pool::append(std::string_view string)
{
	TR_ASSERT(!std::ranges::contains(string, '\0'), "Tried to append string with embedded NUL characters to a string pool.");

	const usize offset{m_data.size()};
	m_data.append_range(string);
	m_data.push_back('\0');
	return iterator{m_data.begin(), m_data.begin() + offset};
}

tr::string_pool::string_builder_t tr::string_pool::string_builder() noexcept
{
	return string_builder_t{*this};
}

tr::string_pool::iterator tr::string_pool::erase(iterator where) noexcept
{
	const decltype(m_data)::const_iterator base{m_data.erase(where.base(), std::next(where).base())};
	return iterator{m_data.begin(), base};
}

tr::string_pool::iterator tr::string_pool::erase(iterator begin, iterator end) noexcept
{
	const decltype(m_data)::const_iterator base{m_data.erase(begin.base(), end.base())};
	return iterator{m_data.begin(), base};
}