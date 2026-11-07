/// @file
/// @brief Provides an interface for reading and writing objects in a portable and compact binary format.

/// @ingroup utility
/// @defgroup binary_io Binary reading and writing
/// @brief Contains an interface for reading and writing objects in a portable and compact binary format.

#pragma once
#include <tr/utility/binary_sink.hpp>
#include <tr/utility/binary_source.hpp>
#include <tr/utility/ranges.hpp>
#include <tr/utility/specialization_of.hpp>

//

namespace tr
{
	/// @ingroup binary_io
	/// @brief Error thrown when a binary read fails.
	class binary_read_error : public exception
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a binary read error.
		/// @param start_position Starting position of the read that threw the error.
		/// @param expected_bytes Number of bytes expected to be read.
		/// @param read_bytes Number of bytes actually read.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] binary_read_error(usize start_position, usize expected_bytes, usize read_bytes);

		/// @}
		/// @name Observers
		/// @{

		/// @brief Returns the name of the error.
		/// @return Always `"Binary read error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// @brief Returns the description of the error.
		/// @return String containing the error description.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// @brief Returns further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @brief Returns the starting position of the read that threw the error.
		/// @return Starting position of the read that threw the error.
		[[nodiscard]] usize start_position() const noexcept;

		/// @brief Returns the number of bytes expected to be read.
		/// @return Number of bytes expected to be read.
		[[nodiscard]] usize expected_bytes() const noexcept;

		/// @brief Returns the number of bytes actually read.
		/// @return Number of bytes actually read.
		[[nodiscard]] usize read_bytes() const noexcept;

		/// @}

	  private:
		/// @brief Starting position of the read that threw the error.
		usize m_start_position;

		/// @brief Number of bytes expected to be read.
		usize m_expected_bytes;

		/// @brief Number of bytes actually read.
		usize m_read_bytes;

