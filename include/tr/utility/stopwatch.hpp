///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides a simple stopwatch class.                                                                                                    //
//                                                                                                                                       //
// Constructing the stopwatch automatically starts it. The stopwatch can then be restarted with the .lap() method, which returns the     //
// elapsed time of the previous lap. The elapsed time since the stopwatch was started can also be checked at any time with               //
// the .elapsed() method:                                                                                                                //
//     - tr::stopwatch stopwatch; -> starts 'stopwatch' automatically                                                                    //
//     - stopwatch.elapsed() -> gets the elapsed time, but does not restart the stopwatch                                                //
//     - stopwatch.lap() -> gets the elapsed time and restarts the stopwatch                                                             //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "chrono.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Basic stopwatch class.
	class stopwatch {
	  public:
		// Constructs and starts the stopwatch.
		stopwatch();

		// Gets the elapsed time since the stopwatch was started.
		duration elapsed() const;
		// Restarts the stopwatch and gets the elapsed time since it was started.
		duration lap();

	  private:
		// The time point at which the current measurement started.
		std::chrono::steady_clock::time_point m_start;
	};
} // namespace tr