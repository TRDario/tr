///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements utf8.hpp.                                                                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../utf8.hpp"

///////////////////////////////////////////////////// BASIC TRAVERSAL AND MANIPULATION ////////////////////////////////////////////////////

template <tr::utf8::base_iterator Iterator> constexpr tr::codepoint tr::utf8::to_cp(Iterator it)
{
	if (u8(*it) < 0x80) {
		return u8(*it);
	}
	else if (u8(*it) < 0xE0) {
		return ((u8(it[0]) & 0x1F) << 6) | (u8(it[1]) & 0x3F);
	}
	else if (u8(*it) < 0xF0) {
		return ((u8(it[0]) & 0xF) << 12) | ((u8(it[1]) & 0x3F) << 6) | (u8(it[2]) & 0x3F);
	}
	else {
		return ((u8(it[0]) & 0x7) << 18) | ((u8(it[1]) & 0x3F) << 12) | ((u8(it[2]) & 0x3F) << 6) | (u8(it[3]) & 0x3F);
	}
}

template <tr::utf8::base_iterator Iterator> constexpr Iterator tr::utf8::next(Iterator it)
{
	if (u8(*it) < 0x80) {
		it += 1;
	}
	else if (u8(*it) < 0xE0) {
		it += 2;
	}
	else if (u8(*it) < 0xF0) {
		it += 3;
	}
	else {
		it += 4;
	}
	return it;
}

template <tr::utf8::base_iterator Iterator> constexpr Iterator tr::utf8::next(Iterator it, usize n)
{
	for (usize i = 0; i < n; ++i) {
		it = tr::utf8::next(it);
	}
	return it;
}

template <tr::utf8::base_iterator Iterator> constexpr Iterator tr::utf8::prev(Iterator it)
{
	while ((*--it & 0xC0) == 0x80) {}
	return it;
}

template <tr::utf8::base_iterator Iterator> constexpr Iterator tr::utf8::prev(Iterator it, usize n)
{
	for (usize i = 0; i < n; ++i) {
		it = tr::utf8::prev(it);
	}
	return it;
}

template <tr::utf8::string String> constexpr String::iterator tr::utf8::insert(String& str, String::iterator where, codepoint cp)
{
	if (cp < 0x80) {
		return str.insert(where, cp);
	}
	else if (cp < 0x800) {
		const std::array<char, 2> chars{
			char(cp >> 6 | 0xC0),
			char((cp & 0x3F) | 0x80),
		};
		return str.insert(where, chars.begin(), chars.end());
	}
	else if (cp < 0x10000) {
		const std::array<char, 3> chars{
			char(cp >> 12 | 0xE0),
			char(((cp >> 6) & 0x3F) | 0x80),
			char((cp & 0x3F) | 0x80),
		};
		return str.insert(where, chars.begin(), chars.end());
	}
	else {
		const std::array<char, 4> chars{
			char(cp >> 18 | 0xF0),
			char(((cp >> 12) & 0x3F) | 0x80),
			char(((cp >> 6) & 0x3F) | 0x80),
			char((cp & 0x3F) | 0x80),
		};
		return str.insert(where, chars.begin(), chars.end());
	}
}

template <tr::utf8::string String> constexpr String::iterator tr::utf8::erase(String& str, String::iterator where)
{
	if (u8(*where) < 0x80) {
		return str.erase(where);
	}
	else if (u8(*where) < 0xE0) {
		return str.erase(where, where + 2);
	}
	else if (u8(*where) < 0xF0) {
		return str.erase(where, where + 3);
	}
	else {
		return str.erase(where, where + 4);
	}
}

template <tr::utf8::string String> constexpr void tr::utf8::pop_back(String& str)
{
	if (!str.empty()) {
		while (!str.empty() && (str.back() & 0xC0) == 0x80) {
			str.pop_back();
		}
		str.pop_back();
	}
}

///////////////////////////////////////////////////////////////// ITERATOR ////////////////////////////////////////////////////////////////

constexpr tr::utf8::iterator::iterator(const char* ptr)
	: m_ptr{ptr}
{
	TR_ASSERT(ptr != nullptr, "Tried to create UTF-8 iterator to nullptr.");
}

