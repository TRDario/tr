#pragma once
#include "binary_io.hpp"
#include "ranges.hpp"

template <tr::default_binary_readable T> void tr::default_binary_reader<T>::read_from_stream(std::istream& is, T& out)
{
	is.read((char*)std::addressof(out), sizeof(T));
}

template <tr::default_binary_readable T>
std::span<const std::byte> tr::default_binary_reader<T>::read_from_span(std::span<const std::byte> span, T& out)
{
	if (span.size() < sizeof(T)) {
		throw std::out_of_range{"Tried to binary read an object larger than the size of the input range."};
	}
	std::memcpy((char*)std::addressof(out), span.data(), sizeof(T));
	return span.subspan(sizeof(T));
}

template <class T>
void tr::binary_reader<std::span<T>>::read_from_stream(std::istream& is, const std::span<T>& out)
	requires(stream_binary_readable<T>)
{
	if constexpr (std::is_base_of_v<default_binary_reader<T>, binary_reader<T>>) {
		is.read((char*)out.data(), out.size_bytes());
	}
	else {
		for (T& v : out) {
			binary_read(is, v);
		}
	}
}

template <class T>
std::span<const std::byte> tr::binary_reader<std::span<T>>::read_from_span(std::span<const std::byte> span, const std::span<T>& out)
	requires(span_binary_readable<T>)
{
	if constexpr (requires { requires std::same_as<typename binary_reader<T>::default_reader, std::true_type>; }) {
		std::ranges::copy(as_objects<T>(span.subspan(0, out.size_bytes())), out.begin());
		return span.subspan(out.size_bytes());
	}
	else {
		for (T& v : out) {
			span = binary_read(span, v);
		}
		return span;
	}
}

template <class A, class B>
void tr::binary_reader<std::pair<A, B>>::read_from_stream(std::istream& is, std::pair<A, B>& out)
	requires(stream_binary_readable<A> && stream_binary_readable<B>)
{
	binary_read(is, out.first);
	binary_read(is, out.second);
}

template <class A, class B>
std::span<const std::byte> tr::binary_reader<std::pair<A, B>>::read_from_span(std::span<const std::byte> span, std::pair<A, B>& out)
	requires(span_binary_readable<A> && span_binary_readable<B>)
{
	span = binary_read(span, out.first);
	return binary_read(span, out.second);
}

template <tr::binary_constructible T>
void tr::binary_reader<std::vector<T>>::read_from_stream(std::istream& is, std::vector<T>& out)
	requires(stream_binary_readable<T>)
{
	out.resize(binary_read<u32>(is));
	binary_reader<std::span<T>>::read_from_stream(is, std::span{out});
}

template <tr::binary_constructible T>
std::span<const std::byte> tr::binary_reader<std::vector<T>>::read_from_span(std::span<const std::byte> span, std::vector<T>& out)
	requires(span_binary_readable<T>)
{
	out.resize(binary_read<u32>(span));
	return binary_reader<std::span<T>>::read_from_span(span, std::span{out});
}

template <tr::binary_constructible K, class... Args>
void tr::binary_reader<std::set<K, Args...>>::read_from_stream(std::istream& is, std::set<K, Args...>& out)
	requires(stream_binary_readable<K>)
{
	const u32 size{binary_read<u32>(is)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		out.insert(binary_read<K>(is));
	}
}

template <tr::binary_constructible K, class... Args>
std::span<const std::byte> tr::binary_reader<std::set<K, Args...>>::read_from_span(std::span<const std::byte> span,
																				   std::set<K, Args...>& out)
	requires(span_binary_readable<K>)
{
	const u32 size{binary_read<u32>(span)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		out.insert(binary_read<K>(span));
	}
	return span;
}

template <tr::binary_constructible K, tr::binary_constructible V, class... Args>
void tr::binary_reader<std::map<K, V, Args...>>::read_from_stream(std::istream& is, std::map<K, V, Args...>& out)
	requires(stream_binary_readable<K> && stream_binary_readable<V>)
{
	const u32 size{binary_read<u32>(is)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		K k{binary_read<K>(is)};
		V v{binary_read<V>(is)};
		out.emplace(std::move(k), std::move(v));
	}
}

template <tr::binary_constructible K, tr::binary_constructible V, class... Args>
std::span<const std::byte> tr::binary_reader<std::map<K, V, Args...>>::read_from_span(std::span<const std::byte> span,
																					  std::map<K, V, Args...>& out)
	requires(span_binary_readable<K> && span_binary_readable<V>)
{
	const u32 size{binary_read<u32>(span)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		K k{binary_read<K>(span)};
		V v{binary_read<V>(span)};
		out.emplace(std::move(k), std::move(v));
	}
	return span;
}

template <tr::binary_constructible K, class... Args>
void tr::binary_reader<std::unordered_set<K, Args...>>::read_from_stream(std::istream& is, std::unordered_set<K, Args...>& out)
	requires(stream_binary_readable<K>)
{
	const u32 size{binary_read<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		out.insert(binary_read<K>(is));
	}
}

