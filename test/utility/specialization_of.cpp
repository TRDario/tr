/// @file
/// @brief Tests specialization_of.hpp.

#include <tr/utility/specialization_of.hpp>

//

template <int A, typename B>
struct vt_template
{
};

static_assert(!tr::is_specialization_of<std::string, std::vector>::value);
static_assert(tr::is_specialization_of<std::vector<int>, std::vector>::value);
static_assert(!tr::is_specialization_of_v<std::string, std::ratio>::value);
static_assert(tr::is_specialization_of_v<std::ratio<1, 10>, std::ratio>::value);
static_assert(!tr::is_specialization_of_vt<std::string, vt_template>::value);
static_assert(tr::is_specialization_of_vt<vt_template<10, int>, vt_template>::value);
static_assert(!tr::is_specialization_of_tv<std::string, std::array>::value);
static_assert(tr::is_specialization_of_tv<std::array<int, 10>, std::array>::value);