#pragma once
#include "concepts.hpp"

namespace tr {
	// Interface for custom readers for use in binary_read.
	template <cv_unqualified_object T> struct binary_reader;
	// Interface for custom writers for use in binary_write.
	template <cv_unqualified_object T> struct binary_writer;

	// Concept that denotes a type able to be read with binary_read.
	template <class T>
	concept binary_readable = requires(std::istream& is, T& out) { tr::binary_reader<std::remove_volatile_t<T>>{}(is, out); };
	// Concept that denotes a type able to be constructed with binary_read.
	template <class T>
	concept binary_constructible = binary_readable<T> && std::default_initializable<T>;
	// Concept that denotes a flush_binary-compatible iterator.
	template <class T>
	concept binary_flushable_iterator = std::output_iterator<T, char> || std::output_iterator<T, signed char> ||
										std::output_iterator<T, unsigned char> || std::output_iterator<T, std::byte>;
	// Concept that denotes a type able to be read with a stream binary_write.
	template <class T>
	concept binary_writable = requires(std::ostream& os, const T& in) { tr::binary_writer<std::remove_cv_t<T>>{}(os, in); };

	// Raw binary reader template: directly reads bytes into the object.
	template <standard_layout T> struct raw_binary_reader {
		using raw_reader = std::true_type;
		void operator()(std::istream& is, T& out) const;
	};
	// Raw binary writer template: directly writes bytes from the object.
	template <standard_layout T> struct raw_binary_writer {
		using raw_writer = std::true_type;
		void operator()(std::ostream& os, const T& in);
	};

	// Does a binary read from a stream.
	template <binary_readable T> void binary_read(std::istream& is, T& out);
	// Does a binary read from a stream.
	template <binary_readable T, usize S> void binary_read(std::istream& is, std::span<T, S> out);
	// Does a binary read from a stream.
	template <binary_constructible T> T binary_read(std::istream& is);
	// Checks for magic bytes from a stream.
	template <template_string_literal Literal> bool binary_read_magic(std::istream& is);

	// Flushes the rest of the stream into an output iterator.
	template <tr::binary_flushable_iterator It> void flush_binary(std::istream& is, It out);
	// Flushes the rest of the stream into a vector of bytes.
	std::vector<std::byte> flush_binary(std::istream& is);

	// Does a binary write to a stream.
	template <binary_writable T> void binary_write(std::ostream& os, const T& in);
} // namespace tr

///////////////////////////////////////////////////////////// SPECIALIZATIONS /////////////////////////////////////////////////////////////

namespace tr {
	// Arithmetic binary readers.
	template <arithmetic T> struct binary_reader<T> : raw_binary_reader<T> {};
	// Enumerator binary readers.
	template <enumerator T> struct binary_reader<T> : raw_binary_reader<T> {};
	// Vector binary readers.
	template <int S, class T> struct binary_reader<glm::vec<S, T>> : raw_binary_reader<glm::vec<S, T>> {};
	// Matrix binary readers.
	template <int C, int R, class T> struct binary_reader<glm::mat<C, R, T>> : raw_binary_reader<glm::mat<C, R, T>> {};

	// Array binary reader.
	template <binary_readable T, usize S> struct binary_reader<std::array<T, S>> {
		void operator()(std::istream& is, std::array<T, S>& out) const;
	};
	// Pair binary reader.
	template <binary_readable A, binary_readable B> struct binary_reader<std::pair<A, B>> {
		void operator()(std::istream& is, std::pair<A, B>& out) const;
	};
	// String binary reader.
	template <> struct binary_reader<std::string> {
		void operator()(std::istream& is, std::string& out) const;
	};
	// Vector binary reader.
	template <binary_constructible T> struct binary_reader<std::vector<T>> {
		void operator()(std::istream& is, std::vector<T>& out) const;
	};
	// Set binary reader.
	template <binary_constructible K, class... Args> struct binary_reader<std::set<K, Args...>> {
		void operator()(std::istream& is, std::set<K, Args...>& out) const;
	};
	// Map binary reader.
	template <binary_constructible K, binary_constructible V, class... Args> struct binary_reader<std::map<K, V, Args...>> {
		void operator()(std::istream& is, std::map<K, V, Args...>& out) const;
	};
	// Unordered set binary reader.
	template <binary_constructible K, class... Args> struct binary_reader<std::unordered_set<K, Args...>> {
		void operator()(std::istream& is, std::unordered_set<K, Args...>& out) const;
	};
	// Hashmap binary reader.
	template <binary_constructible K, binary_constructible V, class... Args> struct binary_reader<std::unordered_map<K, V, Args...>> {
		void operator()(std::istream& is, std::unordered_map<K, V, Args...>& out) const;
	};

	// Arithmetic binary writers.
	template <arithmetic T> struct binary_writer<T> : raw_binary_writer<T> {};
	// Enumerator binary writers.
	template <enumerator T> struct binary_writer<T> : raw_binary_writer<T> {};
	// Vector binary writers.
	template <int S, class T> struct binary_writer<glm::vec<S, T>> : raw_binary_writer<glm::vec<S, T>> {};
	// Matrix binary writers.
	template <int C, int R, class T> struct binary_writer<glm::mat<C, R, T>> : raw_binary_writer<glm::mat<C, R, T>> {};
	// String literal binary writer.
	template <usize S> struct binary_writer<char[S]> {
		void operator()(std::ostream& os, const char (&in)[S]) const;
	};
	// Unformatted span binary writer.
	template <> struct binary_writer<std::span<const std::byte>> {
		void operator()(std::ostream& os, const std::span<const std::byte>& in) const;
	};
	// Unformatted span binary writer.
	template <> struct binary_writer<std::span<std::byte>> : binary_writer<std::span<const std::byte>> {};
	// Span binary writer.
	template <binary_writable T, usize S> struct binary_writer<std::span<T, S>> {
		void operator()(std::ostream& os, const std::span<T, S>& in) const;
	};
	// Pair binary writer.
	template <binary_writable A, binary_writable B> struct binary_writer<std::pair<A, B>> {
		void operator()(std::ostream& os, const std::pair<A, B> in) const;
	};
	// String view binary writer.
	template <> struct binary_writer<std::string_view> {
		void operator()(std::ostream& os, const std::string_view& in) const;
	};
	// String binary writer.
	template <> struct binary_writer<std::string> {
		void operator()(std::ostream& os, const std::string& in) const;
	};
	// Vector binary writer.
	template <binary_writable T> struct binary_writer<std::vector<T>> {
		void operator()(std::ostream& os, const std::vector<T>& in) const;
	};
	// Set binary writer.
	template <binary_writable K, class... Args> struct binary_writer<std::set<K, Args...>> {
		void operator()(std::ostream& os, const std::set<K, Args...>& in) const;
	};
	// Map binary writer.
	template <binary_writable K, binary_writable V, class... Args> struct binary_writer<std::map<K, V, Args...>> {
		void operator()(std::ostream& os, const std::map<K, V, Args...>& in) const;
	};
	// Unordered set binary writer.
	template <binary_writable K, class... Args> struct binary_writer<std::unordered_set<K, Args...>> {
		void operator()(std::ostream& os, const std::unordered_set<K, Args...>& in) const;
	};
	// Hashmap binary writer.
	template <binary_writable K, binary_writable V, class... Args> struct binary_writer<std::unordered_map<K, V, Args...>> {
		void operator()(std::ostream& os, const std::unordered_map<K, V, Args...>& in) const;
	};
} // namespace tr

#include "binary_io_impl.hpp" // IWYU pragma: keep