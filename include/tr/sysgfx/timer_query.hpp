/// @file
/// @brief Provides `tr::timer_query`.

#pragma once
#include <tr/utility/chrono.hpp>
#include <tr/utility/handle.hpp>
#include <tr/utility/integer.hpp>
#include <tr/utility/ref.hpp>

namespace tr
{
	class graphics_context;
} // namespace tr

//

namespace tr
{
	/// Object holding a graphics driver timer query.
	/// @details
	/// Wrapping a sequence of graphics instructions with a timer query's `begin()` and `end()` allows measuring the total time needed to
	/// execute said commands on the GPU side.
	///
	/// Every instance of `tr::timer_query` is associated with a graphics context and cannot outlive its parent context.
	///
	/// Moved-from instances of `tr::timer_query` are left in a special 'invalid' state. Invalid `tr::timer_query` instances may not be
	/// interacted with besides moving a new value into them and checking for validity using `valid()`.
	///
	/// `tr::timer_query` instances may be labeled and are formattable. Example format output: `"My query" (OpenGL ID: 5)`.
	class timer_query
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs a timer query.
		/// @param context Context to create the timer query on.
		[[nodiscard]] explicit timer_query(graphics_context& context) noexcept;

		/// @}
		/// @name Context
		/// @{

		/// Gets a reference to the graphics context the timer query is on.
		/// @return Reference to the graphics context the timer query is on.
		[[nodiscard]] graphics_context& context() const noexcept;

		/// @}
		/// @name Validity
		/// @{

		/// Gets whether the timer query is in a valid state.
		/// @return `true` if the timer query is in a valid state, `false` if it is in an invalid state.
		[[nodiscard]] bool valid() const noexcept;

		/// @}
		/// @name Markers
		/// @{

		/// Marks the beginning of a queried section of graphics instructions.
		/// @pre `begin()` and `end()` must always be called as a pair.
		void begin() noexcept;

		/// Marks the end of a queried section of graphics instructions.
		/// @pre `begin()` and `end()` must always be called as a pair.
		void end() noexcept;

		/// @}
		/// @name Value
		/// @{

		/// Gets whether the timer query holds a value.
		/// @return True if the timer query holds a value, false otherwise.
		[[nodiscard]] bool has_value() const noexcept;

		/// Gets the value held by a timer query.
		/// @note If necessary, this function will wait for the value to become available. If you wish to prevent blocking, check
		/// if `has_value()` returns `true` first.
		/// @return Measured time value held by the timer query.
		[[nodiscard]] nsecs<i64> value() const noexcept;

		/// @}
		/// @name Label
		/// @{

		/// Gets the debug label of the timer query.
		/// @return Debug label of the timer query.
		[[nodiscard]] std::string label() const;

		/// Sets the debug label of the timer query.
		/// @param label Debug label of the timer query.
		void set_label(std::string_view label) noexcept;

		/// @}
		/// @cond gl_interop
		/// @name OpenGL interoperability
		/// @{

		/// Unwraps the OpenGL query object.
		/// @note This does not release the query object.
		/// @return OpenGL query object.
		[[nodiscard]] unsigned int unwrap() const noexcept;

		/// @}
		/// @endcond

	  private:
		/// Query object deleter.
		struct deleter
		{
			/// Reference to the graphics context the query object is on.
			ref<graphics_context> context;

			//

			/// Deletes a query object.
			/// @param qo OpenGL query object ID.
			void operator()(unsigned int qo) const noexcept;
		};

		/// Handle to the OpenGL query object.
		handle<unsigned int, 0, deleter> m_handle;
	};
} // namespace tr

//

/// Timer query formatter.
template <>
struct std::formatter<tr::timer_query>
{
	/// Parses the format specification.
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

	/// Formats a timer query.
	/// @tparam FormatContext Formatting context type.
	/// @param query Timer query to format.
	/// @param context Formatting context.
	/// @return Iterator to the end of the output range.
	template <typename FormatContext>
	FormatContext::iterator format(const tr::timer_query& query, FormatContext& context) const
	{
		if (query.valid()) {
			return std::format_to(context.out(), "\"{}\" (OpenGL ID: {})", query.label(), query.unwrap());
		}
		else {
			return std::format_to(context.out(), "<invalid timer query at {}>", static_cast<const void*>(&query));
		}
	}
};