/// @file
/// @brief Provides a basic stopwatch class.

#pragma once
#include "chrono.hpp"

//

namespace tr
{
	/// Basic stopwatch class.
	class stopwatch
	{
	  public:
		/// @name Constructors
		/// @{

		/// Constructs and starts the stopwatch.
		stopwatch();

		/// @}
		/// @name Access
		/// @{

		/// Gets the elapsed time since the stopwatch was started.
		/// @return Elapsed time since the stopwatch was started.
		duration elapsed() const;

		/// Restarts the stopwatch and gets the elapsed time since it was started.
		/// @return Elapsed time of the previous lap.
		duration lap();

		/// @}

	  private:
		/// Time point at which the current measurement started.
		std::chrono::steady_clock::time_point m_start;
	};
} // namespace tr