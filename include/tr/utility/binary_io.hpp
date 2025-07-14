#pragma once
#include "concepts.hpp"

namespace tr {
	/******************************************************************************************************************
	 * Interface for custom readers for use in binary_read.
	 *
	 * To fulfill the requirements for a valid binary reader specialization, the following must be present:
	 *
	 * static function read_from_stream that takes std::istream& and T&.
	 *
	 * static function read_from_span that takes a byte span and T& and returns another byte span with the read portion
	 * cut off.
	 *
	 * If these conditions are properly met, @em T will fulfill binary_readable.
	 *****************************************************************************************************************/
	template <class T> struct binary_reader {};

	/******************************************************************************************************************
	 * Concept that denotes a type able to be read with binary_read.
	 *
	 * To fulfill this condition, binary_reader<T> must have the static function @em read_from_stream and @em
	 * read_from_span fulfilling the conditions detailed in the description for @em binary_reader.
	 *****************************************************************************************************************/
	template <class T>
	concept binary_readable = std::invocable<decltype(binary_reader<T>::read_from_stream), std::istream&, T&> &&
							  std::invocable<decltype(binary_reader<T>::read_from_span), std::span<const std::byte>, T&> &&
							  std::same_as<return_type_t<decltype(binary_reader<T>::read_from_span)>, std::span<const std::byte>>;

	/******************************************************************************************************************
	 * Concept that denotes a type able to be constructed with binary_read.
	 *
	 * To fulfill this condition, @em T must be binary readable and default-constructible.
	 *****************************************************************************************************************/
	template <class T>
	concept binary_constructible = binary_readable<T> && std::default_initializable<T>;

	/******************************************************************************************************************
	 * Concept that denotes a type able to use the default binary reader.
	 *
	 * To fulfill this condition, @em T must be a standard layout type.
	 *****************************************************************************************************************/
	template <class T>
	concept default_binary_readable = std::is_standard_layout_v<T>;

	/******************************************************************************************************************
	 * Default binary reader.
	 *****************************************************************************************************************/
	template <default_binary_readable T> struct binary_reader<T> {
		using default_reader = std::true_type;

		static void read_from_stream(std::istream& is, T& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, T& out);
	};

	/******************************************************************************************************************
	 * Span binary reader.
	 *****************************************************************************************************************/
	template <binary_readable T> struct binary_reader<std::span<T>> {
		static void read_from_stream(std::istream& is, const std::span<T>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, const std::span<T>& out);
	};

	/******************************************************************************************************************
	 * Pair binary reader.
	 *****************************************************************************************************************/
	template <binary_readable A, binary_readable B> struct binary_reader<std::pair<A, B>> {
		static void read_from_stream(std::istream& is, std::pair<A, B>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::pair<A, B>& out);
	};

	/******************************************************************************************************************
	 * String binary reader.
	 *****************************************************************************************************************/
	template <> struct binary_reader<std::string> {
		static void read_from_stream(std::istream& is, std::string& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::string& out);
	};

	/******************************************************************************************************************
	 * Vector binary reader.
	 *****************************************************************************************************************/
	template <binary_constructible T> struct binary_reader<std::vector<T>> {
		static void read_from_stream(std::istream& is, std::vector<T>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::vector<T>& out);
	};

	/******************************************************************************************************************
	 * Set binary reader.
	 *****************************************************************************************************************/
	template <binary_constructible K, class... Args> struct binary_reader<std::set<K, Args...>> {
		static void read_from_stream(std::istream& is, std::set<K, Args...>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::set<K, Args...>& out);
	};

	/******************************************************************************************************************
	 * Map binary reader.
	 *****************************************************************************************************************/
	template <binary_constructible K, binary_constructible V, class... Args> struct binary_reader<std::map<K, V, Args...>> {
		static void read_from_stream(std::istream& is, std::map<K, V, Args...>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::map<K, V, Args...>& out);
	};

	/******************************************************************************************************************
	 * Unordered set binary reader.
	 *****************************************************************************************************************/
	template <binary_constructible K, class... Args> struct binary_reader<std::unordered_set<K, Args...>> {
		static void read_from_stream(std::istream& is, std::unordered_set<K, Args...>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::unordered_set<K, Args...>& out);
	};

	/******************************************************************************************************************
	 * Hashmap binary reader.
	 *****************************************************************************************************************/
	template <binary_constructible K, binary_constructible V, class... Args> struct binary_reader<std::unordered_map<K, V, Args...>> {
		static void read_from_stream(std::istream& is, std::unordered_map<K, V, Args...>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, std::unordered_map<K, V, Args...>& out);
	};

	/******************************************************************************************************************
	 * Does a binary read from a stream.
	 *
	 * @param[in] is The input stream.
	 * @param[out] out The output object.
	 *****************************************************************************************************************/
	template <binary_readable T> void binary_read(std::istream& is, T& out);

