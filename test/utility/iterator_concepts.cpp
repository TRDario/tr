/// @file
/// @brief Tests utility/iterator_concepts.hpp.

#include <tr/utility/iterator_concepts.hpp>

//

/// Tests the `tr::nothrow_iterator` concept.
static_assert(tr::nothrow_iterator<const char*>, "const char* should fulfill tr::nothrow_iterator, but does not!");

/// Tests the `tr::nothrow_iterator` concept.
static_assert(!tr::nothrow_iterator<std::back_insert_iterator<std::vector<int>>>,
			  "std::back_insert_iterator<std::vector<int>> should not fulfill tr::nothrow_iterator, but does!");

//

/// Tests the `tr::input_iterator` concept.
static_assert(tr::input_iterator_to<const char*, char>, "const char* should fullfill tr::input_iterator_to<char>, but does not!");

/// Tests the `tr::input_iterator` concept.
static_assert(!tr::input_iterator_to<const char*, char&>, "const char* should not fullfill tr::input_iterator_to<char&>, but does!");

/// Tests the `tr::input_iterator` concept.
static_assert(tr::input_iterator_to<std::list<int>::iterator, int>,
			  "std::list<int>::iterator should fullfill tr::input_iterator_to<int>, but does not!");

/// Tests the `tr::input_iterator` concept.
static_assert(tr::input_iterator_to<std::istream_iterator<int>, int>,
			  "std::istream_iterator<int> should fullfill tr::input_iterator_to<int>, but does not!");

/// Tests the `tr::input_iterator` concept.
static_assert(!tr::input_iterator_to<std::back_insert_iterator<std::vector<int>>, int>,
			  "std::back_insert_iterator<std::vector<int>> should not fullfill tr::input_iterator_to<int>, but does!");

//

/// Tests the `tr::forward_iterator_to` concept.
static_assert(tr::forward_iterator_to<const char*, char>, "const char* should fullfill tr::forward_iterator_to<char>, but does not!");

/// Tests the `tr::forward_iterator_to` concept.
static_assert(!tr::forward_iterator_to<const char*, char&>, "const char* should not fullfill tr::forward_iterator_to<char&>, but does!");

/// Tests the `tr::forward_iterator_to` concept.
static_assert(tr::forward_iterator_to<std::list<int>::iterator, int>,
			  "std::list<int>::iterator should fullfill tr::forward_iterator_to<int>, but does not!");

/// Tests the `tr::forward_iterator_to` concept.
static_assert(!tr::forward_iterator_to<std::istream_iterator<int>, int>,
			  "std::istream_iterator<int> should not fullfill tr::forward_iterator_to<int>, but does!");

/// Tests the `tr::forward_iterator_to` concept.
static_assert(!tr::forward_iterator_to<std::back_insert_iterator<std::vector<int>>, int>,
			  "std::back_insert_iterator<std::vector<int>> should not fullfill tr::forward_iterator_to<int>, but does!");

//

/// Tests the `tr::nothrow_forward_iterator_to` concept.
static_assert(tr::nothrow_forward_iterator_to<const char*, char>,
			  "const char* should fullfill tr::nothrow_forward_iterator_to<char>, but does not!");

/// Tests the `tr::nothrow_forward_iterator_to` concept.
static_assert(!tr::nothrow_forward_iterator_to<const char*, char&>,
			  "const char* should not fullfill tr::nothrow_forward_iterator_to<char&>, but does!");

/// Tests the `tr::nothrow_forward_iterator_to` concept.
static_assert(tr::nothrow_forward_iterator_to<std::list<int>::iterator, int>,
			  "std::list<int>::iterator should fullfill tr::nothrow_forward_iterator_to<int>, but does not!");

/// Tests the `tr::nothrow_forward_iterator_to` concept.
static_assert(!tr::nothrow_forward_iterator_to<std::istream_iterator<int>, int>,
			  "std::istream_iterator<int> should not fullfill tr::nothrow_forward_iterator_to<int>, but does!");

/// Tests the `tr::nothrow_forward_iterator_to` concept.
static_assert(!tr::nothrow_forward_iterator_to<std::back_insert_iterator<std::vector<int>>, int>,
			  "std::back_insert_iterator<std::vector<int>> should not fullfill tr::nothrow_forward_iterator_to<int>, but does!");
