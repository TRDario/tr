/// @file
/// @brief Tests specialization_of.hpp.

#include <tr/utility/specialization_of.hpp>

//

template <int A, typename B>
struct ct_template
{
};

//

static_assert(!tr::specialization_of<std::string, std::vector>);

static_assert(tr::specialization_of<std::vector<int>, std::vector>);

static_assert(!tr::specialization_of_c<std::string, std::ratio>);

static_assert(tr::specialization_of_c<std::ratio<1, 10>, std::ratio>);

static_assert(!tr::specialization_of_ct<std::string, ct_template>);

static_assert(tr::specialization_of_ct<ct_template<10, int>, ct_template>);

static_assert(!tr::specialization_of_tc<std::string, std::array>);

static_assert(tr::specialization_of_tc<std::array<int, 10>, std::array>);