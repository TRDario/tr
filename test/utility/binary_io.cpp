///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Tests utility/binary_io.hpp.                                                                                                          //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tr/utility.hpp>

#define CHECK_ROUNDTRIP(expr)                                                                                                              \
	do {                                                                                                                                   \
		const auto value{expr};                                                                                                            \
		tr::write_binary(ios, value);                                                                                                      \
		if (tr::read_binary<decltype(expr)>(ios) != value) {                                                                               \
			tr::println_error("tr::read/write_binary<{}> did not round-trip correctly.", tr::type_name<decltype(expr)>());                 \
			return EXIT_FAILURE;                                                                                                           \
		}                                                                                                                                  \
	} while (0)

int binary_io(int, char**)
{
	tr::rng rng;
	std::stringstream ios;

	// Check primitives.
	CHECK_ROUNDTRIP(rng.generate<tr::i8>());
	CHECK_ROUNDTRIP(rng.generate<tr::u8>());
	CHECK_ROUNDTRIP(rng.generate<tr::i16>());
	CHECK_ROUNDTRIP(rng.generate<tr::u16>());
	CHECK_ROUNDTRIP(rng.generate<tr::i32>());
	CHECK_ROUNDTRIP(rng.generate<tr::u32>());
	CHECK_ROUNDTRIP(rng.generate<tr::i64>());
	CHECK_ROUNDTRIP(rng.generate<tr::u64>());
	CHECK_ROUNDTRIP(rng.generate<float>());
	CHECK_ROUNDTRIP(rng.generate<double>());
	CHECK_ROUNDTRIP(tr::severity::error);
	CHECK_ROUNDTRIP(rng.generate<glm::vec2>(100));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(glm::ortho(0.0f, 1000.0f, 1000.0f, 0.0f)));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(std::pair<int, float>(rng.generate<int>(), rng.generate<float>())));

	// Check magic.
	tr::write_binary_magic(ios, "tr");
	if (!tr::read_binary_magic(ios, "tr")) {
		tr::println_error("tr::read/write_binary_magic did not round-trip correctly.");
		return EXIT_FAILURE;
	}

	// Check strings.
	const char* cstr1{"test1"};
	const char cstr2[]{"test2"};
	std::string_view strview{"test3"};
	std::string str{"test4"};
	tr::write_binary(ios, cstr1, cstr2, strview, str);
	if (tr::read_binary<std::string>(ios) != cstr1 || tr::read_binary<std::string>(ios) != cstr2 ||
		tr::read_binary<std::string>(ios) != strview || tr::read_binary<std::string>(ios) != str) {
		tr::println_error("tr::read/write_binary for strings did not round-trip correctly.");
		return EXIT_FAILURE;
	}

	// Check containers.
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(std::array<int, 5>{1, 2, 3, 4, 5}));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(std::vector<int>{1, 2, 3, 4, 5}));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(std::set<int>{1, 2, 3, 4, 5}));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(boost::unordered_flat_set<int>{1, 2, 3, 4, 5}));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(boost::unordered_node_set<int>{1, 2, 3, 4, 5}));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(std::map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}}));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(boost::unordered_flat_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}}));
	CHECK_ROUNDTRIP(TR_MACRO_COMMA_GUARD(boost::unordered_node_map<int, std::string>{{1, "a"}, {2, "b"}, {3, "c"}}));

	return EXIT_SUCCESS;
}