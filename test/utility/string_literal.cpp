/// @file
/// @brief Tests string_literal.hpp.

#include <tr/utility/string_literal.hpp>

//

/// Tests concatenating string literals.
static_assert(tr::concatenate_string_literals("test", "tost", "tist") == std::string_view{"testtosttist"});