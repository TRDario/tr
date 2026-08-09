/// @file
/// @brief Provides an interface for reading and writing binary data.

#pragma once
#include "concepts.hpp"
#include "specialization_of.hpp"

//

namespace tr
{
	/// Interface for custom readers for use in read_binary.
	/// @tparam Out Output type.
	template <cv_unqualified_object Out>
	struct binary_reader
	{
		static_assert("binary_reader is not specialized for this type!");

		/// Reads a variable from a stream.
		/// @param is Input stream.
		/// @param out Output parameter.
		static void operator()(std::istream& is, Out& out) = delete;
	};

	/// Interface for custom writers for use in write_binary.
	/// @tparam In Input type.
	template <cv_unqualified_object In>
	struct binary_writer
	{
		static_assert("binary_writer is not specialized for this type!");

		/// Writes a variable to a stream.
		/// @param os Output stream.
		/// @param in Variable to write to the stream.
		static void operator()(std::ostream& os, const In& in) = delete;
	};

	//

	/// Type able to be read with read_binary.
	template <typename T>
	concept binary_readable = requires(std::istream& is, T& out) { tr::binary_reader<std::remove_volatile_t<T>>{}(is, out); };

	/// Type passable to the variadic read_binary: a span or a reference to a binary readable.
	template <typename T>
	concept span_or_ref_to_binary_readable = (lvalue_reference<T> && binary_readable<std::remove_reference_t<T>>) ||
											 specialization_of_tv<std::remove_cvref_t<T>, std::span>;

	/// Type able to be constructed with read_binary.
	template <typename T>
	concept binary_constructible = binary_readable<T> && std::default_initializable<T>;

	/// Iterator type able to be used with flush_binary.
	template <typename T>
	concept binary_flushable_iterator = std::output_iterator<T, char> || std::output_iterator<T, signed char> ||
										std::output_iterator<T, unsigned char> || std::output_iterator<T, std::byte>;

	/// Type able to be read with a stream write_binary.
	template <typename T>
	concept binary_writable = requires(std::ostream& os, const T& in) { tr::binary_writer<std::remove_cv_t<T>>{}(os, in); };

	/// @name Binary IO
	/// @{

	/// Reads binary data from a stream.
	/// @tparam Out Binary-readable type.
	/// @param is Input stream.
	/// @param out Output variable.
	template <binary_readable Out>
	void read_binary(std::istream& is, Out& out);

	/// Reads binary data from a stream.
	/// @tparam Out Binary-readable type.
	/// @tparam Size Size of the output span.
	/// @param is Input stream.
	/// @param out Output span.
	template <binary_readable Out, usize Size>
	void read_binary(std::istream& is, std::span<Out, Size> out);

	/// Reads binary data from a stream.
	/// @tparam Outs Binary-readable types.
	/// @param is Input stream.
	/// @param outs Output variables.
	template <span_or_ref_to_binary_readable... Outs>
		requires(sizeof...(Outs) >= 2)
	void read_binary(std::istream& is, Outs&&... outs);

	/// Reads binary data from a stream.
	/// @tparam Out Binary-readable type.
	/// @param is Input stream.
	/// @return Read variable.
	template <binary_constructible Out>
	Out read_binary(std::istream& is);

	/// Checks for magic bytes from a stream.
	/// @param is Input stream.
	/// @param magic Magic bytes to match.
	/// @return `true` if the magic bytes were matched, `false` otherwise.
	bool read_binary_magic(std::istream& is, std::string_view magic);

	//

	/// Flushes the rest of the stream into an output iterator.
	/// @tparam Iterator Iterator to a character-like type.
	/// @param is Input stream.
	/// @param out Output iterator.
	template <tr::binary_flushable_iterator Iterator>
	void flush_binary(std::istream& is, Iterator out);

	/// Flushes the rest of the stream into a vector of bytes.
	/// @param is Input stream.
	/// @return Vector containing the remaining contents of the stream.
	std::vector<std::byte> flush_binary(std::istream& is);

	//

	/// Writes binary data to a stream.
	/// @tparam In Binary-writable type.
	/// @param os Output stream.
	/// @param in Variable to write to the stream.
	template <binary_writable In>
	void write_binary(std::ostream& os, const In& in);

	/// Writes binary data to a stream.
	/// @tparam Ins Binary-writable types.
	/// @param os Output stream.
	/// @param ins Variables to write to the stream.
	template <binary_writable... Ins>
		requires(sizeof...(Ins) >= 2)
	void write_binary(std::ostream& os, const Ins&... ins);

	/// Writes magic bytes to a stream.
	/// @param os Output stream.
	/// @param magic Magic bytes to write to the stream.
	void write_binary_magic(std::ostream& os, std::string_view magic);

	/// @}
} // namespace tr

#include "impl/binary_io.hpp" // IWYU pragma: export