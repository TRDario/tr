/// @file
/// @brief Provides `tr::timer_query`.

#pragma once
#include <tr/utility/chrono.hpp>
#include <tr/utility/ref.hpp>
#include <tr/utility/unique_handle.hpp>

namespace tr
{
	class graphics_context;
} // namespace tr

//

namespace tr
{
	/// @brief Owning container for a graphics driver timer query.
	/// @details
	/// Wrapping a sequence of graphics instructions with a timer query's `begin()` and `end()` allows measuring the total time needed to
	/// execute said commands on the GPU side.
	///
	/// Every instance of `tr::timer_query` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::timer_query` are left in a special 'invalid' state. Invalid `tr::timer_query` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()` or `operator bool()`.
	///
	/// `tr::timer_query` instances may be labeled and are formattable. Example format output: `"My query" (OpenGL ID: 5)`.
	class timer_query
	{
	  public:
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs a timer query.
		/// @param context Context to create the timer query on.
		[[nodiscard]] explicit timer_query(graphics_context& context) noexcept;

		/// @brief Timer queries are not copyable.
		timer_query(const timer_query&) = delete;

		/// @brief Timer queries are not copyable.
		timer_query& operator=(const timer_query&) = delete;

		/// @brief Constructs a timer query by transferring ownership from another `tr::timer_query`.
		/// @param rhs Timer query to move from.
		/// @post `rhs` is left in an invalid state after the move.
		[[nodiscard]] timer_query(timer_query&& rhs) noexcept = default;

		/// @brief Destroys the currently held timer query and replaces it with one moved from another `tr::timer_query`.
		/// @param rhs Timer query to move from.
		/// @post `rhs` is left in an invalid state after the move.
		/// @return Reference to `*this`.
		timer_query& operator=(timer_query&& rhs) noexcept = default;

		/// @}
		/// @name Markers
		/// @{

		/// @brief Marks the beginning of a queried section of graphics instructions.
		/// @pre
		/// The timer query must be in a valid state.
		///
		/// `begin()` and `end()` must always be called as a pair.
		void begin() noexcept;

		/// @brief Marks the end of a queried section of graphics instructions.
		/// @pre
		/// The timer query must be in a valid state.
		///
		/// `begin()` and `end()` must always be called as a pair.
		void end() noexcept;

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Sets the label of the timer query.
		/// @param label Label of the timer query.
		/// @pre The timer query must be in a valid state.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @name Observers
		/// @{

		/// @brief Checks whether the timer query is in a valid state.
		/// @return `true` if the timer query is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] explicit operator bool() const noexcept;

		/// @brief Checks whether the timer query is in a valid state.
		/// @return `true` if the timer query is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @brief Returns a reference to the graphics context the timer query is on.
		/// @pre The timer query must be in a valid state.
		/// @return Reference to the graphics context the timer query is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @brief Check whether the timer query holds a value.
		/// @pre The timer query must be in a valid state.
		/// @return `true` if the timer query holds a value, `false` otherwise.
		[[nodiscard]] bool has_value() const noexcept;

		/// @brief Returns the value held by a timer query.
		/// @note If necessary, this function will wait for the value to become available. If you wish to prevent blocking, check
		/// if `has_value()` returns `true` first.
		/// @pre The timer query must be in a valid state.
		/// @return Measured time value held by the timer query.
		[[nodiscard]] nsecs<i64> value() const noexcept;

		/// @brief Returns the label of the timer query.
		/// @pre The timer query must be in a valid state.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Label of the timer query.
		[[nodiscard]] std::string label() const;

		/// @cond gl_interop
		/// @brief Returns a handle to the underlying OpenGL query object.
		/// @return Handle to the underlying OpenGL query object.
		[[nodiscard]] unsigned int handle() const noexcept;
		/// @endcond

		/// @}

	  private:
		/// @brief OpenGL query object deleter.
		struct query_object_deleter
		{
			/// @brief Reference to the graphics context the query object is on.
			ref<graphics_context> context;

			//

			/// @brief Deletes a query object.
			/// @param handle OpenGL query object handle.
			void operator()(unsigned int handle) const noexcept;
		};

		//

		/// @brief Managed handle to an OpenGL query object.
		unique_handle<unsigned int, 0, query_object_deleter> m_handle;
	};
} // namespace tr

//

/// @brief Timer query formatter.
template <>
struct std::formatter<tr::timer_query>
{
	/// @brief Parses the format specification.
	/// @tparam ParseContext Parsing context type.
	/// @param context Parsing context.
	/// @return Iterator to the end of the parsed specification.
	template <typename ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& context)
	{
		if (context.begin() != context.end() && *context.begin() != '}') {
			throw std::format_error{"Invalid timer query format specification."};
		}
		return context.begin();
	}

	/// @brief Formats a timer query.
	/// @tparam FormatContext Formatting context type.
	/// @param query Timer query to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::timer_query& query, FormatContext& context) const
	{
		if (query) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", query.label(), query.handle());
		}
		else {
			return std::format_to(context.out(), "<invalid timer query at {}>", static_cast<const void*>(&query));
		}
	}
};