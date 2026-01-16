///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements binary_io.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "binary_io.hpp"

/////////////////////////////////////////////////////////// BINARY IO FUNCTIONS ///////////////////////////////////////////////////////////

template <tr::binary_readable T> void tr::read_binary(std::istream& is, T& out)
{
	binary_reader<std::remove_volatile_t<T>>{}(is, out);
}

template <tr::binary_readable T, tr::usize S> void tr::read_binary(std::istream& is, std::span<T, S> out)
{
	if constexpr (requires { requires std::same_as<typename binary_reader<T>::raw_reader, std::true_type>; }) {
		is.read((char*)out.data(), out.size_bytes());
	}
	else {
		for (T& v : out) {
			read_binary(is, v);
		}
	}
}

template <tr::binary_constructible T> T tr::read_binary(std::istream& is)
{
	T out;
	read_binary(is, out);
	return out;
}

template <tr::string_literal Literal> bool tr::read_binary_magic(std::istream& is)
{
	const std::array<char, sizeof(Literal)> buffer{read_binary<std::array<char, sizeof(Literal)>>(is)};
	return std::string_view{buffer.data(), sizeof(Literal)} == Literal;
}

template <tr::binary_flushable_iterator It> void tr::flush_binary(std::istream& is, It out)
{
	while (is.peek() != EOF) {
		if constexpr (std::output_iterator<It, char>) {
			*out++ = char(is.get());
		}
		else if constexpr (std::output_iterator<It, signed char>) {
			*out++ = (signed char)(is.get());
		}
		else if constexpr (std::output_iterator<It, unsigned char>) {
			*out++ = (unsigned char)(is.get());
		}
		else {
			*out++ = std::byte(is.get());
		}
	}
}

template <tr::binary_writable T> void tr::write_binary(std::ostream& os, const T& in)
{
	binary_writer<std::remove_cv_t<T>>{}(os, in);
}

////////////////////////////////////////////////////// BINARY READER SPECIALIZATIONS //////////////////////////////////////////////////////

template <tr::standard_layout T> void tr::raw_binary_reader<T>::operator()(std::istream& is, T& out) const
{
	is.read((char*)std::addressof(out), sizeof(T));
}

template <tr::binary_readable T, tr::usize S>
void tr::binary_reader<std::array<T, S>>::operator()(std::istream& is, std::array<T, S>& out) const
{
	read_binary(is, std::span{out});
}

template <tr::binary_readable A, tr::binary_readable B>
void tr::binary_reader<std::pair<A, B>>::operator()(std::istream& is, std::pair<A, B>& out) const
{
	read_binary(is, out.first);
	read_binary(is, out.second);
}

template <tr::binary_constructible T> void tr::binary_reader<std::vector<T>>::operator()(std::istream& is, std::vector<T>& out) const
{
	out.resize(read_binary<u32>(is));
	read_binary(is, std::span{out});
}

template <tr::binary_constructible K, class... Args>
void tr::binary_reader<std::set<K, Args...>>::operator()(std::istream& is, std::set<K, Args...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		out.insert(read_binary<K>(is));
	}
}

template <tr::binary_constructible K, tr::binary_constructible V, class... Args>
void tr::binary_reader<std::map<K, V, Args...>>::operator()(std::istream& is, std::map<K, V, Args...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		K k{read_binary<K>(is)};
		V v{read_binary<V>(is)};
		out.emplace(std::move(k), std::move(v));
	}
}

template <tr::binary_constructible K, class... Args>
void tr::binary_reader<std::unordered_set<K, Args...>>::operator()(std::istream& is, std::unordered_set<K, Args...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		out.insert(read_binary<K>(is));
	}
}

template <tr::binary_constructible K, tr::binary_constructible V, class... Args>
void tr::binary_reader<std::unordered_map<K, V, Args...>>::operator()(std::istream& is, std::unordered_map<K, V, Args...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		K k{read_binary<K>(is)};
		V v{read_binary<V>(is)};
		out.emplace(std::move(k), std::move(v));
	}
}

////////////////////////////////////////////////////// BINARY WRITER SPECIALIZATIONS //////////////////////////////////////////////////////

template <tr::standard_layout T> void tr::raw_binary_writer<T>::operator()(std::ostream& os, const T& in)
{
	os.write((const char*)std::addressof(in), sizeof(T));
}

template <tr::usize S> void tr::binary_writer<char[S]>::operator()(std::ostream& os, const char (&in)[S]) const
{
	os.write(in, S - 1);
}

template <tr::binary_writable T, tr::usize S>
void tr::binary_writer<std::span<T, S>>::operator()(std::ostream& os, const std::span<T, S>& in) const
{
	if constexpr (requires { requires std::same_as<typename binary_writer<std::remove_const_t<T>>::raw_writer, std::true_type>; }) {
		os.write((const char*)in.data(), in.size_bytes());
	}
	else {
		for (const T& v : in) {
			write_binary(os, v);
		}
	}
}

template <tr::binary_writable A, tr::binary_writable B>
void tr::binary_writer<std::pair<A, B>>::operator()(std::ostream& os, const std::pair<A, B> in) const
{
	write_binary(os, in.first);
	write_binary(os, in.second);
}

template <tr::binary_writable T> void tr::binary_writer<std::vector<T>>::operator()(std::ostream& os, const std::vector<T>& in) const
{
	write_binary(os, u32(in.size()));
	write_binary(os, std::span{in});
}

template <tr::binary_writable K, class... Args>
void tr::binary_writer<std::set<K, Args...>>::operator()(std::ostream& os, const std::set<K, Args...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const auto& k : in) {
		write_binary(os, k);
	}
}

template <tr::binary_writable K, tr::binary_writable V, class... Args>
void tr::binary_writer<std::map<K, V, Args...>>::operator()(std::ostream& os, const std::map<K, V, Args...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const auto& [k, v] : in) {
		write_binary(os, k);
		write_binary(os, v);
	}
}

template <tr::binary_writable K, class... Args>
void tr::binary_writer<std::unordered_set<K, Args...>>::operator()(std::ostream& os, const std::unordered_set<K, Args...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const auto& k : in) {
		write_binary(os, k);
	}
}

template <tr::binary_writable K, tr::binary_writable V, class... Args>
void tr::binary_writer<std::unordered_map<K, V, Args...>>::operator()(std::ostream& os, const std::unordered_map<K, V, Args...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const auto& [k, v] : in) {
		write_binary(os, k);
		write_binary(os, v);
	}
}