#pragma once
#include "chrono.hpp"

namespace tr {
	// Minimal stopwatch class.
	class stopwatch {
	  public:
		// Constructs and starts the stopwatch.
		stopwatch();

		// Gets the elapsed time since the stopwatch was started.
		duration elapsed() const;
		// Restarts the stopwatch and gets the elapsed time since it was started.
		duration lap();

	  private:
		time_point m_start;
	};
} // namespace tr