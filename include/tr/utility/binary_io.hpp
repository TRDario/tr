#pragma once
#include "concepts.hpp"

namespace tr {
	// Interface for custom readers for use in binary_read.
	template <class T> struct binary_reader {
		static_assert(ALWAYS_FALSE<T>, "binary_reader is not specialized for this type!");
	};

	// Concept that denotes a type able to use the default binary reader.
	template <class T>
	concept default_binary_readable = std::is_standard_layout_v<T> && std::same_as<T, std::remove_cvref_t<T>>;
	// Default binary reader.
	template <default_binary_readable T> struct default_binary_reader {
		using default_reader = std::true_type;

		static void read_from_stream(std::istream& is, T& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, T& out);
	};

	// Concept that denotes a type able to be read with a stream binary_read.
	template <class T>
	concept stream_binary_readable = std::invocable<decltype(binary_reader<T>::read_from_stream), std::istream&, T&>;
	// Concept that denotes a type able to be read with a span binary_read.
	template <class T>
	concept span_binary_readable = std::invocable<decltype(binary_reader<T>::read_from_span), std::span<const std::byte>, T&> &&
								   std::same_as<return_type_t<decltype(binary_reader<T>::read_from_span)>, std::span<const std::byte>>;
	// Concept that denotes a type able to be constructed with binary_read.
	template <class T>
	concept binary_constructible = std::default_initializable<T>;
	// Concept that denotes a type able to be constructed with a stream binary_read.
	template <class T>
	concept stream_binary_constructible = stream_binary_readable<T> && binary_constructible<T>;
	// Concept that denotes a type able to be constructed with a span binary_read.
	template <class T>
	concept span_binary_constructible = span_binary_readable<T> && binary_constructible<T>;

	// Arithmetic binary readers.
	template <arithmetic T> struct binary_reader<T> : default_binary_reader<T> {};
	// Enumerator binary readers.
	template <enumerator T> struct binary_reader<T> : default_binary_reader<T> {};
	// Vector binary readers.
	template <int S, class T> struct binary_reader<glm::vec<S, T>> : default_binary_reader<glm::vec<S, T>> {};
	// Matrix binary readers.
	template <int C, int R, class T> struct binary_reader<glm::mat<C, R, T>> : default_binary_reader<glm::mat<C, R, T>> {};

