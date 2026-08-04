/// @file
/// @brief Tests integer.hpp.

#include <tr/utility/integer.hpp>

//

static_assert(std::same_as<tr::size_type_t<10>, tr::u8>);
static_assert(std::same_as<tr::size_type_t<1000>, tr::u16>);
static_assert(std::same_as<tr::size_type_t<100000>, tr::u32>);
static_assert(std::same_as<tr::size_type_t<10000000000>, tr::u64>);
