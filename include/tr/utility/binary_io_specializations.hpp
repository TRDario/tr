/// @file
/// @brief Provides binary readers and writers for commonly used types.

#pragma once
#include <tr/utility/binary_io.hpp>

//

namespace tr
{
	/// @ingroup binary_io
	/// @brief Binary reader for bytes.
	template <>
	struct binary_reader<std::byte>
	{
		/// @brief Reads a byte from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Byte to output to.
		/// @exception binary_read_error If reading the byte failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, std::byte& output)
		{
			const usize start_position{source.position()};
			const usize read_bytes{source.read(std::span{&output, 1})};
			if (read_bytes != 1) {
				throw binary_read_error{start_position, 1, read_bytes};
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for bytes.
	template <>
	struct binary_writer<std::byte>
	{
		/// @brief Writes a byte to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Byte to write.
		/// @exception binary_write_error If writing the byte failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, std::byte input)
		{
			const usize start_position{sink.position()};
			const usize written_bytes{sink.write(std::span{&input, 1})};
			if (written_bytes != 1) {
				throw binary_write_error{start_position, 1, written_bytes};
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for byte spans.
	/// @tparam Extent Extent of the byte span.
	template <std::size_t Extent>
	struct binary_reader<std::span<std::byte, Extent>>
	{
		/// @brief Reads a byte span from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Byte span to output to.
		/// @exception binary_read_error If reading the byte span failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, std::span<std::byte, Extent> output)
		{
			const usize start_position{source.position()};
			const usize read_bytes{source.read(output)};
			if (read_bytes != output.size()) {
				throw binary_read_error{start_position, output.size(), read_bytes};
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for byte spans.
	/// @tparam Extent Extent of the byte span.
	template <std::size_t Extent>
	struct binary_writer<std::span<std::byte, Extent>>
	{
		/// @brief Writes a byte span to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Byte span to write.
		/// @exception binary_write_error If writing the byte span failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, std::span<std::byte, Extent> input)
		{
			const usize start_position{sink.position()};
			const usize written_bytes{sink.write(input)};
			if (written_bytes != input.size()) {
				throw binary_write_error{start_position, input.size(), written_bytes};
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for byte spans.
	/// @tparam Extent Extent of the byte span.
	template <std::size_t Extent>
	struct binary_writer<std::span<const std::byte, Extent>>
	{
		/// @brief Writes a byte span to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Byte span to write.
		/// @exception binary_write_error If writing the byte span failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, std::span<const std::byte, Extent> input)
		{
			const usize start_position{sink.position()};
			const usize written_bytes{sink.write(input)};
			if (written_bytes != input.size()) {
				throw binary_write_error{start_position, input.size(), written_bytes};
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for booleans.
	template <>
	struct binary_reader<bool>
	{
		/// @brief Reads a boolean from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Boolean to output to.
		/// @exception binary_read_error If reading the boolean failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, bool& output)
		{
			read_binary(source, as_mut_bytes(output));
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for booleans.
	template <>
	struct binary_writer<bool>
	{
		/// @brief Writes a boolean to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Boolean to write.
		/// @exception binary_write_error If writing the boolean failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, bool input)
		{
			write_binary(sink, u8{input});
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for unsigned integers.
	/// @tparam UnsignedInteger Portable unsigned integer type.
	template <std::unsigned_integral UnsignedInteger>
		requires(sizeof(UnsignedInteger) <= 8)
	struct binary_reader<UnsignedInteger>
	{
		/// @brief Reads an unsigned integer from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Unsigned integer to output to.
		/// @exception binary_read_error If reading the unsigned integer failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, UnsignedInteger& output)
		{
			constexpr usize size_bytes{sizeof(UnsignedInteger)};

			if constexpr (std::endian::native == std::endian::little || size_bytes == 1) {
				read_binary(source, as_mut_bytes(output));
			}
			else {
				const std::array<std::byte, size_bytes> buffer{read_binary<std::array<std::byte, size_bytes>>(source)};
				output = 0;
				for (usize i = 0; i < size_bytes; ++i) {
					output |= std::to_integer<UnsignedInteger>(buffer[size_bytes - i - 1]) << (i << 3);
				}
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for unsigned integers.
	/// @tparam UnsignedInteger Portable unsigned integer type.
	template <std::unsigned_integral UnsignedInteger>
		requires(sizeof(UnsignedInteger) <= 8)
	struct binary_writer<UnsignedInteger>
	{
		/// @brief Writes an unsigned integer to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Unsigned integer to write.
		/// @exception binary_write_error If writing the unsigned integer failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, UnsignedInteger input)
		{
			constexpr usize size_bytes{sizeof(UnsignedInteger)};

			if constexpr (std::endian::native == std::endian::little || size_bytes == 1) {
				write_binary(sink, as_bytes(input));
			}
			else {
				std::array<std::byte, size_bytes> buffer;
				for (usize i = 0; i < size_bytes; ++i) {
					buffer[i] = static_cast<std::byte>((input >> ((size_bytes - i - 1) << 3)) & 255);
				}
				write_binary(sink, buffer);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for signed integers.
	/// @tparam UnsignedInteger Portable signed integer type.
	template <std::signed_integral SignedInteger>
		requires(sizeof(SignedInteger) <= 8)
	struct binary_reader<SignedInteger>
	{
		/// @brief Reads a signed integer from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Signed integer to output to.
		/// @exception binary_read_error If reading the signed integer failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, SignedInteger& output)
		{
			output = std::bit_cast<SignedInteger>(read_binary<std::make_unsigned_t<SignedInteger>>(source));
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for signed integers.
	/// @tparam UnsignedInteger Portable signed integer type.
	template <std::signed_integral SignedInteger>
		requires(sizeof(SignedInteger) <= 8)
	struct binary_writer<SignedInteger>
	{
		/// @brief Writes a signed integer to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Signed integer to write.
		/// @exception binary_write_error If writing the signed integer failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, SignedInteger input)
		{
			write_binary(sink, std::bit_cast<std::make_unsigned_t<SignedInteger>>(input));
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for floating-point numbers.
	/// @tparam FloatingPoint Portable floating-point number type.
	template <std::floating_point FloatingPoint>
		requires std::numeric_limits<FloatingPoint>::is_iec559 &&
				 (std::same_as<float, FloatingPoint> || std::same_as<double, FloatingPoint>)
	struct binary_reader<FloatingPoint>
	{
		/// @brief Reads a floating-point number from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Floating-point number to output to.
		/// @exception binary_read_error If reading the floating-point number failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, FloatingPoint& output)
		{
			output = std::bit_cast<FloatingPoint>(read_binary<std::conditional_t<std::same_as<FloatingPoint, float>, u32, u64>>(source));
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for floating-point numbers.
	/// @tparam FloatingPoint Portable floating-point number type.
	template <std::floating_point FloatingPoint>
		requires std::numeric_limits<FloatingPoint>::is_iec559 &&
				 (std::same_as<float, FloatingPoint> || std::same_as<double, FloatingPoint>)
	struct binary_writer<FloatingPoint>
	{
		/// @brief Writes a floating-point number to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Floating-point number to write.
		/// @exception binary_write_error If writing the floating-point number failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, FloatingPoint input)
		{
			write_binary(sink, std::bit_cast<std::conditional_t<std::same_as<FloatingPoint, float>, u32, u64>>(input));
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for enumerators.
	/// @tparam Enumerator Portable enumerator type.
	template <typename Enumerator>
		requires std::is_enum_v<Enumerator>
	struct binary_reader<Enumerator>
	{
		/// @brief Reads an enumerator from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Enumerator to output to.
		/// @exception binary_read_error If reading the enumerator failed.
		template <binary_source Source>
			requires binary_readable_with<std::underlying_type_t<Enumerator>&, Source>
		static constexpr void operator()(Source& source, Enumerator& output)
		{
			output = Enumerator{read_binary<std::underlying_type_t<Enumerator>>(source)};
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for enumerators.
	/// @tparam Enumerator Portable enumerator type.
	template <typename Enumerator>
		requires std::is_enum_v<Enumerator>
	struct binary_writer<Enumerator>
	{
		/// @brief Writes an enumerator to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Enumerator to write.
		/// @exception binary_write_error If writing the enumerator failed.
		template <binary_sink Sink>
			requires binary_writable_with<std::underlying_type_t<Enumerator>, Sink>
		static constexpr void operator()(Sink& sink, Enumerator input)
		{
			write_binary(sink, std::to_underlying(input));
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for vectors.
	/// @tparam Dimensions Number of vector dimensions.
	/// @tparam Rep Portable vector representation type.
	template <int Dimensions, typename Rep>
	struct binary_reader<glm::vec<Dimensions, Rep>>
	{
		/// @brief Reads a vector from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Vector to output to.
		/// @exception binary_read_error If reading the vector failed.
		template <binary_source Source>
			requires binary_readable_with<Rep&, Source>
		static constexpr void operator()(Source& source, glm::vec<Dimensions, Rep>& output)
		{
			for (int i = 0; i < Dimensions; ++i) {
				read_binary(source, output[i]);
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for vectors.
	/// @tparam Dimensions Number of vector dimensions.
	/// @tparam Rep Portable vector representation type.
	template <int Dimensions, typename Rep>
	struct binary_writer<glm::vec<Dimensions, Rep>>
	{
		/// @brief Writes a vector to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Vector to write.
		/// @exception binary_write_error If writing the vector failed.
		template <binary_sink Sink>
			requires binary_writable_with<Rep, Sink>
		static constexpr void operator()(Sink& sink, glm::vec<Dimensions, Rep> input)
		{
			for (int i = 0; i < Dimensions; ++i) {
				write_binary(sink, input[i]);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for matrices.
	/// @tparam Columns Number of matrix columns.
	/// @tparam Rows Number of matrix rows.
	/// @tparam Rep Portable matrix representation type.
	template <int Columns, int Rows, typename Rep>
	struct binary_reader<glm::mat<Columns, Rows, Rep>>
	{
		/// @brief Reads a matrix from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Matrix to output to.
		/// @exception binary_read_error If reading the matrix failed.
		template <binary_source Source>
			requires binary_readable_with<Rep&, Source>
		static constexpr void operator()(Source& source, glm::mat<Columns, Rows, Rep>& output)
		{
			for (int i = 0; i < Columns; ++i) {
				for (int j = 0; j < Rows; ++j) {
					read_binary(source, output[i][j]);
				}
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for matrices.
	/// @tparam Columns Number of matrix columns.
	/// @tparam Rows Number of matrix rows.
	/// @tparam Rep Portable matrix representation type.
	template <int Columns, int Rows, typename Rep>
	struct binary_writer<glm::mat<Columns, Rows, Rep>>
	{
		/// @brief Writes a matrix to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Matrix to write.
		/// @exception binary_write_error If writing the matrix failed.
		template <binary_sink Sink>
			requires binary_writable_with<Rep, Sink>
		static constexpr void operator()(Sink& sink, const glm::mat<Columns, Rows, Rep>& input)
		{
			for (int i = 0; i < Columns; ++i) {
				for (int j = 0; j < Rows; ++j) {
					write_binary(sink, input[i][j]);
				}
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for tuple-like objects.
	/// @tparam TupleLike Tuple-like type with portable elements.
	template <typename TupleLike>
		requires requires { std::tuple_size<TupleLike>::value; }
	struct binary_reader<TupleLike>
	{
	  private:
		/// @brief Reads a tuple-like object from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Tuple-like object to output to.
		/// @exception binary_read_error If reading the tuple-like object failed.
		template <binary_source Source, usize... Indices>
			requires(binary_readable_with<std::tuple_element_t<Indices, TupleLike>&, Source> && ...)
		static constexpr void operator()(Source& source, TupleLike& output, std::index_sequence<Indices...>)
		{
			using std::get;
			read_binary(source, get<Indices>(output)...);
		}

	  public:
		/// @brief Reads a tuple-like object from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Tuple-like object to output to.
		/// @exception binary_read_error If reading the tuple-like object failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, TupleLike& output)
		{
			binary_reader<TupleLike>{}(source, output, std::make_index_sequence<std::tuple_size_v<TupleLike>>{});
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for tuple-like objects.
	/// @tparam TupleLike Tuple-like type with portable elements.
	template <typename TupleLike>
		requires requires { std::tuple_size<TupleLike>::value; }
	struct binary_writer<TupleLike>
	{
	  private:
		/// @brief Writes a tuple-like object to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Tuple-like object to write.
		/// @exception binary_write_error If writing the tuple-like object failed.
		template <binary_sink Sink, usize... Indices>
			requires(binary_writable_with<const std::tuple_element_t<Indices, TupleLike>&, Sink> && ...)
		static constexpr void operator()(Sink& sink, const TupleLike& input, std::index_sequence<Indices...>)
		{
			using std::get;
			write_binary(sink, get<Indices>(input)...);
		}

	  public:
		/// @brief Writes a tuple-like object to a binary sink.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Tuple-like object to write.
		/// @exception binary_write_error If writing the tuple-like object failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, const TupleLike& input)
		{
			binary_writer<TupleLike>{}(sink, input, std::make_index_sequence<std::tuple_size_v<TupleLike>>{});
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for spans.
	/// @tparam Element Binary readable element type.
	/// @tparam Extent Extent of the span.
	template <typename Element, std::size_t Extent>
	struct binary_reader<std::span<Element, Extent>>
	{
		/// @brief Reads a span from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Span to output to.
		/// @exception binary_read_error If reading the span failed.
		template <binary_source Source>
			requires binary_readable_with<Element&, Source>
		static constexpr void operator()(Source& source, std::span<Element, Extent> output)
		{
			for (Element& element : output) {
				read_binary(source, element);
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for spans.
	/// @tparam Element Binary readable element type.
	/// @tparam Extent Extent of the span.
	template <typename Element, std::size_t Extent>
	struct binary_writer<std::span<Element, Extent>>
	{
		/// @brief Writes a span to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Span to write.
		/// @exception binary_write_error If writing the span failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Element&, Sink>
		static constexpr void operator()(Sink& sink, std::span<Element, Extent> input)
		{
			for (const Element& element : input) {
				write_binary(sink, element);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for strings.
	template <>
	struct binary_reader<std::string>
	{
		/// @brief Reads a string from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output String to output to.
		/// @exception binary_read_error If reading the string failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, std::string& output)
		{
			output.resize(read_binary<u32>(source));
			read_binary(source, std::span{output});
		}
	};

	/// @ingroup binary_io
	/// @brief Binary reader for inplace strings.
	/// @tparam Capacity Maximum capacity of the string.
	template <usize Capacity>
	struct binary_reader<inplace_string<Capacity>>
	{
		/// @brief Reads an inplace string from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Inplace string to output to.
		/// @exception binary_read_error If reading the inplace string failed.
		template <binary_source Source>
		static constexpr void operator()(Source& source, inplace_string<Capacity>& output)
		{
			output.resize(static_cast<typename inplace_string<Capacity>::size_type>(read_binary<u32>(source)));
			read_binary(source, std::span{output});
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for string views.
	template <>
	struct binary_writer<std::string_view>
	{
		/// @brief Writes a string to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input String to write.
		/// @exception binary_write_error If writing the string failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, std::string_view input)
		{
			write_binary(sink, static_cast<u32>(input.size()), std::span<const char>{input});
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for string literals.
	/// @tparam Size Size of the string literal.
	template <usize Size>
	struct binary_writer<char[Size]>
	{
		/// @brief Writes a string to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input String literal to write.
		/// @exception binary_write_error If writing the string literal failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, const char (&input)[Size])
		{
			write_binary(sink, std::string_view{input});
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for strings.
	template <>
	struct binary_writer<std::string>
	{
		/// @brief Writes a string to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input String to write.
		/// @exception binary_write_error If writing the string failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, const std::string& input)
		{
			write_binary(sink, std::string_view{input});
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for inplace strings.
	/// @tparam Capacity Maximum capacity of the string.
	template <usize Capacity>
	struct binary_writer<inplace_string<Capacity>>
	{
		/// @brief Writes an inplace string to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input String to write.
		/// @exception binary_write_error If writing the inplace string failed.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, const inplace_string<Capacity>& input)
		{
			write_binary(sink, std::string_view{input});
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for vectors.
	/// @tparam Element Binary constructible element type.
	template <typename Element>
	struct binary_reader<std::vector<Element>>
	{
		/// @brief Reads a vector from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Vector to output to.
		/// @exception binary_read_error If reading the vector failed.
		template <binary_source Source>
			requires binary_constructible_with<Element, Source>
		static constexpr void operator()(Source& source, std::vector<Element>& output)
		{
			output.resize(read_binary<u32>(source));
			read_binary(source, std::span{output});
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for vectors.
	/// @tparam Element Binary writable element type.
	template <typename Element>
	struct binary_writer<std::vector<Element>>
	{
		/// @brief Writes a vector to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Vector to write.
		/// @exception binary_write_error If writing the vector failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Element&, Sink>
		static constexpr void operator()(Sink& sink, const std::vector<Element>& input)
		{
			write_binary(sink, static_cast<u32>(input.size()), std::span{input});
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for sets.
	/// @tparam Key Binary constructible key type.
	template <typename Key>
	struct binary_reader<std::set<Key>>
	{
		/// @brief Reads a set from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Set to output to.
		/// @exception binary_read_error If reading the set failed.
		template <binary_source Source>
			requires binary_constructible_with<Key, Source>
		static constexpr void operator()(Source& source, std::set<Key>& output)
		{
			const u32 size{read_binary<u32>(source)};
			output.clear();
			for (u32 i = 0; i < size; ++i) {
				output.insert(read_binary<Key>(source));
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for sets.
	/// @tparam Key Binary writable key type.
	template <typename Key>
	struct binary_writer<std::set<Key>>
	{
		/// @brief Writes a set to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Set to write.
		/// @exception binary_write_error If writing the set failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Key&, Sink>
		static constexpr void operator()(Sink& sink, const std::set<Key>& input)
		{
			write_binary(sink, static_cast<u32>(input.size()));
			for (const Key& key : input) {
				write_binary(sink, key);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for unordered flat sets.
	/// @tparam Key Binary constructible key type.
	template <typename Key>
	struct binary_reader<boost::unordered_flat_set<Key>>
	{
		/// @brief Reads an unordered flat set from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Unordered flat set to output to.
		/// @exception binary_read_error If reading the unordered flat set failed.
		template <binary_source Source>
			requires binary_constructible_with<Key, Source>
		static constexpr void operator()(Source& source, boost::unordered_flat_set<Key>& output)
		{
			const u32 size{read_binary<u32>(source)};
			output.clear();
			output.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				output.insert(read_binary<Key>(source));
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for unordered flat sets.
	/// @tparam Key Binary writable key type.
	template <typename Key>
	struct binary_writer<boost::unordered_flat_set<Key>>
	{
		/// @brief Writes a set to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Set to write.
		/// @exception binary_write_error If writing the set failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Key&, Sink>
		static constexpr void operator()(Sink& sink, const boost::unordered_flat_set<Key>& input)
		{
			write_binary(sink, static_cast<u32>(input.size()));
			for (const Key& key : input) {
				write_binary(sink, key);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for unordered node sets.
	/// @tparam Key Binary constructible key type.
	template <typename Key>
	struct binary_reader<boost::unordered_node_set<Key>>
	{
		/// @brief Reads an unordered node set from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Unordered node set to output to.
		/// @exception binary_read_error If reading the unordered node set failed.
		template <binary_source Source>
			requires binary_constructible_with<Key, Source>
		static constexpr void operator()(Source& source, boost::unordered_node_set<Key>& output)
		{
			const u32 size{read_binary<u32>(source)};
			output.clear();
			output.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				output.insert(read_binary<Key>(source));
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for unordered node sets.
	/// @tparam Key Binary writable key type.
	template <typename Key>
	struct binary_writer<boost::unordered_node_set<Key>>
	{
		/// @brief Writes an unordered node set to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Unordered node set to write.
		/// @exception binary_write_error If writing the unordered node set failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Key&, Sink>
		static constexpr void operator()(Sink& sink, const boost::unordered_node_set<Key>& input)
		{
			write_binary(sink, static_cast<u32>(input.size()));
			for (const Key& key : input) {
				write_binary(sink, key);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for maps.
	/// @tparam Key Binary constructible key type.
	/// @tparam Value Binary constructible value type.
	template <typename Key, typename Value>
	struct binary_reader<std::map<Key, Value>>
	{
		/// @brief Reads a map from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Map to output to.
		/// @exception binary_read_error If reading the map failed.
		template <binary_source Source>
			requires binary_constructible_with<Key, Source> && binary_constructible_with<Value, Source>
		static constexpr void operator()(Source& source, std::map<Key, Value>& output)
		{
			const u32 size{read_binary<u32>(source)};
			output.clear();
			for (u32 i = 0; i < size; ++i) {
				Key key{read_binary<Key>(source)};
				Value value{read_binary<Value>(source)};
				output.emplace(std::move(key), std::move(value));
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for maps.
	/// @tparam Key Binary writable key type.
	/// @tparam Value Binary writable value type.
	template <typename Key, typename Value>
	struct binary_writer<std::map<Key, Value>>
	{
		/// @brief Writes a map to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Map to write.
		/// @exception binary_write_error If writing the map failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Key&, Sink> && binary_writable_with<const Value&, Sink>
		static constexpr void operator()(Sink& sink, const std::map<Key, Value>& input)
		{
			write_binary(sink, static_cast<u32>(input.size()));
			for (const auto& [key, value] : input) {
				write_binary(sink, key, value);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for unordered flat maps.
	/// @tparam Key Binary constructible key type.
	/// @tparam Value Binary constructible value type.
	template <typename Key, typename Value>
	struct binary_reader<boost::unordered_flat_map<Key, Value>>
	{
		/// @brief Reads an unordered flat map from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Unordered flat map to output to.
		/// @exception binary_read_error If reading the unordered flat map failed.
		template <binary_source Source>
			requires binary_constructible_with<Key, Source> && binary_constructible_with<Value, Source>
		static constexpr void operator()(Source& source, boost::unordered_flat_map<Key, Value>& output)
		{
			const u32 size{read_binary<u32>(source)};
			output.clear();
			output.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				Key key{read_binary<Key>(source)};
				Value value{read_binary<Value>(source)};
				output.emplace(std::move(key), std::move(value));
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for unordered flat maps.
	/// @tparam Key Binary writable key type.
	/// @tparam Value Binary writable value type.
	template <typename Key, typename Value>
	struct binary_writer<boost::unordered_flat_map<Key, Value>>
	{
		/// @brief Writes an unordered flat map to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Unordered flat map to write.
		/// @exception binary_write_error If writing the unordered flat map failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Key&, Sink> && binary_writable_with<const Value&, Sink>
		static constexpr void operator()(Sink& sink, const boost::unordered_flat_map<Key, Value>& input)
		{
			write_binary(sink, static_cast<u32>(input.size()));
			for (const auto& [key, value] : input) {
				write_binary(sink, key, value);
			}
		}
	};

	//

	/// @ingroup binary_io
	/// @brief Binary reader for unordered node maps.
	/// @tparam Key Binary constructible key type.
	/// @tparam Value Binary constructible value type.
	template <typename Key, typename Value>
	struct binary_reader<boost::unordered_node_map<Key, Value>>
	{
		/// @brief Reads an unordered node map from a binary source.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Unordered node map to output to.
		/// @exception binary_read_error If reading the unordered node map failed.
		template <binary_source Source>
			requires binary_constructible_with<Key, Source> && binary_constructible_with<Value, Source>
		static constexpr void operator()(Source& source, boost::unordered_node_map<Key, Value>& output)
		{
			const u32 size{read_binary<u32>(source)};
			output.clear();
			output.reserve(size);
			for (u32 i = 0; i < size; ++i) {
				Key key{read_binary<Key>(source)};
				Value value{read_binary<Value>(source)};
				output.emplace(std::move(key), std::move(value));
			}
		}
	};

	/// @ingroup binary_io
	/// @brief Binary writer for unordered node maps.
	/// @tparam Key Binary writable key type.
	/// @tparam Value Binary writable value type.
	template <typename Key, typename Value>
	struct binary_writer<boost::unordered_node_map<Key, Value>>
	{
		/// @brief Writes an unordered node map to a binary source.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Unordered node map to write.
		/// @exception binary_write_error If writing the unordered node map failed.
		template <binary_sink Sink>
			requires binary_writable_with<const Key&, Sink> && binary_writable_with<const Value&, Sink>
		static constexpr void operator()(Sink& sink, const boost::unordered_node_map<Key, Value>& input)
		{
			write_binary(sink, static_cast<u32>(input.size()));
			for (const auto& [key, value] : input) {
				write_binary(sink, key, value);
			}
		}
	};
} // namespace tr