template <tr::binary_constructible K, class... Args>
std::span<const std::byte> tr::binary_reader<std::unordered_set<K, Args...>>::read_from_span(std::span<const std::byte> span,
																							 std::unordered_set<K, Args...>& out)
	requires(span_binary_readable<K>)
{
	const u32 size{binary_read<u32>(span)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		out.insert(binary_read<K>(span));
	}
	return span;
}

template <tr::binary_constructible K, tr::binary_constructible V, class... Args>
void tr::binary_reader<std::unordered_map<K, V, Args...>>::read_from_stream(std::istream& is, std::unordered_map<K, V, Args...>& out)
	requires(stream_binary_readable<K> && stream_binary_readable<V>)
{
	const u32 size{binary_read<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		K k{binary_read<K>(is)};
		V v{binary_read<V>(is)};
		out.emplace(std::move(k), std::move(v));
	}
}

template <tr::binary_constructible K, tr::binary_constructible V, class... Args>
std::span<const std::byte> tr::binary_reader<std::unordered_map<K, V, Args...>>::read_from_span(std::span<const std::byte> span,
																								std::unordered_map<K, V, Args...>& out)
	requires(span_binary_readable<K> && span_binary_readable<V>)
{
	const u32 size{binary_read<u32>(span)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		K k{binary_read<K>(span)};
		V v{binary_read<V>(span)};
		out.emplace(std::move(k), std::move(v));
	}
	return span;
}

template <tr::stream_binary_readable T> void tr::binary_read(std::istream& is, T& out)
{
	binary_reader<T>::read_from_stream(is, out);
}

template <tr::stream_binary_constructible T> T tr::binary_read(std::istream& is)
{
	T out;
	binary_reader<T>::read_from_stream(is, out);
	return out;
}

template <tr::span_binary_readable T> std::span<const std::byte> tr::binary_read(std::span<const std::byte> span, T& out)
{
	return binary_reader<T>::read_from_span(span, out);
}

template <tr::span_binary_readable T, std::ranges::contiguous_range Range> void tr::binary_read(Range&& range, T& out)
{
	binary_reader<T>::read_from_span(std::span<const std::byte>{range_bytes(range)}, out);
}

template <tr::span_binary_constructible T> T tr::binary_read(std::span<const std::byte>& span)
{
	T out;
	span = binary_reader<T>::read_from_span(span, out);
	return out;
}

template <tr::span_binary_constructible T, std::ranges::contiguous_range Range> T tr::binary_read(Range&& range)
{
	T out;
	binary_reader<T>::read_from_span(std::span<const std::byte>{range_bytes(range)}, out);
	return out;
}

template <tr::template_string_literal Literal> bool tr::binary_read_magic(std::istream& is)
{
	return std::string_view{binary_read<std::array<char, sizeof(Literal)>>(is).data(), sizeof(Literal)} ==
		   std::string_view{Literal.data, sizeof(Literal)};
}

