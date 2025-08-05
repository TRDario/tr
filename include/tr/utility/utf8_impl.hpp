#pragma once
#include "utf8.hpp"

template <tr::utf8::base_iterator T> constexpr tr::codepoint tr::utf8::to_cp(T it)
{
	if (static_cast<unsigned char>(*it) < 0x80) {
		return static_cast<unsigned char>(*it);
	}
	else if (static_cast<unsigned char>(*it) < 0xE0) {
		return ((static_cast<unsigned char>(it[0]) & 0x1F) << 6) | (static_cast<unsigned char>(it[1]) & 0x3F);
	}
	else if (static_cast<unsigned char>(*it) < 0xF0) {
		return ((static_cast<unsigned char>(it[0]) & 0xF) << 12) | ((static_cast<unsigned char>(it[1]) & 0x3F) << 6) |
			   (static_cast<unsigned char>(it[2]) & 0x3F);
	}
	else {
		return ((static_cast<unsigned char>(it[0]) & 0x7) << 18) | ((static_cast<unsigned char>(it[1]) & 0x3F) << 12) |
			   ((static_cast<unsigned char>(it[2]) & 0x3F) << 6) | (static_cast<unsigned char>(it[3]) & 0x3F);
	}
}

template <tr::utf8::base_iterator T> constexpr T tr::utf8::next(T it)
{
	if (static_cast<std::uint8_t>(*it) < 0x80) {
		it += 1;
	}
	else if (static_cast<std::uint8_t>(*it) < 0xE0) {
		it += 2;
	}
	else if (static_cast<std::uint8_t>(*it) < 0xF0) {
		it += 3;
	}
	else {
		it += 4;
	}
	return it;
}

template <tr::utf8::base_iterator T> constexpr T tr::utf8::next(T it, std::size_t n)
{
	for (std::size_t i = 0; i < n; ++i) {
		it = tr::utf8::next(it);
	}
	return it;
}

template <tr::utf8::base_iterator T> constexpr T tr::utf8::prev(T it)
{
	while ((*--it & 0xC0) == 0x80) {}
	return it;
}

template <tr::utf8::base_iterator T> constexpr T tr::utf8::prev(T it, std::size_t n)
{
	for (std::size_t i = 0; i < n; ++i) {
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
		const std::array<char, 2> chars{static_cast<char>(cp >> 6), static_cast<char>(cp & 0x3F)};
		return str.insert(where, chars.begin(), chars.end());
	}
	else if (cp < 0x10000) {
		const std::array<char, 3> chars{static_cast<char>(cp >> 12), static_cast<char>((cp >> 6) & 0x3F), static_cast<char>(cp & 0x3F)};
		return str.insert(where, chars.begin(), chars.end());
	}
	else {
		const std::array<char, 4> chars{static_cast<char>(cp >> 18), static_cast<char>((cp >> 12) & 0x3F),
										static_cast<char>((cp >> 6) & 0x3F), static_cast<char>(cp & 0x3F)};
		return str.insert(where, chars.begin(), chars.end());
	}
}

template <tr::utf8::string String> constexpr String::iterator tr::utf8::erase(String& str, String::iterator where)
{
	if (static_cast<unsigned char>(*where) < 0x80) {
		return str.erase(where);
	}
	else if (static_cast<unsigned char>(*where) < 0xE0) {
		return str.erase(where, where + 2);
	}
	else if (static_cast<unsigned char>(*where) < 0xF0) {
		return str.erase(where, where + 3);
	}
	else {
		return str.erase(where, where + 4);
	}
}

template <tr::utf8::string String> constexpr void tr::utf8::pop_back(String& str)
{
	do {
		str.pop_back();
	} while (!str.empty() && (str.back() & 0xC0) == 0x80);
}

//

constexpr tr::utf8::const_it::const_it(const char* ptr)
	: m_ptr{ptr}
{
	TR_ASSERT(ptr != nullptr, "Tried to create UTF-8 iterator to nullptr.");
}

constexpr tr::codepoint tr::utf8::const_it::operator*() const
{
	TR_ASSERT(m_ptr != nullptr, "Tried to dereference default-constructed UTF-8 iterator.");

	return to_cp(m_ptr);
}

constexpr tr::utf8::const_it& tr::utf8::const_it::operator++()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to increment default-constructed UTF-8 iterator.");

	m_ptr = next(m_ptr);
	return *this;
}

constexpr tr::utf8::const_it tr::utf8::const_it::operator++(int)
{
	const_it prev{*this};
	++(*this);
	return prev;
}

constexpr tr::utf8::const_it& tr::utf8::const_it::operator--()
{
	TR_ASSERT(m_ptr != nullptr, "Tried to decrement default-constructed UTF-8 iterator.");

	m_ptr = prev(m_ptr);
	return *this;
}

constexpr tr::utf8::const_it tr::utf8::const_it::operator--(int)
{
	auto prev{*this};
	--(*this);
	return prev;
}

constexpr const char* tr::utf8::const_it::base() const
{
	return m_ptr;
}

//

constexpr tr::utf8::const_it tr::utf8::begin(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return str.data();
}

constexpr tr::utf8::const_it tr::utf8::end(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return str.data() + str.size();
}

constexpr tr::utf8::const_reverse_it tr::utf8::rbegin(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return const_reverse_it{tr::utf8::end(str)};
}

constexpr tr::utf8::const_reverse_it tr::utf8::rend(std::string_view str)
{
	if (str.empty()) {
		return {};
	}
	return const_reverse_it{tr::utf8::begin(str)};
}

constexpr std::ranges::subrange<tr::utf8::const_it> tr::utf8::range(std::string_view str)
{
	return {utf8::begin(str), utf8::end(str)};
}

constexpr std::size_t tr::utf8::length(std::string_view str)
{
	return std::distance(utf8::begin(str), utf8::end(str));
}