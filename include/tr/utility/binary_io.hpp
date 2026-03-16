///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an interface for reading and writing binary data.                                                                            //
//                                                                                                                                       //
// Binary data can be read to one or multiple output variables from a stream using tr::read_binary. If the output type is                //
// default-constructible, a variant of the function may be used that returns the read value. A special reading operation exists for      //
// checking for magic bytes:                                                                                                             //
//     - int x; tr::read_binary(is, x) -> reads an integer value from 'is' into x                                                        //
//     - int x; char y; tr::read_binary(is, x, y) -> reads an integer to 'x' and a character into 'y' from 'is'                          //
//     - tr::read_binary<int>(is) -> reads an integer value from 'is'                                                                    //
//     - tr::read_binary_magic<"tr">(is) -> reads 2 bytes from 'is' and returns true if they match 'tr', and false otherwise             //
//                                                                                                                                       //
// It is sometimes necessary to extract all remaining data out of a stream, which can be done with tr::flush_binary, outputting either   //
// to an existing vector, or to a new one which is then returned:                                                                        //
//     - std::vector<std::byte> buffer; tr::flush_binary(is, buffer) -> extracts all data from 'is' into buffer                          //
//     - tr::flush_binary(is) -> extracts all data from 'is' into a new vector                                                           //
//                                                                                                                                       //
// Binary data can be written to an output stream using tr::write_binary:                                                                //
//     - tr::write_binary(os, 50, 1.0f) -> writes the bytes of integer value '50' and float '1.0f' to 'os'                               //
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
	template <cv_unqualified_object Out> struct binary_reader;
	// Interface for custom writers for use in write_binary.
	template <cv_unqualified_object In> struct binary_writer;

	// Concept that denotes a type able to be read with read_binary.
	template <typename T>
	concept binary_readable = requires(std::istream& is, T& out) { tr::binary_reader<std::remove_volatile_t<T>>{}(is, out); };
	// Concept that denotes a type passable to the variadic read_binary: a span or a reference to a binary readable.
	template <typename T>
	concept span_or_ref_to_binary_readable =
		(lvalue_reference<T> && binary_readable<std::remove_reference_t<T>>) || specialization_of_tv<std::remove_cvref_t<T>, std::span>;
	// Concept that denotes a type able to be constructed with read_binary.
	template <typename T>
	concept binary_constructible = binary_readable<T> && std::default_initializable<T>;
	// Concept that denotes a flush_binary-compatible iterator.
	template <typename T>
	concept binary_flushable_iterator = std::output_iterator<T, char> || std::output_iterator<T, signed char> ||
										std::output_iterator<T, unsigned char> || std::output_iterator<T, std::byte>;
	// Concept that denotes a type able to be read with a stream write_binary.
	template <typename T>
	concept binary_writable = requires(std::ostream& os, const T& in) { tr::binary_writer<std::remove_cv_t<T>>{}(os, in); };

	// Raw binary reader template: directly reads bytes into the object.
	template <standard_layout Out> struct raw_binary_reader {
		using raw_reader = std::true_type;
		void operator()(std::istream& is, Out& out) const;
	};
	// Raw binary writer template: directly writes bytes from the object.
	template <standard_layout In> struct raw_binary_writer {
		using raw_writer = std::true_type;
		void operator()(std::ostream& os, const In& in);
	};

	// Reads binary data from a stream.
	template <binary_readable Out> void read_binary(std::istream& is, Out& out);
	// Reads binary data from a stream.
	template <binary_readable Out, usize Size> void read_binary(std::istream& is, std::span<Out, Size> out);
	// Reads binary data from a stream.
	template <span_or_ref_to_binary_readable... Outs>
		requires(sizeof...(Outs) >= 2)
	void read_binary(std::istream& is, Outs&&... outs);
	// Reads binary data from a stream.
	template <binary_constructible Out> Out read_binary(std::istream& is);
	// Checks for magic bytes from a stream.
	template <string_literal Literal> bool read_binary_magic(std::istream& is);

	// Flushes the rest of the stream into an output iterator.
	template <tr::binary_flushable_iterator Iterator> void flush_binary(std::istream& is, Iterator out);
	// Flushes the rest of the stream into a vector of bytes.
	std::vector<std::byte> flush_binary(std::istream& is);

	// Writes binary data to a stream.
	template <binary_writable In> void write_binary(std::ostream& os, const In& in);
	// Writes binary data to a stream.
	template <binary_writable... Ins>
		requires(sizeof...(Ins) >= 2)
	void write_binary(std::ostream& os, const Ins&... ins);
} // namespace tr

