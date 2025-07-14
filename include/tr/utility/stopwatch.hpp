#pragma once
#include "chrono.hpp"

namespace tr {
	/** @ingroup utility
	 *  @defgroup stopwatch Stopwatch
	 *  Minimal stopwatch class.
	 *  @{
	 */

	/******************************************************************************************************************
	 * Minimal stopwatch class.
	 ******************************************************************************************************************/
	class stopwatch {
	  public:
		/**************************************************************************************************************
		 * Constructs and starts the stopwatch.
		 **************************************************************************************************************/
		stopwatch() noexcept;

		/**************************************************************************************************************
		 * Gets the elapsed time since the stopwatch was started.
		 *
		 * @return The elapsed time since the stopwatch was started.
		 **************************************************************************************************************/
		duration elapsed() const noexcept;

		/**************************************************************************************************************
		 * Restarts the stopwatch and gets the elapsed time since it was started.
		 *
		 * @return The elapsed time since the stopwatch was started.
		 **************************************************************************************************************/
		duration lap() noexcept;

	  private:
		time_point _start;
	};

	/// @}
} // namespace tr