	// Span binary reader.
	template <class T> struct binary_reader<std::span<T>> {
		static void read_from_stream(std::istream& is, const std::span<T>& out)
			requires(stream_binary_readable<T>);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, const std::span<T>& out)
			requires(span_binary_readable<T>);
	};
	// Array binary reader.
	template <class T, usize S> struct binary_reader<std::array<T, S>> : binary_reader<std::span<T>> {};
	// Pair binary reader.
	template <class A, class B> struct binary_reader<std::pair<A, B>> {
		static void read_from_stream(std::istream& is, std::pair<A, B>& out)
			requires(stream_binary_readable<A> && stream_binary_readable<B>);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::pair<A, B>& out)
			requires(span_binary_readable<A> && span_binary_readable<B>);
	};
	// String binary reader.
	template <> struct binary_reader<std::string> {
		static void read_from_stream(std::istream& is, std::string& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::string& out);
	};
	// Vector binary reader.
	template <binary_constructible T> struct binary_reader<std::vector<T>> {
		static void read_from_stream(std::istream& is, std::vector<T>& out)
			requires(stream_binary_readable<T>);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::vector<T>& out)
			requires(span_binary_readable<T>);
	};
	// Set binary reader.
	template <binary_constructible K, class... Args> struct binary_reader<std::set<K, Args...>> {
		static void read_from_stream(std::istream& is, std::set<K, Args...>& out)
			requires(stream_binary_readable<K>);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::set<K, Args...>& out)
			requires(span_binary_readable<K>);
	};
	// Map binary reader.
	template <binary_constructible K, binary_constructible V, class... Args> struct binary_reader<std::map<K, V, Args...>> {
		static void read_from_stream(std::istream& is, std::map<K, V, Args...>& out)
			requires(stream_binary_readable<K> && stream_binary_readable<V>);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::map<K, V, Args...>& out)
			requires(span_binary_readable<K> && span_binary_readable<V>);
	};
	// Unordered set binary reader.
	template <binary_constructible K, class... Args> struct binary_reader<std::unordered_set<K, Args...>> {
		static void read_from_stream(std::istream& is, std::unordered_set<K, Args...>& out)
			requires(stream_binary_readable<K>);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::unordered_set<K, Args...>& out)
			requires(span_binary_readable<K>);
	};
	// Hashmap binary reader.
	template <binary_constructible K, binary_constructible V, class... Args> struct binary_reader<std::unordered_map<K, V, Args...>> {
		static void read_from_stream(std::istream& is, std::unordered_map<K, V, Args...>& out)
			requires(stream_binary_readable<K> && stream_binary_readable<V>);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::unordered_map<K, V, Args...>& out)
			requires(span_binary_readable<K> && span_binary_readable<V>);
	};

	// Does a binary read from a stream.
	template <stream_binary_readable T> void binary_read(std::istream& is, T& out);
	// Does a binary read from a stream.
	template <stream_binary_constructible T> T binary_read(std::istream& is);
	// Does a binary read from a span.
	template <span_binary_readable T> std::span<const std::byte> binary_read(std::span<const std::byte> span, T& out);
	// Does a binary read from a range.
	template <span_binary_readable T, std::ranges::contiguous_range Range> void binary_read(Range&& range, T& out);
	// Does a binary read from a span.
	template <span_binary_constructible T> T binary_read(std::span<const std::byte>& span);
	// Does a binary read from a range.
	template <span_binary_constructible T, std::ranges::contiguous_range Range> T binary_read(Range&& range);
	// Checks for magic bytes from a stream.
	template <template_string_literal Literal> bool binary_read_magic(std::istream& is);
	// Checks for magic bytes from a span.
	template <template_string_literal Literal> bool binary_read_magic(std::span<const std::byte>& span);

	// Concept that denotes a flush_binary-compatible iterator.
	template <class T>
	concept binary_flushable_iterator = std::output_iterator<T, char> || std::output_iterator<T, signed char> ||
										std::output_iterator<T, unsigned char> || std::output_iterator<T, std::byte>;
	// Flushes the rest of the stream into an output iterator.
	template <tr::binary_flushable_iterator It> void flush_binary(std::istream& is, It out);
	// Flushes the rest of the stream into a vector of bytes.
	std::vector<std::byte> flush_binary(std::istream& is);

	// Interface for custom writers for use in binary_write.
	template <class T> struct binary_writer {
		static_assert(ALWAYS_FALSE<T>, "binary_writer is not specialized for this type!");
	};

	// Concept that denotes a type able to use the default binary writer.
	template <class T>
	concept default_binary_writable = std::is_standard_layout_v<T>;
	// Default binary writer.
	template <default_binary_writable T> struct default_binary_writer {
		using default_writer = std::true_type;

		static void write_to_stream(std::ostream& os, const T& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const T& in);
	};

	// Concept that denotes a type able to be read with a stream binary_read.
	template <class T>
	concept stream_binary_writable =
		std::invocable<decltype(binary_writer<std::remove_cvref_t<T>>::write_to_stream), std::ostream&, const T&>;
	// Concept that denotes a type able to be read with a span binary_read.
	template <class T>
	concept span_binary_writable =
		std::invocable<decltype(binary_writer<std::remove_cvref_t<T>>::write_to_span), std::span<std::byte>, const T&> &&
		std::same_as<return_type_t<decltype(binary_writer<std::remove_cvref_t<T>>::write_to_span)>, std::span<std::byte>>;

	// Arithmetic binary writers.
	template <arithmetic T> struct binary_writer<T> : default_binary_writer<T> {};
	// Enumerator binary writers.
	template <enumerator T> struct binary_writer<T> : default_binary_writer<T> {};
	// Vector binary writers.
	template <int S, class T> struct binary_writer<glm::vec<S, T>> : default_binary_writer<glm::vec<S, T>> {};
	// Matrix binary writers.
	template <int C, int R, class T> struct binary_writer<glm::mat<C, R, T>> : default_binary_writer<glm::mat<C, R, T>> {};
	// String literal binary writer.
	template <usize S> struct binary_writer<char[S]> {
		static void write_to_stream(std::ostream& os, const char (&in)[S]);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const char (&in)[S]);
	};
	// Unformatted span binary writer.
	template <> struct binary_writer<std::span<const std::byte>> {
		static void write_to_stream(std::ostream& os, const std::span<const std::byte>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::span<const std::byte>& in);
	};
	// Unformatted span binary writer.
	template <> struct binary_writer<std::span<std::byte>> : binary_writer<std::span<const std::byte>> {};
	// Span binary writer.
	template <class T> struct binary_writer<std::span<T>> {
		static void write_to_stream(std::ostream& os, const std::span<T>& in)
			requires(stream_binary_writable<T>);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::span<T>& in)
			requires(span_binary_writable<T>);
	};
	// Pair binary writer.
	template <class A, class B> struct binary_writer<std::pair<A, B>> {
		static void write_to_stream(std::ostream& os, const std::pair<A, B> in)
			requires(stream_binary_writable<A> && stream_binary_writable<B>);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::pair<A, B>& in)
			requires(span_binary_writable<A> && span_binary_writable<B>);
	};
	// String view binary writer.
	template <> struct binary_writer<std::string_view> {
		static void write_to_stream(std::ostream& os, const std::string_view& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::string_view& in);
	};
	// String binary writer.
	template <> struct binary_writer<std::string> {
		static void write_to_stream(std::ostream& os, const std::string& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::string& in);
	};
	// Vector binary writer.
	template <class T> struct binary_writer<std::vector<T>> {
		static void write_to_stream(std::ostream& os, const std::vector<T>& in)
			requires(stream_binary_writable<T>);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::vector<T>& in)
			requires(span_binary_writable<T>);
	};
	// Set binary writer.
	template <class K, class... Args> struct binary_writer<std::set<K, Args...>> {
		static void write_to_stream(std::ostream& os, const std::set<K, Args...>& in)
			requires(stream_binary_writable<K>);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::set<K, Args...>& in)
			requires(span_binary_writable<K>);
	};
	// Map binary writer.
	template <class K, class V, class... Args> struct binary_writer<std::map<K, V, Args...>> {
		static void write_to_stream(std::ostream& os, const std::map<K, V, Args...>& in)
			requires(stream_binary_writable<K> && stream_binary_writable<V>);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::map<K, V, Args...>& in)
			requires(span_binary_writable<K> && span_binary_writable<V>);
	};
	// Unordered set binary writer.
	template <class K, class... Args> struct binary_writer<std::unordered_set<K, Args...>> {
		static void write_to_stream(std::ostream& os, const std::unordered_set<K, Args...>& in)
			requires(stream_binary_writable<K>);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::unordered_set<K, Args...>& in)
			requires(span_binary_writable<K>);
	};
	// Hashmap binary writer.
	template <class K, class V, class... Args> struct binary_writer<std::unordered_map<K, V, Args...>> {
		static void write_to_stream(std::ostream& os, const std::unordered_map<K, V, Args...>& in)
			requires(stream_binary_writable<K> && stream_binary_writable<V>);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::unordered_map<K, V, Args...>& in)
			requires(span_binary_writable<K> && span_binary_writable<V>);
	};

	// Does a binary write to a stream.
	template <stream_binary_writable T> void binary_write(std::ostream& os, const T& in);
	// Does a binary write to a span.
	template <span_binary_writable T> std::span<std::byte> binary_write(std::span<std::byte> span, const T& in);
} // namespace tr

#include "binary_io_impl.hpp"