///////////////////////////////////////////////////////////// SPECIALIZATIONS /////////////////////////////////////////////////////////////

namespace tr {
	// Arithmetic binary readers.
	template <arithmetic Arithmetic>
		requires(cv_unqualified_object<Arithmetic>)
	struct binary_reader<Arithmetic> : raw_binary_reader<Arithmetic> {};
	// Enumerator binary readers.
	template <enumerator Enumerator>
		requires(cv_unqualified_object<Enumerator>)
	struct binary_reader<Enumerator> : raw_binary_reader<Enumerator> {};
	// Vector binary readers.
	template <int Dimensions, typename Element>
	struct binary_reader<glm::vec<Dimensions, Element>> : raw_binary_reader<glm::vec<Dimensions, Element>> {};
	// Matrix binary readers.
	template <int Columns, int Rows, typename Element>
	struct binary_reader<glm::mat<Columns, Rows, Element>> : raw_binary_reader<glm::mat<Columns, Rows, Element>> {};

	// Array binary reader.
	template <binary_readable Element, usize Size> struct binary_reader<std::array<Element, Size>> {
		void operator()(std::istream& is, std::array<Element, Size>& out) const;
	};
	// Pair binary reader.
	template <binary_readable First, binary_readable Second> struct binary_reader<std::pair<First, Second>> {
		void operator()(std::istream& is, std::pair<First, Second>& out) const;
	};
	// String binary reader.
	template <> struct binary_reader<std::string> {
		void operator()(std::istream& is, std::string& out) const;
	};
	// Vector binary reader.
	template <binary_constructible Element> struct binary_reader<std::vector<Element>> {
		void operator()(std::istream& is, std::vector<Element>& out) const;
	};
	// Set binary reader.
	template <binary_constructible Key, typename... Other> struct binary_reader<std::set<Key, Other...>> {
		void operator()(std::istream& is, std::set<Key, Other...>& out) const;
	};
	// Map binary reader.
	template <binary_constructible Key, binary_constructible V, typename... Other> struct binary_reader<std::map<Key, V, Other...>> {
		void operator()(std::istream& is, std::map<Key, V, Other...>& out) const;
	};
	// Unordered flat set binary reader.
	template <binary_constructible Key, typename... Other> struct binary_reader<boost::unordered_flat_set<Key, Other...>> {
		void operator()(std::istream& is, boost::unordered_flat_set<Key, Other...>& out) const;
	};
	// Unordered node set binary reader.
	template <binary_constructible Key, typename... Other> struct binary_reader<boost::unordered_node_set<Key, Other...>> {
		void operator()(std::istream& is, boost::unordered_node_set<Key, Other...>& out) const;
	};
	// Unordered flat map binary reader.
	template <binary_constructible Key, binary_constructible V, typename... Other>
	struct binary_reader<boost::unordered_flat_map<Key, V, Other...>> {
		void operator()(std::istream& is, boost::unordered_flat_map<Key, V, Other...>& out) const;
	};
	// Unordered node map binary reader.
	template <binary_constructible Key, binary_constructible V, typename... Other>
	struct binary_reader<boost::unordered_node_map<Key, V, Other...>> {
		void operator()(std::istream& is, boost::unordered_node_map<Key, V, Other...>& out) const;
	};

