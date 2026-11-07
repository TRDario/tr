/// @file
/// @brief Provides binary sinks.

#pragma once
#include <tr/utility/iostream.hpp>

//

namespace tr
{
	/// @brief Concept checking for a binary sink passable to binary writing functions.
	/// @details To fulfill this concept, a type must have a `write()` method that writes bytes from a span and returns the number of
	/// written bytes, as well as a `position()` method that returns the current write head position within the sink.
	/// @tparam Target Type to check for being a binary sink.
	template <typename Target>
	concept binary_sink = requires(Target sink, std::span<const std::byte> source) {
		{ sink.write(source) } -> std::same_as<usize>;
		{ sink.position() } -> std::same_as<usize>;
	};

	//

	/// @brief Binary sink backed by a span of bytes.
	class span_binary_sink
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a span binary sink.
		/// @param span Destination byte span.
		[[nodiscard]] constexpr span_binary_sink(std::span<std::byte> span) noexcept
			: m_span{span}
			, m_position{0}
		{
		}

		/// @brief Constructs a copy of a span binary source.
		/// @details The sink will be constructed with the same destination span and at the same position as `rhs`.
		/// @param rhs Span binary sink to copy from.
		[[nodiscard]] constexpr span_binary_sink(const span_binary_sink& rhs) noexcept = default;

		/// @brief Replaces the span binary source with a copy of another.
		/// @details The sink will be set with the same destination span and at the same position as `rhs`.
		/// @param rhs Span binary sink to copy from.
		/// @return Reference to `*this`.
		[[nodiscard]] constexpr span_binary_sink& operator=(const span_binary_sink& rhs) noexcept = default;

		/// @}
		/// @name Binary sink methods
		/// @{

		/// @brief Writes bytes from a source span to the sink.
		/// @param source Source byte buffer.
		/// @return Number of bytes written to the sink. May be less than or equal to `source.size()`.
		constexpr usize write(std::span<const std::byte> source) noexcept
		{
			const usize bytes_written{std::min(source.size(), m_span.size() - m_position)};
			std::ranges::copy(source.begin(), source.begin() + bytes_written, m_span.begin() + m_position);
			m_position += bytes_written;
			return bytes_written;
		}

		/// @brief Returns the current write head position within the sink.
		/// @return Current write head position within the sink.
		[[nodiscard]] constexpr usize position() const noexcept
		{
			return m_position;
		}

		/// @}
	  private:
		/// @brief Destination byte span.
		std::span<std::byte> m_span;

		/// @brief Current position within the destination span.
		usize m_position;
	};

	/// @brief Binary sink backed by a byte vector.
	class vector_binary_sink
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a vector binary sink.
		[[nodiscard]] constexpr vector_binary_sink() noexcept = default;

		/// @brief Constructs a vector binary sink with a vector copied from another sink.
		/// @param rhs Vector binary sink to copy from.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (basic exception guarantee).
		[[nodiscard]] constexpr vector_binary_sink(const vector_binary_sink& rhs) = default;

		/// @brief Replaces the vector of the sink with a copy of a vector of another sink.
		/// @param rhs Vector binary sink to copy from.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return Reference to `*this`.
		[[nodiscard]] constexpr vector_binary_sink& operator=(const vector_binary_sink& rhs) = default;

		/// @brief Constructs a vector binary sink with a vector moved from another sink.
		/// @param rhs Vector binary sink to move from.
		/// @post `rhs` will be left with a vector in a valid, but unspecified state.
		[[nodiscard]] constexpr vector_binary_sink(vector_binary_sink&& rhs) noexcept = default;

		/// @brief Replaces the vector of the sink with one moved from another sink.
		/// @param rhs Vector binary sink to move from.
		/// @post `rhs` will be left with a vector in a valid, but unspecified state.
		/// @return Reference to `*this`.
		[[nodiscard]] constexpr vector_binary_sink& operator=(vector_binary_sink&& rhs) noexcept = default;

		/// @}
		/// @name Binary sink methods
		/// @{

		/// @brief Writes bytes from a source span to the sink.
		/// @param source Source byte buffer.
		/// @return Number of bytes written to the sink. May be less than or equal to `source.size()`.
		constexpr usize write(std::span<const std::byte> source) noexcept
		{
			try {
				m_vector.resize(m_vector.size() + source.size());
			}
			catch (...) {
				return 0;
			}

			std::ranges::copy(source, m_vector.end() - source.size());
			return source.size();
		}

		/// @brief Returns the current write head position within the sink.
		/// @return Current write head position within the sink.
		[[nodiscard]] constexpr usize position() const noexcept
		{
			return m_vector.size();
		}

		/// @}
		/// @name Vector operations
		/// @{

		/// @brief Returns a reference to the vector backing the sink.
		/// @return Reference to the vector backing the sink.
		template <typename Self>
		[[nodiscard]] constexpr decltype(auto) vector(this Self&& self) noexcept
		{
			return std::forward_like<Self>(self.m_vector);
		}

		/// @}
	  private:
		/// @brief Destination byte vector.
		std::vector<std::byte> m_vector;
	};

	/// @brief Binary sink that writes to a file.
	class file_binary_sink
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Opens a binary destination file.
		/// @param path Path to the destination file.
		/// @exception file_open_error If opening the destination file failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] file_binary_sink(const std::filesystem::path& path);

		/// @brief File binary sinks are not copyable.
		file_binary_sink(const file_binary_sink&) = delete;

		/// @brief File binary sinks are not copyable.
		file_binary_sink& operator=(const file_binary_sink&) = delete;

		/// @brief Transfers ownership over an opened destination file from `rhs` (if one is open).
		/// @param rhs File binary sink to transfer ownership from.
		/// @post `rhs` is left without an open file.
		[[nodiscard]] file_binary_sink(file_binary_sink&& rhs) noexcept;

		/// @brief Closes the currently open file (if one is open) and transfers ownership over an opened destination file from `rhs` (if
		/// one is open).
		/// @param rhs File binary sink to transfer ownership from.
		/// @post `rhs` is left without an open file.
		/// @return Reference to `*this`.
		file_binary_sink& operator=(file_binary_sink&& rhs) noexcept;

		/// @brief Closes the associated destination file, if one is open.
		~file_binary_sink() noexcept;

		/// @}
		/// @name Binary sink methods
		/// @{

		/// @brief Writes bytes from a source span to the sink.
		/// @param source Source byte buffer.
		/// @return Number of bytes written to the sink. May be less than or equal to than `source.size()`.
		usize write(std::span<const std::byte> source) noexcept;

		/// @brief Returns the current write head position within the sink.
		/// @return Current write head position within the sink.
		[[nodiscard]] usize position() const noexcept;

		/// @}
		/// @name File operations
		/// @{

		/// @brief Checks if a destination file is open.
		/// @return `true` if a destination file is open, `false` otherwise.
		[[nodiscard]] operator bool() const noexcept;

		/// @brief Checks if a destination file is open.
		/// @return `true` if a destination file is open, `false` otherwise.
		[[nodiscard]] bool is_open() const noexcept;

		/// @brief Opens a new destination file, replacing the currently open one if necessary.
		/// @param path Path to the destination file.
		/// @exception file_open_error If opening the destination file failed.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void open(const std::filesystem::path& path);

		/// @brief Closes the associated destination file, if one is open.
		void close() noexcept;

		/// @}
	  private:
		/// @brief Destination file pointer.
		std::FILE* m_file;

		/// @brief Current position within the destination file.
		usize m_position;
	};
} // namespace tr