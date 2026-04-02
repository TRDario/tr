///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements binary_io.hpp.                                                                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "../binary_io.hpp"

/////////////////////////////////////////////////////////// BINARY IO FUNCTIONS ///////////////////////////////////////////////////////////

template <tr::binary_readable Out> void tr::read_binary(std::istream& is, Out& out)
{
	binary_reader<std::remove_volatile_t<Out>>{}(is, out);
}

template <tr::binary_readable Out, tr::usize Size> void tr::read_binary(std::istream& is, std::span<Out, Size> out)
{
	if constexpr (requires { requires std::same_as<typename binary_reader<Out>::raw_reader, std::true_type>; }) {
		is.read((char*)out.data(), out.size_bytes());
	}
	else {
		for (Out& v : out) {
			read_binary(is, v);
		}
	}
}

template <tr::span_or_ref_to_binary_readable... Outs>
	requires(sizeof...(Outs) >= 2)
void tr::read_binary(std::istream& is, Outs&&... outs)
{
	(read_binary(is, outs), ...);
}

template <tr::binary_constructible Out> Out tr::read_binary(std::istream& is)
{
	Out out;
	read_binary(is, out);
	return out;
}

template <tr::binary_flushable_iterator Iterator> void tr::flush_binary(std::istream& is, Iterator out)
{
	while (is.peek() != EOF) {
		if constexpr (std::output_iterator<Iterator, char>) {
			*out++ = char(is.get());
		}
		else if constexpr (std::output_iterator<Iterator, signed char>) {
			*out++ = (signed char)(is.get());
		}
		else if constexpr (std::output_iterator<Iterator, unsigned char>) {
			*out++ = (unsigned char)(is.get());
		}
		else {
			*out++ = std::byte(is.get());
		}
	}
}

template <tr::binary_writable In> void tr::write_binary(std::ostream& os, const In& in)
{
	binary_writer<std::remove_cv_t<In>>{}(os, in);
}

template <tr::binary_writable... Ins>
	requires(sizeof...(Ins) >= 2)
void tr::write_binary(std::ostream& os, const Ins&... ins)
{
	(write_binary(os, ins), ...);
}

////////////////////////////////////////////////////// BINARY READER SPECIALIZATIONS //////////////////////////////////////////////////////

template <tr::standard_layout Out> void tr::raw_binary_reader<Out>::operator()(std::istream& is, Out& out) const
{
	is.read((char*)std::addressof(out), sizeof(Out));
}

template <tr::binary_readable Element, tr::usize Size>
void tr::binary_reader<std::array<Element, Size>>::operator()(std::istream& is, std::array<Element, Size>& out) const
{
	read_binary(is, std::span{out});
}

template <tr::binary_readable First, tr::binary_readable Second>
void tr::binary_reader<std::pair<First, Second>>::operator()(std::istream& is, std::pair<First, Second>& out) const
{
	read_binary(is, out.first, out.second);
}

template <tr::binary_constructible Element>
void tr::binary_reader<std::vector<Element>>::operator()(std::istream& is, std::vector<Element>& out) const
{
	out.resize(read_binary<u32>(is));
	read_binary(is, std::span{out});
}

template <tr::binary_constructible Key, typename... Other>
void tr::binary_reader<std::set<Key, Other...>>::operator()(std::istream& is, std::set<Key, Other...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		out.insert(read_binary<Key>(is));
	}
}

template <tr::binary_constructible Key, tr::binary_constructible Value, typename... Other>
void tr::binary_reader<std::map<Key, Value, Other...>>::operator()(std::istream& is, std::map<Key, Value, Other...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	for (u32 i = 0; i < size; ++i) {
		Key key{read_binary<Key>(is)};
		Value value{read_binary<Value>(is)};
		out.emplace(std::move(key), std::move(value));
	}
}

template <tr::binary_constructible Key, typename... Other>
void tr::binary_reader<boost::unordered_flat_set<Key, Other...>>::operator()(std::istream& is,
																			 boost::unordered_flat_set<Key, Other...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		out.insert(read_binary<Key>(is));
	}
}

template <tr::binary_constructible Key, typename... Other>
void tr::binary_reader<boost::unordered_node_set<Key, Other...>>::operator()(std::istream& is,
																			 boost::unordered_node_set<Key, Other...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		out.insert(read_binary<Key>(is));
	}
}

