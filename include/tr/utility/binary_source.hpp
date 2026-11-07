/// @file
/// @brief Provides binary sources.

#pragma once
#include <tr/utility/iostream.hpp>

//

namespace tr
{
	/// @brief Concept checking for a binary source passable to binary reading functions.
	/// @details To fulfill this concept, a type must have a `read()` method that reads bytes into a span and returns the number of read
	/// bytes, as well as a `position()` method that returns the current read head position within the source. As an additional constraint
	/// that is not checked for here, the source may never read more than `destination.size()` bytes.
	/// @tparam Target Type to check for being a binary source.
	template <typename Target>
	concept binary_source = requires(Target source, std::span<std::byte> destination) {
		{ source.read(destination) } -> std::same_as<usize>;
		{ source.position() } -> std::same_as<usize>;
	};

	//

	/// @brief Binary source backed by a span of bytes.
	class span_binary_source
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a span binary source.
		/// @param span Source byte span.
		[[nodiscard]] constexpr span_binary_source(std::span<const std::byte> span) noexcept
			: m_span{span}
			, m_position{0}
		{
		}

		/// @brief Constructs a copy of a span binary source.
		/// @details The source will be constructed with the same source span and at the same position as `rhs`.
		/// @param rhs Span binary source to copy from.
		[[nodiscard]] constexpr span_binary_source(const span_binary_source& rhs) noexcept = default;

		/// @brief Replaces the span binary source with a copy of another.
		/// @details The source will be set with the same source span and at the same position as `rhs`.
		/// @param rhs Span binary source to copy from.
		/// @return Reference to `*this`.
		[[nodiscard]] constexpr span_binary_source& operator=(const span_binary_source& rhs) noexcept = default;

		/// @}
		/// @name Binary source methods
		/// @{

		/// @brief Reads bytes from the source into a destination buffer.
		/// @param destination Destination byte buffer.
		/// @return Number of bytes read into the destination. May be less than or equal to `destination.size()`.
		constexpr usize read(std::span<std::byte> destination) noexcept
		{
			const usize bytes_read{std::min(destination.size(), m_span.size() - m_position)};
			std::ranges::copy(m_span.subspan(m_position, bytes_read), destination.begin());
			m_position += bytes_read;
			return bytes_read;
		}

		/// @brief Returns the current read head position within the source.
		/// @return Current read head position within the source.
		[[nodiscard]] constexpr usize position() const noexcept
		{
			return m_position;
		}

		/// @}

	  private:
		/// @brief Source byte span.
		std::span<const std::byte> m_span;

		/// @brief Current position within the source span.
		usize m_position;
	};

	/// @brief Binary source that reads from a file.
	class file_binary_source
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Opens a binary source file.
		/// @param path Path to the source file.
		/// @exception file_not_found If the source file was not found.
		/// @exception file_open_error If opening the source file failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] file_binary_source(const std::filesystem::path& path);

		/// @brief File binary sources are not copyable.
		file_binary_source(const file_binary_source&) = delete;

		/// @brief File binary sources are not copyable.
		file_binary_source& operator=(const file_binary_source&) = delete;

		/// @brief Transfers ownership over an opened source file from `rhs` (if one is open).
		/// @param rhs File binary source to transfer ownership from.
		/// @post `rhs` is left without an open file.
		[[nodiscard]] file_binary_source(file_binary_source&& rhs) noexcept;

		/// @brief Closes the currently open file (if one is open) and transfers ownership over an opened source file from `rhs` (if one is
		/// open).
		/// @param rhs File binary source to transfer ownership from.
		/// @post `rhs` is left without an open file.
		/// @return Reference to `*this`.
		file_binary_source& operator=(file_binary_source&& rhs) noexcept;

		/// @brief Closes the associated source file, if one is open.
		~file_binary_source() noexcept;

		/// @}
		/// @name Binary source methods
		/// @{

		/// @brief Reads bytes from the source into a destination buffer.
		/// @param destination Destination byte buffer.
		/// @return Number of bytes read into the destination. May be less than or equal to `destination.size()`.
		usize read(std::span<std::byte> destination) noexcept;

		/// @brief Returns the current read head position within the source.
		/// @return Current read head position within the source.
		[[nodiscard]] usize position() const noexcept;

		/// @}
		/// @name File operations
		/// @{

		/// @brief Checks if a source file is open.
		/// @return `true` if a source file is open, `false` otherwise.
		[[nodiscard]] operator bool() const noexcept;

		/// @brief Checks if a source file is open.
		/// @return `true` if a source file is open, `false` otherwise.
		[[nodiscard]] bool is_open() const noexcept;

		/// @brief Opens a new source file, replacing the currently open one if necessary.
		/// @param path Path to the source file.
		/// @exception file_not_found If the source file was not found.
		/// @exception file_open_error If opening the source file failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void open(const std::filesystem::path& path);

		/// @brief Closes the associated source file, if one is open.
		void close() noexcept;

		/// @}
	  private:
		/// @brief Source file pointer.
		std::FILE* m_file;

		/// @brief Current position within the source file.
		usize m_position;
	};
} // namespace tr