constexpr tr::codepoint tr::utf8::iterator::operator*() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to dereference default-constructed UTF-8 iterator.");

	return to_cp(m_ptr);
}

constexpr tr::utf8::iterator& tr::utf8::iterator::operator++()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to increment default-constructed UTF-8 iterator.");

	m_ptr = next(m_ptr);
	return *this;
}

constexpr tr::utf8::iterator tr::utf8::iterator::operator++(int)
{
	iterator prev{*this};
	++(*this);
	return prev;
}

constexpr tr::utf8::iterator& tr::utf8::iterator::operator--()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to decrement default-constructed UTF-8 iterator.");

	m_ptr = prev(m_ptr);
	return *this;
}

constexpr tr::utf8::iterator tr::utf8::iterator::operator--(int)
{
	iterator prev{*this};
	--(*this);
	return prev;
}

constexpr const char* tr::utf8::iterator::base() const
{
	return m_ptr;
}

///////////////////////////////////////////////////////////// INDEXED ITERATOR ////////////////////////////////////////////////////////////

constexpr tr::utf8::indexed_iterator::indexed_iterator(const char* ptr, ssize index)
	: m_ptr{ptr}, m_index{index}
{
	TR_ASSERT(ptr != nullptr, "Tried to create UTF-8 iterator to nullptr.");
}

constexpr auto tr::utf8::operator<=>(const indexed_iterator& l, const indexed_iterator& r)
{
	return l.m_ptr <=> r.m_ptr;
}

constexpr bool tr::utf8::operator==(const indexed_iterator& l, const indexed_iterator& r)
{
	return l.m_ptr == r.m_ptr;
}

constexpr tr::codepoint tr::utf8::indexed_iterator::operator*() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to dereference default-constructed UTF-8 iterator.");

	return to_cp(m_ptr);
}

constexpr tr::utf8::indexed_iterator& tr::utf8::indexed_iterator::operator++()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to increment default-constructed UTF-8 iterator.");

	if (++m_index > 0) {
		m_ptr = next(m_ptr);
	}
	return *this;
}

constexpr tr::utf8::indexed_iterator tr::utf8::indexed_iterator::operator++(int)
{
	indexed_iterator prev{*this};
	++(*this);
	return prev;
}

constexpr tr::utf8::indexed_iterator& tr::utf8::indexed_iterator::operator--()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to decrement default-constructed UTF-8 iterator.");

	if (--m_index >= 0) {
		m_ptr = prev(m_ptr);
	}
	return *this;
}

constexpr tr::utf8::indexed_iterator tr::utf8::indexed_iterator::operator--(int)
{
	indexed_iterator prev{*this};
	--(*this);
	return prev;
}

constexpr const char* tr::utf8::indexed_iterator::base() const
{
	return m_ptr;
}

constexpr tr::ssize tr::utf8::indexed_iterator::index() const
{
	return m_index;
}

///////////////////////////////////////////////////////////// CODEPOINT VIEWS /////////////////////////////////////////////////////////////

constexpr tr::utf8::iterator tr::utf8::begin(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return str.data();
}

constexpr tr::utf8::indexed_iterator tr::utf8::ibegin(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return {str.data(), 0};
}

constexpr tr::utf8::iterator tr::utf8::end(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return str.data() + str.size();
}

constexpr tr::utf8::indexed_iterator tr::utf8::iend(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return {str.data() + str.size(), ssize(str.size())};
}

constexpr tr::utf8::reverse_iterator tr::utf8::rbegin(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return reverse_iterator{tr::utf8::end(str)};
}

constexpr tr::utf8::reverse_indexed_iterator tr::utf8::ribegin(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return reverse_indexed_iterator{tr::utf8::iend(str)};
}

constexpr tr::utf8::reverse_iterator tr::utf8::rend(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return reverse_iterator{tr::utf8::begin(str)};
}

constexpr tr::utf8::reverse_indexed_iterator tr::utf8::riend(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return reverse_indexed_iterator{tr::utf8::ibegin(str)};
}

constexpr std::ranges::subrange<tr::utf8::iterator> tr::utf8::range(std::string_view str)
{
	return {utf8::begin(str), utf8::end(str)};
}

constexpr tr::usize tr::utf8::length(std::string_view str)
{
	return std::distance(utf8::begin(str), utf8::end(str));
}