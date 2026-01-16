///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an interface for reading and writing binary data.                                                                            //
//                                                                                                                                       //
// Binary data can be read to an output variable from a stream using tr::read_binary. If the output type is default-constructible, a     //
// variant of the function may be used that returns the read value. A special reading operation exists for checking for magic bytes:     //
//     - int x; tr::read_binary(is, x) -> reads an integer value from 'is' into x                                                        //
//     - tr::read_binary<int>(is) -> reads an integer value from 'is'                                                                    //
//     - tr::read_binary_magic<"tr">(is) -> reads 2 bytes from 'is' and returns true if they match 'tr', and false otherwise             //
//                                                                                                                                       //
// It is sometimes necessary to extract all remaining data out of a stream, which can be done with tr::flush_binary, outputting either   //
// to an existing vector, or to a new one which is then returned:                                                                        //
//     - std::vector<std::byte> buffer; tr::flush_binary(is, buffer) -> extracts all data from 'is' into buffer                          //
//     - tr::flush_binary(is) -> extracts all data from 'is' into a new vector                                                           //
//                                                                                                                                       //
// Binary data can be written to an output stream using tr::write_binary:                                                                //
//     - tr::write_binary(os, 50) -> writes the bytes of integer value '50' to 'os'                                                      //
//                                                                                                                                       //
// To enable binary reading and/or writing for a custom type, the structs tr::binary_reader and tr::binary_writer respectively must be   //
// specialized, tr::binary_reader with operator()(std::istream&, T&) and tr::binary_writer with operator()(std::ostream&, const T&).     //
// Most primitives and standard library containers, as well as some tr types have specialized readers and writers. tr::raw_binary_reader //
// and tr::raw_binary_writer are provided as convenience and can be inherited from for the simplest case (read/write the bytes of an     //
// object directly):                                                                                                                     //
//     - template <> struct tr::binary_reader<my_int> : tr::raw_binary_reader<my_int> {};                                                //
//       -> enables binary reading of class my_int, reader directly writes to the bytes of a my_int object                               //
//     - template <> struct tr::binary_writer<my_int> : tr::raw_binary_writer<my_int> {};                                                //
//       -> enables binary writing of class my_int, writer directly reads from the bytes of a my_int object                              //
//     - template <> struct tr::binary_reader<my_int> {                                                                                  //
//           void operator()(std::istream& is, my_int& out) { read_binary(is, out.m_base); }                                             //
//       }                                                                                                                               //
//       -> enables binary reading of class my_int with a custom reader                                                                  //
//     - template <> struct tr::binary_writer<my_int> {                                                                                  //
//           void operator()(std::ostream& os, const my_int& in) { write_binary(os, in.m_base); }                                        //
//       }                                                                                                                               //
//       -> enables binary writing of class my_int with a custom writer                                                                  //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "concepts.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Interface for custom readers for use in read_binary.
	template <cv_unqualified_object T> struct binary_reader;
	// Interface for custom writers for use in write_binary.
	template <cv_unqualified_object T> struct binary_writer;

	// Concept that denotes a type able to be read with read_binary.
	template <class T>
	concept binary_readable = requires(std::istream& is, T& out) { tr::binary_reader<std::remove_volatile_t<T>>{}(is, out); };
	// Concept that denotes a type able to be constructed with read_binary.
	template <class T>
	concept binary_constructible = binary_readable<T> && std::default_initializable<T>;
	// Concept that denotes a flush_binary-compatible iterator.
	template <class T>
	concept binary_flushable_iterator = std::output_iterator<T, char> || std::output_iterator<T, signed char> ||
										std::output_iterator<T, unsigned char> || std::output_iterator<T, std::byte>;
	// Concept that denotes a type able to be read with a stream write_binary.
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

	// Reads binary data from a stream.
	template <binary_readable T> void read_binary(std::istream& is, T& out);
	// Reads binary data from a stream.
	template <binary_readable T, usize S> void read_binary(std::istream& is, std::span<T, S> out);
	// Reads binary data from a stream.
	template <binary_constructible T> T read_binary(std::istream& is);
	// Checks for magic bytes from a stream.
	template <string_literal Literal> bool read_binary_magic(std::istream& is);

	// Flushes the rest of the stream into an output iterator.
	template <tr::binary_flushable_iterator It> void flush_binary(std::istream& is, It out);
	// Flushes the rest of the stream into a vector of bytes.
	std::vector<std::byte> flush_binary(std::istream& is);

	// Writes binary data to a stream.
	template <binary_writable T> void write_binary(std::ostream& os, const T& in);
} // namespace tr

///////////////////////////////////////////////////////////// SPECIALIZATIONS /////////////////////////////////////////////////////////////

namespace tr {
	// Arithmetic binary readers.
	template <arithmetic T>
		requires(cv_unqualified_object<T>)
	struct binary_reader<T> : raw_binary_reader<T> {};
	// Enumerator binary readers.
	template <enumerator T>
		requires(cv_unqualified_object<T>)
	struct binary_reader<T> : raw_binary_reader<T> {};
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
	template <arithmetic T>
		requires(cv_unqualified_object<T>)
	struct binary_writer<T> : raw_binary_writer<T> {};
	// Enumerator binary writers.
	template <enumerator T>
		requires(cv_unqualified_object<T>)
	struct binary_writer<T> : raw_binary_writer<T> {};
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

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "binary_io_impl.hpp" // IWYU pragma: keep