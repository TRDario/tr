///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/utf8.hpp.                                                                                                               //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tr/utility.hpp>

int utf8(int, char**)
{
	constexpr tr::static_string<10> characters{"ač東😳"};
	constexpr std::array<tr::codepoint, 4> codepoints{0x61, 0x10D, 0x6771, 0x1F633};
	constexpr std::array<tr::usize, 5> offsets{0, 1, 3, 6, 10};

	// Test UTF-8 -> codepoint conversion.
	for (int i = 0; i < 4; ++i) {
		if (tr::utf8::to_cp(characters.begin() + offsets[i]) != codepoints[i]) {
			tr::println_error("tr::utf8::to_cp(it) did not return the expected result.");
			return EXIT_FAILURE;
		}
	}

	// Test forward iteration.
	for (auto [it, i] = std::pair{characters.begin(), 0}; i <= 4; it = tr::utf8::next(it), ++i) {
		if (tr::usize(std::distance(characters.begin(), it)) != offsets[i]) {
			tr::println_error("tr::utf8::next(it) did not return the expected result.");
			return EXIT_FAILURE;
		}
		if (tr::utf8::next(characters.begin(), i) != it) {
			tr::println_error("tr::utf8::next(it, n) did not return the expected result.");
			return EXIT_FAILURE;
		}
	}

	// Test backward iteration.
	for (auto [it, i] = std::pair{characters.end(), 4}; i >= 0; it = tr::utf8::prev(it), --i) {
		if (tr::usize(std::distance(characters.begin(), it)) != offsets[i]) {
			tr::println_error("tr::utf8::prev(it) did not return the expected result.");
			return EXIT_FAILURE;
		}
		if (tr::utf8::prev(characters.end(), 4 - i) != it) {
			tr::println_error("tr::utf8::prev(it, n) did not return the expected result.");
			return EXIT_FAILURE;
		}
	}

	// Test insertion.
	tr::static_string<10> insert;
	for (tr::codepoint cp : codepoints) {
		tr::utf8::insert(insert, insert.begin(), cp);
	}
	if (insert != "😳東ča") {
		tr::println_error("tr::utf8::insert(string, where, codepoint) did not behave correctly.");
		return EXIT_FAILURE;
	}

	// Test deletion.
	tr::static_string<10> erase{insert};
	tr::utf8::erase(erase, erase.begin());
	if (erase != "東ča") {
		tr::println_error("tr::utf8::erase(string, where) did not behave correctly for 4-byte character.");
		return EXIT_FAILURE;
	}
	tr::utf8::erase(erase, erase.begin());
	if (erase != "ča") {
		tr::println_error("tr::utf8::erase(string, where) did not behave correctly for 3-byte character.");
		return EXIT_FAILURE;
	}
	tr::utf8::erase(erase, erase.begin());
	if (erase != "a") {
		tr::println_error("tr::utf8::erase(string, where) did not behave correctly for 2-byte character.");
		return EXIT_FAILURE;
	}
	tr::utf8::erase(erase, erase.begin());
	if (!erase.empty()) {
		tr::println_error("tr::utf8::erase(string, where) did not behave correctly for 1-byte character.");
		return EXIT_FAILURE;
	}

	// Test popping back.
	tr::static_string<10> pop_back{insert};
	tr::utf8::pop_back(pop_back);
	if (pop_back != "😳東č") {
		tr::println_error("tr::utf8::pop_back(string) did not behave correctly for 4-byte character.");
		return EXIT_FAILURE;
	}
	tr::utf8::pop_back(pop_back);
	if (pop_back != "😳東") {
		tr::println_error("tr::utf8::pop_back(string) did not behave correctly for 3-byte character.");
		return EXIT_FAILURE;
	}
	tr::utf8::pop_back(pop_back);
	if (pop_back != "😳") {
		tr::println_error("tr::utf8::pop_back(string) did not behave correctly for 2-byte character.");
		return EXIT_FAILURE;
	}
	tr::utf8::pop_back(pop_back);
	if (!pop_back.empty()) {
		tr::println_error("tr::utf8::pop_back(string) did not behave correctly for 1-byte character.");
		return EXIT_FAILURE;
	}

	// Test UTF-8 iterator.
	tr::static_vector<tr::codepoint, 4> range_codepoints;
	for (tr::codepoint cp : tr::utf8::range(characters)) {
		range_codepoints.emplace_back(cp);
	}
	if (!std::ranges::equal(range_codepoints, codepoints)) {
		tr::println_error("tr::utf8::const_it did not behave correctly.");
		return EXIT_FAILURE;
	}

	// Test length check.
	if (tr::utf8::length(characters) != 4) {
		tr::println_error("tr::utf8::length(string) did not return the expected result.");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}