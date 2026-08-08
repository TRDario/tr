/// @file
/// @brief Tests type_name.hpp.

#include <tr/utility/type_name.hpp>

//

static_assert(tr::type_name<int>() == "int");

static_assert(tr::type_name<const float>() == "const float");