		/// @brief Description of the error.
		std::string m_description;
	};

	/// @ingroup binary_io
	/// @brief Error thrown when a binary write fails.
	class binary_write_error : public exception
	{
	  public:
		/// @brief Constructs a binary write error.
		/// @param start_position Starting position of the write that triggered the error.
		/// @param expected_bytes Number of bytes expected to be written.
		/// @param written_bytes Number of bytes actually written.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		binary_write_error(usize start_position, usize expected_bytes, usize written_bytes);

		/// @name Observers
		/// @{

		/// @brief Returns the name of the error.
		/// @return Always `"Binary write error"`.
		[[nodiscard]] std::string_view name() const noexcept override;

		/// @brief Returns the description of the error.
		/// @return String containing the error description.
		[[nodiscard]] std::string_view description() const noexcept override;

		/// @brief Returns further details about the error.
		/// @return Always empty.
		[[nodiscard]] std::string_view details() const noexcept override;

		/// @brief Returns the starting position of the write that threw the error.
		/// @return Starting position of the write that threw the error.
		[[nodiscard]] usize start_position() const noexcept;

		/// @brief Returns the number of bytes expected to be written.
		/// @return Number of bytes expected to be written.
		[[nodiscard]] usize expected_bytes() const noexcept;

		/// @brief Returns the number of bytes actually written.
		/// @return Number of bytes actually written.
		[[nodiscard]] usize written_bytes() const noexcept;

		/// @}

	  private:
		/// @brief Starting position of the write that threw the error.
		usize m_start_position;

		/// @brief Number of bytes expected to be written.
		usize m_expected_bytes;

		/// @brief Number of bytes actually written.
		usize m_written_bytes;

		/// @brief Description of the error.
		std::string m_description;
	};

	//

	/// @ingroup binary_io
	/// @brief Interface for custom readers for use in `tr::read_binary()`.
	/// @tparam Output Output object type.
	/// @note Any overloads to const or reference-qualified types will be ignored.
	template <typename Output>
	struct binary_reader
	{
		static_assert(false, "tr::binary_reader is not specialized for this type!");

#ifdef TR_DOXYGEN
		/// @brief Reads a value from a binary source.
		/// @details It's recommended to throw `tr::binary_read_error` on failed reads.
		/// @tparam Source Binary source type.
		/// @param source Binary source to read from.
		/// @param output Object to output to.
		template <binary_source Source>
		static constexpr void operator()(Source& source, Output& output);
#endif
	};

	/// @ingroup binary_io
	/// @brief Interface for custom writers for use in `tr::write_binary()`.
	/// @tparam Input Input object type.
	/// @note Any overloads to const or reference-qualified types will be ignored.
	template <typename Input>
	struct binary_writer
	{
		static_assert(false, "tr::binary_writer is not specialized for this type!");

#ifdef TR_DOXYGEN
		/// @brief Writes a value to a binary sink.
		/// @details It's recommended to throw `tr::binary_write_error` on failed writes.
		/// @tparam Sink Binary sink type.
		/// @param sink Binary sink to write to.
		/// @param input Object to write.
		template <binary_sink Sink>
		static constexpr void operator()(Sink& sink, const Input& input);
#endif
	};

	//

	/// @ingroup binary_io
	/// @brief Concept checking for a type readable with `tr::read_binary()` using a source of type `Source`.
	/// @tparam Target Type to check for readility with `tr::read_binary()` using a source of type `Source`.
	/// @tparam Source Binary source type being used.
	template <typename Target, typename Source>
	concept binary_readable_with = requires(Source&& source, Target&& output) {
		tr::binary_reader<std::remove_cvref_t<Target>>{}(source, std::forward<Target>(output));
	};

	/// @ingroup binary_io
	/// @brief Concept checking for a type constructible with `tr::read_binary()` using a source of type `Source`.
	/// @tparam Target Type to check for constructibility with `tr::read_binary()` using a source of type `Source`.
	/// @tparam Source Binary source type being used.
	template <typename Target, typename Source>
	concept binary_constructible_with = binary_readable_with<Target&, Source> && std::default_initializable<Target>;

	/// @ingroup binary_io
	/// @brief Concept checking for a type writable with `tr::write_binary()` using a sink of type `Sink`.
	/// @tparam Target Type to check for writability with `tr::write_binary()` using a sink of type `Sink`.
	/// @tparam Sink Binary sink type being used.
	template <typename Target, typename Sink>
	concept binary_writable_with = requires(Sink&& sink, Target&& input) {
		tr::binary_writer<std::remove_cvref_t<Target>>{}(sink, std::forward<Target>(input));
	};

	//

	/// @ingroup binary_io
	/// @brief Reads objects encoded in a binary format from a source.
	/// @tparam Source Binary source type.
	/// @tparam Outputs Output object types.
	/// @param source Binary source to read from.
	/// @param outputs Objects to read to.
	/// @exception binary_read_error If reading an object failed.
	template <binary_source Source, binary_readable_with<Source>... Outputs>
		requires(sizeof...(Outputs) >= 1)
	constexpr void read_binary(Source&& source, Outputs&&... outputs)
	{
		(tr::binary_reader<std::remove_cvref_t<Outputs>>{}(source, std::forward<Outputs>(outputs)), ...);
	}

	/// @ingroup binary_io
	/// @brief Reads an object encoded in a binary format from a source.
	/// @tparam Source Binary source type.
	/// @tparam Output Output object type.
	/// @param source Binary source to read from.
	/// @exception binary_read_error If reading the object failed.
	/// @return Read object.
	template <typename Output, binary_source Source>
		requires binary_constructible_with<Output, Source>
	[[nodiscard]] constexpr Output read_binary(Source&& source)
	{
		Output out;
		read_binary(source, out);
		return out;
	}

	/// @ingroup binary_io
	/// @brief Checks for magic bytes.
	/// @tparam Source Binary source type.
	/// @tparam Size Size of the magic byte string literal.
	/// @param source Binary source.
	/// @param magic Magic bytes to match.
	/// @return `true` if the magic bytes were matched, `false` otherwise.
	template <binary_source Source, usize Size>
	[[nodiscard]] constexpr bool check_binary_magic(Source&& source, const char (&magic)[Size])
	{
		std::array<char, Size - 1> buffer;
		read_binary(source, as_mut_bytes(buffer));
		return std::string_view{buffer.data(), Size - 1} == std::string_view{magic};
	}

	//

	/// @ingroup binary_io
	/// @brief Writes an object encoded in a binary format to a sink.
	/// @tparam Sink Binary sink type.
	/// @tparam Inputs Input object types.
	/// @param sink Binary sink to write to.
	/// @param inputs Objects to write.
	/// @exception binary_write_error If writing an object failed.
	template <binary_sink Sink, binary_writable_with<Sink>... Inputs>
	constexpr void write_binary(Sink&& sink, Inputs&&... inputs)
	{
		(tr::binary_writer<std::remove_cvref_t<Inputs>>{}(sink, std::forward<Inputs>(inputs)), ...);
	}

	/// @ingroup binary_io
	/// @brief Writes magic bytes to a sink.
	/// @tparam Sink Binary sink type.
	/// @param sink Binary sink to write to.
	/// @param magic Magic bytes to output.
	/// @exception binary_write_error If writing the magic bytes failed.
	template <binary_sink Sink>
	constexpr void write_binary_magic(Sink&& sink, std::string_view magic)
	{
		const usize start_position{sink.position()};
		const usize written_bytes{sink.write(range_bytes(magic))};
		if (written_bytes != magic.size()) {
			throw binary_write_error{start_position, magic.size(), written_bytes};
		}
	}

} // namespace tr