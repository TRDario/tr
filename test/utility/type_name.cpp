/// @file
/// @brief Tests type_name.hpp.

#include <tr/utility/type_name.hpp>

//

/// Tests the type name of 'int'.
static_assert(tr::type_name<int>() == "int");

/// Tests the type name of 'const float'.
static_assert(tr::type_name<const float>() == "const float");