template <tr::binary_constructible Key, tr::binary_constructible Value, typename... Other>
void tr::binary_reader<boost::unordered_flat_map<Key, Value, Other...>>::operator()(
	std::istream& is, boost::unordered_flat_map<Key, Value, Other...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		Key key{read_binary<Key>(is)};
		Value value{read_binary<Value>(is)};
		out.emplace(std::move(key), std::move(value));
	}
}

template <tr::binary_constructible Key, tr::binary_constructible Value, typename... Other>
void tr::binary_reader<boost::unordered_node_map<Key, Value, Other...>>::operator()(
	std::istream& is, boost::unordered_node_map<Key, Value, Other...>& out) const
{
	const u32 size{read_binary<u32>(is)};
	out.clear();
	out.reserve(size);
	for (u32 i = 0; i < size; ++i) {
		Key key{read_binary<Key>(is)};
		Value value{read_binary<Value>(is)};
		out.emplace(std::move(key), std::move(value));
	}
}

////////////////////////////////////////////////////// BINARY WRITER SPECIALIZATIONS //////////////////////////////////////////////////////

template <tr::standard_layout In> void tr::raw_binary_writer<In>::operator()(std::ostream& os, const In& in)
{
	os.write((const char*)std::addressof(in), sizeof(In));
}

template <tr::binary_writable Element, tr::usize Size>
void tr::binary_writer<std::span<Element, Size>>::operator()(std::ostream& os, const std::span<Element, Size>& in) const
{
	if constexpr (requires { requires std::same_as<typename binary_writer<std::remove_const_t<Element>>::raw_writer, std::true_type>; }) {
		os.write((const char*)in.data(), in.size_bytes());
	}
	else {
		for (const Element& element : in) {
			write_binary(os, element);
		}
	}
}

template <tr::binary_writable First, tr::binary_writable Second>
void tr::binary_writer<std::pair<First, Second>>::operator()(std::ostream& os, const std::pair<First, Second> in) const
{
	write_binary(os, in.first, in.second);
}

template <tr::usize Size> void tr::binary_writer<char[Size]>::operator()(std::ostream& os, const char (&in)[Size]) const
{
	write_binary(os, std::string_view{in});
}

template <tr::binary_writable Element, tr::usize Size>
void tr::binary_writer<Element[Size]>::operator()(std::ostream& os, const Element (&in)[Size]) const
{
	write_binary(os, std::span{in});
}

template <tr::binary_writable Element, tr::usize Size>
void tr::binary_writer<std::array<Element, Size>>::operator()(std::ostream& os, const std::array<Element, Size>& in) const
{
	write_binary(os, std::span{in});
}

template <tr::binary_writable Element>
void tr::binary_writer<std::vector<Element>>::operator()(std::ostream& os, const std::vector<Element>& in) const
{
	write_binary(os, u32(in.size()), std::span{in});
}

template <tr::binary_writable Key, typename... Other>
void tr::binary_writer<std::set<Key, Other...>>::operator()(std::ostream& os, const std::set<Key, Other...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const Key& key : in) {
		write_binary(os, key);
	}
}

template <tr::binary_writable Key, tr::binary_writable Value, typename... Other>
void tr::binary_writer<std::map<Key, Value, Other...>>::operator()(std::ostream& os, const std::map<Key, Value, Other...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const auto& [key, value] : in) {
		write_binary(os, key, value);
	}
}

template <tr::binary_writable Key, typename... Other>
void tr::binary_writer<boost::unordered_flat_set<Key, Other...>>::operator()(std::ostream& os,
																			 const boost::unordered_flat_set<Key, Other...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const Key& key : in) {
		write_binary(os, key);
	}
}

template <tr::binary_writable Key, typename... Other>
void tr::binary_writer<boost::unordered_node_set<Key, Other...>>::operator()(std::ostream& os,
																			 const boost::unordered_node_set<Key, Other...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const Key& key : in) {
		write_binary(os, key);
	}
}

template <tr::binary_writable Key, tr::binary_writable Value, typename... Other>
void tr::binary_writer<boost::unordered_flat_map<Key, Value, Other...>>::operator()(
	std::ostream& os, const boost::unordered_flat_map<Key, Value, Other...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const auto& [key, value] : in) {
		write_binary(os, key, value);
	}
}

template <tr::binary_writable Key, tr::binary_writable Value, typename... Other>
void tr::binary_writer<boost::unordered_node_map<Key, Value, Other...>>::operator()(
	std::ostream& os, const boost::unordered_node_map<Key, Value, Other...>& in) const
{
	write_binary(os, u32(in.size()));
	for (const auto& [key, value] : in) {
		write_binary(os, key, value);
	}
}