template <tr::template_string_literal Literal> bool tr::binary_read_magic(std::span<const std::byte>& span)
{
	return std::string_view{binary_read<std::array<char, sizeof(Literal)>>(span).data(), sizeof(Literal)} ==
		   std::string_view{Literal.data, sizeof(Literal)};
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

//

template <tr::default_binary_writable T> void tr::default_binary_writer<T>::write_to_stream(std::ostream& os, const T& in)
{
	os.write((const char*)std::addressof(in), sizeof(T));
}

template <tr::default_binary_writable T>
std::span<std::byte> tr::default_binary_writer<T>::write_to_span(std::span<std::byte> span, const T& in)
{
	if (span.size() < sizeof(T)) {
		throw std::out_of_range{"Tried to binary write an object larger than the size of the output range."};
	}
	std::ranges::copy(as_bytes(in), span.begin());
	return span.subspan(sizeof(T));
}

template <tr::usize S> void tr::binary_writer<char[S]>::write_to_stream(std::ostream& os, const char (&in)[S])
{
	os.write(in, S - 1);
}

template <tr::usize S> std::span<std::byte> tr::binary_writer<char[S]>::write_to_span(std::span<std::byte> span, const char (&in)[S])
{
	if (span.size() < S - 1) {
		throw std::out_of_range{"Tried to binary write an object larger than the size of the output range."};
	}
	std::ranges::copy(as_bytes(in) | std::views::take(S - 1), span.begin());
	return span.subspan(S - 1);
}

template <class T>
void tr::binary_writer<std::span<T>>::write_to_stream(std::ostream& os, const std::span<T>& in)
	requires(stream_binary_writable<T>)
{
	if constexpr (std::is_base_of_v<default_binary_writer<std::remove_cvref_t<T>>, binary_writer<std::remove_cvref_t<T>>>) {
		os.write((const char*)in.data(), in.size_bytes());
	}
	else {
		for (const T& v : in) {
			binary_write(os, v);
		}
	}
}

template <class T>
std::span<std::byte> tr::binary_writer<std::span<T>>::write_to_span(std::span<std::byte> span, const std::span<T>& in)
	requires(span_binary_writable<T>)
{
	if constexpr (std::is_base_of_v<default_binary_writer<std::remove_cvref_t<T>>, binary_writer<std::remove_cvref_t<T>>>) {
		std::ranges::copy(range_bytes(in), span.begin());
		return span.subspan(in.size_bytes());
	}
	else {
		for (const T& v : in) {
			span = binary_write(span, v);
		}
		return span;
	}
}

template <class A, class B>
void tr::binary_writer<std::pair<A, B>>::write_to_stream(std::ostream& os, const std::pair<A, B> in)
	requires(stream_binary_writable<A> && stream_binary_writable<B>)
{
	binary_write(os, in.first);
	binary_write(os, in.second);
}

template <class A, class B>
std::span<std::byte> tr::binary_writer<std::pair<A, B>>::write_to_span(std::span<std::byte> span, const std::pair<A, B>& in)
	requires(span_binary_writable<A> && span_binary_writable<B>)
{
	span = binary_write(span, in.first);
	return binary_write(span, in.second);
}

template <class T>
void tr::binary_writer<std::vector<T>>::write_to_stream(std::ostream& os, const std::vector<T>& in)
	requires(stream_binary_writable<T>)
{
	binary_write(os, u32(in.size()));
	binary_writer<std::span<const T>>::write_to_stream(os, std::span{in});
}

template <class T>
std::span<std::byte> tr::binary_writer<std::vector<T>>::write_to_span(std::span<std::byte> span, const std::vector<T>& in)
	requires(span_binary_writable<T>)
{
	span = binary_write(span, u32(in.size()));
	return binary_writer<std::span<const T>>::write_to_span(span, std::span{in});
}

template <class K, class... Args>
void tr::binary_writer<std::set<K, Args...>>::write_to_stream(std::ostream& os, const std::set<K, Args...>& in)
	requires(stream_binary_writable<K>)
{
	binary_write(os, u32(in.size()));
	for (const auto& k : in) {
		binary_write(os, k);
	}
}

template <class K, class... Args>
std::span<std::byte> tr::binary_writer<std::set<K, Args...>>::write_to_span(std::span<std::byte> span, const std::set<K, Args...>& in)
	requires(span_binary_writable<K>)
{
	span = binary_write(span, u32(in.size()));
	for (const auto& k : in) {
		span = binary_write(span, k);
	}
	return span;
}

template <class K, class V, class... Args>
void tr::binary_writer<std::map<K, V, Args...>>::write_to_stream(std::ostream& os, const std::map<K, V, Args...>& in)
	requires(stream_binary_writable<K> && stream_binary_writable<V>)
{
	binary_write(os, u32(in.size()));
	for (const auto& [k, v] : in) {
		binary_write(os, k);
		binary_write(os, v);
	}
}

template <class K, class V, class... Args>
std::span<std::byte> tr::binary_writer<std::map<K, V, Args...>>::write_to_span(std::span<std::byte> span, const std::map<K, V, Args...>& in)
	requires(span_binary_writable<K> && span_binary_writable<V>)
{
	span = binary_write(span, u32(in.size()));
	for (const auto& [k, v] : in) {
		span = binary_write(span, k);
		span = binary_write(span, v);
	}
	return span;
}

template <class K, class... Args>
void tr::binary_writer<std::unordered_set<K, Args...>>::write_to_stream(std::ostream& os, const std::unordered_set<K, Args...>& in)
	requires(stream_binary_writable<K>)
{
	binary_write(os, u32(in.size()));
	for (const auto& k : in) {
		binary_write(os, k);
	}
}

template <class K, class... Args>
std::span<std::byte> tr::binary_writer<std::unordered_set<K, Args...>>::write_to_span(std::span<std::byte> span,
																					  const std::unordered_set<K, Args...>& in)
	requires(span_binary_writable<K>)
{
	span = binary_write(span, u32(in.size()));
	for (const auto& k : in) {
		span = binary_write(span, k);
	}
	return span;
}

template <class K, class V, class... Args>
void tr::binary_writer<std::unordered_map<K, V, Args...>>::write_to_stream(std::ostream& os, const std::unordered_map<K, V, Args...>& in)
	requires(stream_binary_writable<K> && stream_binary_writable<V>)
{
	binary_write(os, u32(in.size()));
	for (const auto& [k, v] : in) {
		binary_write(os, k);
		binary_write(os, v);
	}
}

template <class K, class V, class... Args>
std::span<std::byte> tr::binary_writer<std::unordered_map<K, V, Args...>>::write_to_span(std::span<std::byte> span,
																						 const std::unordered_map<K, V, Args...>& in)
	requires(span_binary_writable<K> && span_binary_writable<V>)
{
	span = binary_write(span, u32(in.size()));
	for (const auto& [k, v] : in) {
		span = binary_write(span, k);
		span = binary_write(span, v);
	}
	return span;
}

template <tr::stream_binary_writable T> void tr::binary_write(std::ostream& os, const T& in)
{
	binary_writer<std::remove_cvref_t<T>>::write_to_stream(os, in);
}

template <tr::span_binary_writable T> std::span<std::byte> tr::binary_write(std::span<std::byte> span, const T& in)
{
	return binary_writer<std::remove_cvref_t<T>>::write_to_span(span, in);
}