	/******************************************************************************************************************
	 * Does a binary read from a stream.
	 *
	 * @param[in] is The input stream.
	 *
	 * @return The read object.
	 *****************************************************************************************************************/
	template <binary_constructible T> T binary_read(std::istream& is);

	/******************************************************************************************************************
	 * Does a binary read from a span.
	 *
	 * @param[in] span The input span.
	 * @param[out] out The output object.
	 *
	 * @return The span with the read-from region removed.
	 *****************************************************************************************************************/
	template <binary_readable T> std::span<const std::byte> binary_read(std::span<const std::byte> span, T& out);

	/******************************************************************************************************************
	 * Does a binary read from a range.
	 *
	 * @param[in] range The input range.
	 * @param[out] out The output object.
	 *****************************************************************************************************************/
	template <binary_readable T, std::ranges::contiguous_range Range> void binary_read(Range&& range, T& out);

	/******************************************************************************************************************
	 * Does a binary read from a span.
	 *
	 * @param[in] span The input span.
	 *
	 * @return The read object.
	 *****************************************************************************************************************/
	template <binary_constructible T> T binary_read(std::span<const std::byte>& span);

	/******************************************************************************************************************
	 * Does a binary read from a range.
	 *
	 * @param[in] range The input range.
	 *
	 * @return The read object.
	 *****************************************************************************************************************/
	template <binary_constructible T, std::ranges::contiguous_range Range> T binary_read(Range&& range);

	/******************************************************************************************************************
	 * Checks for magic bytes from a stream.
	 *
	 * @tparam Literal The magic literal to check for.
	 *
	 * @param[in] is The input stream.
	 *
	 * @return True if the sequence was found, false otherwise.
	 *****************************************************************************************************************/
	template <template_string_literal Literal> bool binary_read_magic(std::istream& is);

	/******************************************************************************************************************
	 * Checks for magic bytes from a span.
	 *
	 * @tparam Literal The magic literal to check for.
	 *
	 * @param[in] is The input span.
	 *
	 * @return True if the sequence was found, false otherwise.
	 *****************************************************************************************************************/
	template <template_string_literal Literal> bool binary_read_magic(std::span<const std::byte>& span);

	//

	/******************************************************************************************************************
	 * Concept that denotes a binaryFlush-compatible iterator.
	 *
	 * To satisfy this requirement, a container must be an output iterator to a char variant or std::byte.
	 *****************************************************************************************************************/
	template <class T>
	concept binary_flushable_iterator = std::output_iterator<T, char> || std::output_iterator<T, signed char> ||
										std::output_iterator<T, unsigned char> || std::output_iterator<T, std::byte>;

	/******************************************************************************************************************
	 * Flushes the rest of the stream into an output iterator.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 *
	 * @param[in] is The input stream.
	 * @param[out] out The output iterator.
	 ******************************************************************************************************************/
	template <tr::binary_flushable_iterator It> void flush_binary(std::istream& is, It out);

	/******************************************************************************************************************
	 * Flushes the rest of the stream into a vector of bytes.
	 *
	 * @exception std::ios_base::failure If the stream throws an error.
	 * @exception std::bad_alloc If vector construction fails.
	 *
	 * @param[in] is The input stream.
	 *
	 * @return A byte vector with the rest of the stream data.
	 ******************************************************************************************************************/
	std::vector<std::byte> flush_binary(std::istream& is);

	//

	/******************************************************************************************************************
	 * Interface for custom writes for use in binary_write.
	 *
	 * To fulfill the requirements for a valid binary reader specialization, the following must be present:
	 *
	 * static function write_to_stream that takes std::ostream& and const T&.
	 *
	 * static function write_to_span that takes a byte span and const T& and returns another byte span with the written
	 * to portion cut off.
	 *
	 * If these conditions are properly met, @em T will fulfill binary_writable.
	 *****************************************************************************************************************/
	template <class T> struct binary_writer {};

	/******************************************************************************************************************
	 * Concept that denotes a type able to be read with binary_read.
	 *
	 * To fulfill this condition, binary_writer<T> must have the static function @em write_to_stream and @em
	 * write_to_span fulfilling the conditions detailed in the description for @em binary_writer.
	 *****************************************************************************************************************/
	template <class T>
	concept binary_writable = std::invocable<decltype(binary_writer<T>::write_to_stream), std::ostream&, const T&> &&
							  std::invocable<decltype(binary_writer<T>::write_to_span), std::span<std::byte>, const T&> &&
							  std::same_as<return_type_t<decltype(binary_writer<T>::write_to_span)>, std::span<std::byte>>;

	/******************************************************************************************************************
	 * Concept that denotes a type able to use the default binary writer.
	 *
	 * To fulfill this condition, @em T must be a standard layout type.
	 *****************************************************************************************************************/
	template <class T>
	concept default_binary_writable = std::is_standard_layout_v<T>;