	// Arithmetic binary writers.
	template <arithmetic Arithmetic>
		requires(cv_unqualified_object<Arithmetic>)
	struct binary_writer<Arithmetic> : raw_binary_writer<Arithmetic> {};
	// Enumerator binary writers.
	template <enumerator Enumerator>
		requires(cv_unqualified_object<Enumerator>)
	struct binary_writer<Enumerator> : raw_binary_writer<Enumerator> {};
	// Vector binary writers.
	template <int Dimensions, typename Element>
	struct binary_writer<glm::vec<Dimensions, Element>> : raw_binary_writer<glm::vec<Dimensions, Element>> {};
	// Matrix binary writers.
	template <int Columns, int Rows, typename Element>
	struct binary_writer<glm::mat<Columns, Rows, Element>> : raw_binary_writer<glm::mat<Columns, Rows, Element>> {};
	// String literal binary writer.
	template <usize Size> struct binary_writer<char[Size]> {
		void operator()(std::ostream& os, const char (&in)[Size]) const;
	};
	// Unformatted span binary writer.
	template <> struct binary_writer<std::span<const std::byte>> {
		void operator()(std::ostream& os, const std::span<const std::byte>& in) const;
	};
	// Unformatted span binary writer.
	template <> struct binary_writer<std::span<std::byte>> : binary_writer<std::span<const std::byte>> {};
	// Span binary writer.
	template <binary_writable Element, usize Size> struct binary_writer<std::span<Element, Size>> {
		void operator()(std::ostream& os, const std::span<Element, Size>& in) const;
	};
	// Pair binary writer.
	template <binary_writable First, binary_writable Second> struct binary_writer<std::pair<First, Second>> {
		void operator()(std::ostream& os, const std::pair<First, Second> in) const;
	};
	// String view binary writer.
	template <> struct binary_writer<std::string_view> {
		void operator()(std::ostream& os, const std::string_view& in) const;
	};
	// String binary writer.
	template <> struct binary_writer<std::string> {
		void operator()(std::ostream& os, const std::string& in) const;
	};
	// Array binary writer.
	template <binary_writable Element, usize Size> struct binary_writer<std::array<Element, Size>> {
		void operator()(std::ostream& os, const std::array<Element, Size>& in) const;
	};
	// Vector binary writer.
	template <binary_writable Element> struct binary_writer<std::vector<Element>> {
		void operator()(std::ostream& os, const std::vector<Element>& in) const;
	};
	// Set binary writer.
	template <binary_writable Key, typename... Other> struct binary_writer<std::set<Key, Other...>> {
		void operator()(std::ostream& os, const std::set<Key, Other...>& in) const;
	};
	// Map binary writer.
	template <binary_writable Key, binary_writable Value, typename... Other> struct binary_writer<std::map<Key, Value, Other...>> {
		void operator()(std::ostream& os, const std::map<Key, Value, Other...>& in) const;
	};
	// Unordered flat set binary writer.
	template <binary_writable Key, typename... Other> struct binary_writer<boost::unordered_flat_set<Key, Other...>> {
		void operator()(std::ostream& os, const boost::unordered_flat_set<Key, Other...>& in) const;
	};
	// Unordered node set binary writer.
	template <binary_writable Key, typename... Other> struct binary_writer<boost::unordered_node_set<Key, Other...>> {
		void operator()(std::ostream& os, const boost::unordered_node_set<Key, Other...>& in) const;
	};
	// Unordered flat map writer.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<boost::unordered_flat_map<Key, Value, Other...>> {
		void operator()(std::ostream& os, const boost::unordered_flat_map<Key, Value, Other...>& in) const;
	};
	// Unordered node map writer.
	template <binary_writable Key, binary_writable Value, typename... Other>
	struct binary_writer<boost::unordered_node_map<Key, Value, Other...>> {
		void operator()(std::ostream& os, const boost::unordered_node_map<Key, Value, Other...>& in) const;
	};
} // namespace tr

#include "impl/binary_io.hpp" // IWYU pragma: export