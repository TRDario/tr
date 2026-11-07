/// @file
/// @brief Tests specialization_of.hpp.

#include <tr/utility/specialization_of.hpp>

//

/// @brief Dummy template with a constant and type parameter.
/// @tparam A, B Dummy template parameters.
template <int A, typename B>
struct ct_template
{
};

//

/// @brief Tests a negative case of `tr::specialization_of`.
static_assert(!tr::specialization_of<std::string, std::vector>);

/// @brief Tests a positive case of `tr::specialization_of`.
static_assert(tr::specialization_of<std::vector<int>, std::vector>);

/// @brief Tests a negative case of `tr::specialization_of_c`.
static_assert(!tr::specialization_of_c<std::string, std::ratio>);

/// @brief Tests a positive case of `tr::specialization_of_c`.
static_assert(tr::specialization_of_c<std::ratio<1, 10>, std::ratio>);

/// @brief Tests a negative case of `tr::specialization_of_ct`.
static_assert(!tr::specialization_of_ct<std::string, ct_template>);

/// @brief Tests a positive case of `tr::specialization_of_ct`.
static_assert(tr::specialization_of_ct<ct_template<10, int>, ct_template>);

/// @brief Tests a negative case of `tr::specialization_of_tc`.
static_assert(!tr::specialization_of_tc<std::string, std::array>);

/// @brief Tests a positive case of `tr::specialization_of_tc`.
static_assert(tr::specialization_of_tc<std::array<int, 10>, std::array>);