	/******************************************************************************************************************
	 * Default binary writer.
	 *****************************************************************************************************************/
	template <default_binary_writable T> struct binary_writer<T> {
		using default_writer = std::true_type;

		static void write_to_stream(std::ostream& os, const T& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const T& in);
	};

	/******************************************************************************************************************
	 * String literal binary writer.
	 *****************************************************************************************************************/
	template <std::size_t S> struct binary_writer<char[S]> {
		static void write_to_stream(std::ostream& os, const char (&in)[S]);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const char (&in)[S]);
	};

	/******************************************************************************************************************
	 * Unformatted span binary writer.
	 *****************************************************************************************************************/
	template <> struct binary_writer<std::span<const std::byte>> {
		static void write_to_stream(std::ostream& os, const std::span<const std::byte>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::span<const std::byte>& in);
	};

	/******************************************************************************************************************
	 * Unformatted span binary writer.
	 *****************************************************************************************************************/
	template <> struct binary_writer<std::span<std::byte>> {
		static void write_to_stream(std::ostream& os, const std::span<std::byte>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::span<std::byte>& in);
	};

	/******************************************************************************************************************
	 * Span binary writer.
	 *****************************************************************************************************************/
	template <binary_writable T> struct binary_writer<std::span<T>> {
		static void write_to_stream(std::ostream& os, const std::span<T>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::span<T>& in);
	};

	/******************************************************************************************************************
	 * Span binary writer.
	 *****************************************************************************************************************/
	template <binary_writable T> struct binary_writer<std::span<const T>> {
		static void write_to_stream(std::ostream& os, const std::span<const T>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::span<const T>& in);
	};

	/******************************************************************************************************************
	 * Pair binary writer.
	 *****************************************************************************************************************/
	template <binary_writable A, binary_writable B> struct binary_writer<std::pair<A, B>> {
		static void write_to_stream(std::ostream& os, const std::pair<A, B> in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::pair<A, B>& in);
	};

	/******************************************************************************************************************
	 * String view binary writer.
	 *****************************************************************************************************************/
	template <> struct binary_writer<std::string_view> {
		static void write_to_stream(std::ostream& os, const std::string_view& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::string_view& in);
	};

	/******************************************************************************************************************
	 * String binary writer.
	 *****************************************************************************************************************/
	template <> struct binary_writer<std::string> {
		static void write_to_stream(std::ostream& os, const std::string& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::string& in);
	};

	/******************************************************************************************************************
	 * Vector binary writer.
	 *****************************************************************************************************************/
	template <binary_writable T> struct binary_writer<std::vector<T>> {
		static void write_to_stream(std::ostream& os, const std::vector<T>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::vector<T>& in);
	};

	/******************************************************************************************************************
	 * Set binary writer.
	 *****************************************************************************************************************/
	template <binary_writable K, class... Args> struct binary_writer<std::set<K, Args...>> {
		static void write_to_stream(std::ostream& os, const std::set<K, Args...>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::set<K, Args...>& in);
	};

	/******************************************************************************************************************
	 * Map binary writer.
	 *****************************************************************************************************************/
	template <binary_writable K, binary_writable V, class... Args> struct binary_writer<std::map<K, V, Args...>> {
		static void write_to_stream(std::ostream& os, const std::map<K, V, Args...>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::map<K, V, Args...>& in);
	};

	/******************************************************************************************************************
	 * Unordered set binary writer.
	 *****************************************************************************************************************/
	template <binary_writable K, class... Args> struct binary_writer<std::unordered_set<K, Args...>> {
		static void write_to_stream(std::ostream& os, const std::unordered_set<K, Args...>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::unordered_set<K, Args...>& in);
	};

	/******************************************************************************************************************
	 * Hashmap binary writer.
	 *****************************************************************************************************************/
	template <binary_writable K, binary_writable V, class... Args> struct binary_writer<std::unordered_map<K, V, Args...>> {
		static void write_to_stream(std::ostream& os, const std::unordered_map<K, V, Args...>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const std::unordered_map<K, V, Args...>& in);
	};

	/******************************************************************************************************************
	 * Does a binary write to a stream.
	 *
	 * @param[out] os The output stream.
	 * @param[in] in The input object.
	 *****************************************************************************************************************/
	template <binary_writable T> void binary_write(std::ostream& os, const T& in);

	/******************************************************************************************************************
	 * Does a binary write to a span.
	 *
	 * @param[out] os The output span.
	 * @param[in] in The input object.
	 *
	 * @return The output span with the written-to part removed.
	 *****************************************************************************************************************/
	template <binary_writable T> std::span<std::byte> binary_write(std::span<std::byte> span, const T& in);
} // namespace tr

#include "binary_io_impl.hpp"