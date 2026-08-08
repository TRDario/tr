/// @file
/// @brief Tests specialization_of.hpp.

#include <tr/utility/specialization_of.hpp>

//

template <int A, typename B>
struct vt_template
{
};

//

static_assert(!tr::specialization_of<std::string, std::vector>);

static_assert(tr::specialization_of<std::vector<int>, std::vector>);

static_assert(!tr::specialization_of_v<std::string, std::ratio>);

static_assert(tr::specialization_of_v<std::ratio<1, 10>, std::ratio>);

static_assert(!tr::specialization_of_vt<std::string, vt_template>);

static_assert(tr::specialization_of_vt<vt_template<10, int>, vt_template>);

static_assert(!tr::specialization_of_tv<std::string, std::array>);

static_assert(tr::specialization_of_tv<std::array<int, 10>, std::array>);