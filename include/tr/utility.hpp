/// @file
/// @brief Includes all headers from the utility module.
/// @details
/// The utility module provides mostly helper functionality supplemental to the other modules.

#pragma once
#include <tr/utility/alignment.hpp>                 // IWYU pragma: export
#include <tr/utility/angle.hpp>                     // IWYU pragma: export
#include <tr/utility/atlas_entries.hpp>             // IWYU pragma: export
#include <tr/utility/atlas_packer.hpp>              // IWYU pragma: export
#include <tr/utility/benchmark.hpp>                 // IWYU pragma: export
#include <tr/utility/binary_io.hpp>                 // IWYU pragma: export
#include <tr/utility/binary_io_specializations.hpp> // IWYU pragma: export
#include <tr/utility/chrono.hpp>                    // IWYU pragma: export
#include <tr/utility/circle.hpp>                    // IWYU pragma: export
#include <tr/utility/color.hpp>                     // IWYU pragma: export
#include <tr/utility/concepts.hpp>                  // IWYU pragma: export
#include <tr/utility/defer.hpp>                     // IWYU pragma: export
#include <tr/utility/draw_geometry.hpp>             // IWYU pragma: export
#include <tr/utility/dynamic_ref_cast.hpp>          // IWYU pragma: export
#include <tr/utility/encryption.hpp>                // IWYU pragma: export
#include <tr/utility/enum.hpp>                      // IWYU pragma: export
#include <tr/utility/exception.hpp>                 // IWYU pragma: export
#include <tr/utility/function_traits.hpp>           // IWYU pragma: export
#include <tr/utility/handle.hpp>                    // IWYU pragma: export
#include <tr/utility/hash_map.hpp>                  // IWYU pragma: export
#include <tr/utility/integer.hpp>                   // IWYU pragma: export
#include <tr/utility/iostream.hpp>                  // IWYU pragma: export
#include <tr/utility/iterator_interface.hpp>        // IWYU pragma: export
#include <tr/utility/line.hpp>                      // IWYU pragma: export
#include <tr/utility/localization_map.hpp>          // IWYU pragma: export
#include <tr/utility/logger.hpp>                    // IWYU pragma: export
#include <tr/utility/macro.hpp>                     // IWYU pragma: export
#include <tr/utility/math.hpp>                      // IWYU pragma: export
#include <tr/utility/matrix.hpp>                    // IWYU pragma: export
#include <tr/utility/mstream.hpp>                   // IWYU pragma: export
#include <tr/utility/norm_cast.hpp>                 // IWYU pragma: export
#include <tr/utility/opt_ref.hpp>                   // IWYU pragma: export
#include <tr/utility/out_handle.hpp>                // IWYU pragma: export
#include <tr/utility/pointer_iterator.hpp>          // IWYU pragma: export
#include <tr/utility/polygon.hpp>                   // IWYU pragma: export
#include <tr/utility/ranges.hpp>                    // IWYU pragma: export
#include <tr/utility/rectangle.hpp>                 // IWYU pragma: export
#include <tr/utility/rectangle_edges.hpp>           // IWYU pragma: export
#include <tr/utility/ref.hpp>                       // IWYU pragma: export
#include <tr/utility/rng.hpp>                       // IWYU pragma: export
#include <tr/utility/specialization_of.hpp>         // IWYU pragma: export
#include <tr/utility/static_string.hpp>             // IWYU pragma: export
#include <tr/utility/static_vector.hpp>             // IWYU pragma: export
#include <tr/utility/stopwatch.hpp>                 // IWYU pragma: export
#include <tr/utility/string_literal.hpp>            // IWYU pragma: export
#include <tr/utility/timer.hpp>                     // IWYU pragma: export
#include <tr/utility/triangle.hpp>                  // IWYU pragma: export
#include <tr/utility/type_name.hpp>                 // IWYU pragma: export
#include <tr/utility/utf8.hpp>                      // IWYU pragma: export
#include <tr/utility/variant.hpp>                   // IWYU pragma: export
#include <tr/utility/vector.hpp>                    // IWYU pragma: export
#include <tr/utility/zstring_view.hpp>              // IWYU pragma: export

/// Namespace containing all tr functionality
namespace tr
{
	/// Namespace containing all aliases defined by tr.
	inline namespace aliases
	{
	}
	/// Namespace containing all literals defined by tr.
	inline namespace literals
	{
	}
} // namespace tr