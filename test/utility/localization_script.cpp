/// @file
/// @brief Tests utility/localization_script.hpp.

#include <gtest/gtest.h>
#include <tr/utility/localization_script.hpp>

//

/// Localization script test fixture.
class localization_script_test : public ::testing::Test
{
  protected:
	/// Vector of localization script errors.
	std::vector<tr::localization_script_error> errors;

	/// Localization map.
	tr::localization_map map;
};

/// Test fixture for localization script tests revolving around key-value pairs.
class localization_script_key_value_test : public localization_script_test
{
};

/// Test fixture for localization script tests revolving around namespaces.
class localization_script_namespace_test : public localization_script_test
{
};

/// Test fixture for localization script tests revolving around errors.
class localization_script_error_test : public localization_script_test
{
};

//

TEST_F(localization_script_key_value_test, basic)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo=\"Foo\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Foo");
}

TEST_F(localization_script_key_value_test, whitespace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo // line comment \n\t=\n\t /* Multiline comment */ \"Foo\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Foo");
}

TEST_F(localization_script_key_value_test, unicode)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "ač東😳 = \"Ač東😳\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["ač東😳"], "Ač東😳");
}

TEST_F(localization_script_key_value_test, escaped)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), R"(foo = "\\, \", \n")");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "\\, \", \n");
}

TEST_F(localization_script_key_value_test, multiline)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Multiline\nValue\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Multiline\nValue");
}

TEST_F(localization_script_key_value_test, multiple)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Foo\"bar=\"Bar\"\nbaz = \"Baz\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["foo"], "Foo");
	EXPECT_EQ(map["bar"], "Bar");
	EXPECT_EQ(map["baz"], "Baz");
}

//

TEST_F(localization_script_namespace_test, empty)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo {}");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 0);
}

TEST_F(localization_script_namespace_test, basic)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo{bar = \"Bar\" baz = \"Baz\"}");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 2);
	EXPECT_EQ(map["foo.bar"], "Bar");
	EXPECT_EQ(map["foo.baz"], "Baz");
}

TEST_F(localization_script_namespace_test, whitespace)
{
	constexpr std::string_view script{R"(
		foo /* multiline comment */ {
			// line comment
			bar = "Bar"
			baz = "Baz"
			// line comment
		}
	)"};

	tr::parse_localization_script_to(map, std::back_inserter(errors), script);
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 2);
	EXPECT_EQ(map["foo.bar"], "Bar");
	EXPECT_EQ(map["foo.baz"], "Baz");
}

TEST_F(localization_script_namespace_test, nested)
{
	constexpr std::string_view script{R"(
		foo {
			bar {
				baz = "Baz"
			}
		}
	)"};

	tr::parse_localization_script_to(map, std::back_inserter(errors), script);
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar.baz"], "Baz");
}

TEST_F(localization_script_namespace_test, reopening)
{
	constexpr std::string_view script{R"(
		foo {
			bar = "Bar"
		}
		baz {
			qux = "Qux"
		}
		foo {
			quux = "Quux"
		}
	)"};

	tr::parse_localization_script_to(map, std::back_inserter(errors), script);
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["foo.bar"], "Bar");
	EXPECT_EQ(map["baz.qux"], "Qux");
	EXPECT_EQ(map["foo.quux"], "Quux");
}

//

TEST_F(localization_script_error_test, unexpected_backslash)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "\\foo\\{\\bar\\=\\\"Bar\"\\}\\");
	EXPECT_EQ(errors.size(), 7);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_unexpected_backslash>());
	}
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar"], "Bar");
}

TEST_F(localization_script_error_test, unexpected_slash)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "/foo/{/bar/=/\"Bar\"/}/");
	EXPECT_EQ(errors.size(), 7);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_unexpected_slash>());
	}
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar"], "Bar");
}

TEST_F(localization_script_error_test, unknown_escape_sequence)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), R"(foo = "\\, \", \n, \e")");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_unknown_escape_sequence>());
	EXPECT_EQ(errors[0].get<tr::localization_script_unknown_escape_sequence>().character, 'e');
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "\\, \", \n, ");
}

TEST_F(localization_script_error_test, unterminated_string)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Unterminated");
	EXPECT_EQ(errors.size(), 2);
	EXPECT_TRUE(errors[0].is<tr::localization_script_unterminated_string>());
	EXPECT_EQ(map.size(), 0);
}

TEST_F(localization_script_error_test, unterminated_multiline_comment)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "/*This is a comment*");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_unterminated_multiline_comment>());
	EXPECT_EQ(map.size(), 0);
}

TEST_F(localization_script_error_test, expected_symbol)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "= \"Foo\"");
	EXPECT_EQ(errors.size(), 2);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_expected_symbol>());
	}
	EXPECT_EQ(map.size(), 0);
}

TEST_F(localization_script_error_test, expected_closing_brace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo {\nbar {\nbaz = \"Baz\"");
	EXPECT_EQ(errors.size(), 2);
	EXPECT_TRUE(errors[0].is<tr::localization_script_expected_closing_brace>());
	EXPECT_EQ(errors[0].get<tr::localization_script_expected_closing_brace>().opening_brace_line, 2);
	EXPECT_TRUE(errors[1].is<tr::localization_script_expected_closing_brace>());
	EXPECT_EQ(errors[1].get<tr::localization_script_expected_closing_brace>().opening_brace_line, 1);
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar.baz"], "Baz");
}

TEST_F(localization_script_error_test, extraneous_closing_brace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo { bar = \"Bar\" } }");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_extraneous_closing_brace>());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar"], "Bar");
}

TEST_F(localization_script_error_test, expected_symbol_or_closing_brace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo { = \"Foo\" }");
	EXPECT_EQ(errors.size(), 2);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_expected_symbol_or_closing_brace>());
	}
	EXPECT_EQ(map.size(), 0);
}

TEST_F(localization_script_error_test, expected_equals_or_opening_brace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_expected_equals_or_opening_brace>());
	EXPECT_EQ(map.size(), 0);
}

TEST_F(localization_script_error_test, expected_string)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo=");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_expected_string>());
	EXPECT_EQ(map.size(), 0);
}

TEST_F(localization_script_error_test, duplicate_key)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Foo\"\nfoo = \"Bar\"");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_duplicate_key>());
	EXPECT_EQ(errors[0].get<tr::localization_script_duplicate_key>().key, "foo");
	EXPECT_EQ(errors[0].get<tr::localization_script_duplicate_key>().original_line, 1);
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Foo");
}