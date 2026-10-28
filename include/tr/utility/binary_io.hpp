/// @file
/// @brief Provides an interface for reading and writing binary data.

#pragma once
#include <tr/utility/integer.hpp>
#include <tr/utility/specialization_of.hpp>

//

namespace tr
{
	/// Interface for custom readers for use in read_binary.
	/// @tparam Out Output type.
	/// @note Any overloads to const or reference-qualified types will be ignored.
	template <typename Out>
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
	/// @note Any overloads to const or reference-qualified types will be ignored.
	template <typename In>
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
	template <typename Target>
	concept binary_readable = requires(std::istream& is, Target& out) { tr::binary_reader<std::remove_cvref_t<Target>>{}(is, out); } ||
							  specialization_of_tc<std::remove_cvref_t<Target>, std::span>;

	/// Type able to be constructed with read_binary.
	template <typename Target>
	concept binary_constructible = binary_readable<Target> && std::default_initializable<Target>;

	/// Type able to be read with a stream write_binary.
	template <typename Target>
	concept binary_writable = requires(std::ostream& os, const Target& in) { tr::binary_writer<std::remove_const_t<Target>>{}(os, in); };

	/// @name Binary IO
	/// @{

	/// Reads binary data from a stream.
	/// @tparam Out Binary-readable type.
	/// @param is Input stream.
	/// @param out Output variable.
	template <binary_readable Out>
	void read_binary(std::istream& is, Out& out)
	{
		binary_reader<Out>{}(is, out);
	}

	/// Reads binary data from a stream.
	/// @tparam Out Binary-readable type.
	/// @tparam Size Size of the output span.
	/// @param is Input stream.
	/// @param out Output span.
	template <binary_readable Out, usize Size>
	void read_binary(std::istream& is, std::span<Out, Size> out)
	{
		if constexpr (requires { requires std::same_as<typename binary_reader<Out>::default_reader, std::true_type>; }) {
			is.read(reinterpret_cast<char*>(out.data()), out.size_bytes());
		}
		else {
			for (Out& v : out) {
				read_binary(is, v);
			}
		}
	}

	/// Reads binary data from a stream.
	/// @tparam Outs Binary-readable types.
	/// @param is Input stream.
	/// @param outs Output variables.
	template <typename... Outs>
		requires(sizeof...(Outs) >= 2 && (binary_readable<Outs> && ...))
	void read_binary(std::istream& is, Outs&&... outs)
	{
		(read_binary(is, outs), ...);
	}

	/// Reads binary data from a stream.
	/// @tparam Out Binary-readable type.
	/// @param is Input stream.
	/// @return Read variable.
	template <binary_constructible Out>
	[[nodiscard]] Out read_binary(std::istream& is)
	{
		Out out;
		read_binary(is, out);
		return out;
	}

	/// Checks for magic bytes from a stream.
	/// @param is Input stream.
	/// @param magic Magic bytes to match.
	/// @return `true` if the magic bytes were matched, `false` otherwise.
	[[nodiscard]] bool read_binary_magic(std::istream& is, std::string_view magic);

	//

	/// Flushes the rest of the stream into an output iterator.
	/// @tparam Iterator Iterator to a character-like type.
	/// @param is Input stream.
	/// @param out Output iterator.
	template <typename Iterator>
		requires std::output_iterator<Iterator, char> || std::output_iterator<Iterator, signed char> ||
				 std::output_iterator<Iterator, unsigned char> || std::output_iterator<Iterator, std::byte>
	void flush_binary(std::istream& is, Iterator out)
	{
		while (is.peek() != EOF) {
			if constexpr (std::output_iterator<Iterator, char>) {
				*out++ = static_cast<char>(is.get());
			}
			else if constexpr (std::output_iterator<Iterator, signed char>) {
				*out++ = static_cast<signed char>(is.get());
			}
			else if constexpr (std::output_iterator<Iterator, unsigned char>) {
				*out++ = static_cast<unsigned char>(is.get());
			}
			else {
				*out++ = static_cast<std::byte>(is.get());
			}
		}
	}

	/// Flushes the rest of the stream into a vector of bytes.
	/// @param is Input stream.
	/// @return Vector containing the remaining contents of the stream.
	[[nodiscard]] std::vector<std::byte> flush_binary(std::istream& is);

	//

	/// Writes binary data to a stream.
	/// @tparam In Binary-writable type.
	/// @param os Output stream.
	/// @param in Variable to write to the stream.
	template <binary_writable In>
	void write_binary(std::ostream& os, const In& in)
	{
		binary_writer<std::remove_const_t<In>>{}(os, in);
	}

	/// Writes binary data to a stream.
	/// @tparam Ins Binary-writable types.
	/// @param os Output stream.
	/// @param ins Variables to write to the stream.
	template <binary_writable... Ins>
		requires(sizeof...(Ins) >= 2)
	void write_binary(std::ostream& os, const Ins&... ins)
	{
		(write_binary(os, ins), ...);
	}

	/// Writes magic bytes to a stream.
	/// @param os Output stream.
	/// @param magic Magic bytes to write to the stream.
	void write_binary_magic(std::ostream& os, std::string_view magic);

	/// @}
} // namespace tr