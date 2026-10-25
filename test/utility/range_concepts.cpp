/// @file
/// @brief Tests utility/range_concepts.hpp.

#include <tr/utility/range_concepts.hpp>

//

/// Tests `tr::sized_output_range` in the positive case.
static_assert(tr::sized_output_range<std::span<int>, int>, "std::span<int> should fulfill tr::sized_output_range<int>, but does not!");

/// Tests `tr::sized_output_range` in the negative case.
static_assert(!tr::sized_output_range<std::span<int>, std::vector<int>>,
			  "std::span<int> should not fulfill tr::sized_output_range<std::vector<int>>, but does!");

//

/// Tests `tr::input_iterator_to` in the positive case.
static_assert(tr::input_iterator_to<std::vector<int>::iterator, int>,
			  "std::vector<int>::iterator should fulfill tr::input_iterator_to<int>, but does not!");

/// Tests `tr::input_iterator_to` in the positive case.
static_assert(tr::input_iterator_to<std::istream_iterator<char>, char>,
			  "std::istream_iterator<char> should fulfill tr::input_iterator_to<char>, but does not!");

/// Tests `tr::input_iterator_to` in the negative case.
static_assert(!tr::input_iterator_to<std::vector<int>::iterator, float&>,
			  "std::vector<int>::iterator should not fulfill tr::input_iterator_to<float&>, but does!");

/// Tests `tr::input_iterator_to` in the negative case.
static_assert(!tr::input_iterator_to<std::back_insert_iterator<std::vector<int>>::iterator, int>,
			  "std::back_insert_iterator<std::vector<int>>::iterator should not fulfill tr::input_iterator_to<int>, but does!");

//

/// Tests `tr::forward_iterator_to` in the positive case.
static_assert(tr::forward_iterator_to<std::vector<int>::iterator, int>,
			  "std::vector<int>::iterator should fulfill tr::forward_iterator_to<int>, but does not!");

/// Tests `tr::forward_iterator_to` in the negative case.
static_assert(!tr::forward_iterator_to<std::istream_iterator<char>, char>,
			  "std::istream_iterator<char> should not fulfill tr::forward_iterator_to<char>, but does!");

/// Tests `tr::forward_iterator_to` in the negative case.
static_assert(!tr::forward_iterator_to<std::vector<int>::iterator, float&>,
			  "std::vector<int>::iterator should not fulfill tr::forward_iterator_to<float&>, but does!");

/// Tests `tr::forward_iterator_to` in the negative case.
static_assert(!tr::forward_iterator_to<std::back_insert_iterator<std::vector<int>>::iterator, int>,
			  "std::back_insert_iterator<std::vector<int>>::iterator should not fulfill tr::forward_iterator_to<int>, but does!");

//

/// Tests `tr::nothrow_forward_iterator_to` in the positive case.
static_assert(tr::nothrow_forward_iterator_to<std::vector<int>::iterator, int>,
			  "std::vector<int>::iterator should fulfill tr::nothrow_forward_iterator_to<int>, but does not!");

/// Tests `tr::nothrow_forward_iterator_to` in the negative case.
static_assert(!tr::nothrow_forward_iterator_to<std::istream_iterator<char>, char>,
			  "std::istream_iterator<char> should not fulfill tr::nothrow_forward_iterator_to<char>, but does!");

/// Tests `tr::nothrow_forward_iterator_to` in the negative case.
static_assert(!tr::nothrow_forward_iterator_to<std::vector<int>::iterator, float&>,
			  "std::vector<int>::iterator should not fulfill tr::nothrow_forward_iterator_to<float&>, but does!");

/// Tests `tr::nothrow_forward_iterator_to` in the negative case.
static_assert(!tr::nothrow_forward_iterator_to<std::back_insert_iterator<std::vector<int>>::iterator, int>,
			  "std::back_insert_iterator<std::vector<int>>::iterator should not fulfill tr::nothrow_forward_iterator_to<int>, but does!");