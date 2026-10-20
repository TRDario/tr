/// @file
/// @brief Tests utility/localization_script.hpp.

#include <gtest/gtest.h>
#include <tr/utility/localization_script.hpp>

//

/// Localization script test fixture.
class LocalizationScriptTest : public ::testing::Test
{
  protected:
	/// Vector of localization script errors.
	std::vector<tr::localization_script_error> errors;

	/// Localization map.
	tr::localization_map map;
};

/// Test fixture for localization script tests revolving around key-value pairs.
class LocalizationScriptKeyValueTest : public LocalizationScriptTest
{
};

/// Test fixture for localization script tests revolving around namespaces.
class LocalizationScriptNamespaceTest : public LocalizationScriptTest
{
};

/// Test fixture for localization script tests revolving around errors.
class LocalizationScriptErrorTest : public LocalizationScriptTest
{
};

//

/// Tests a basic localization script.
TEST_F(LocalizationScriptKeyValueTest, ParsesValue)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo=\"Foo\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Foo");
}

/// Tests a localization script with varied whitespace.
TEST_F(LocalizationScriptKeyValueTest, ParsesValueWithWhitespace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo // line comment \n\t=\n\t /* Multiline comment */ \"Foo\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Foo");
}

/// Tests a localization script with Unicode characters.
TEST_F(LocalizationScriptKeyValueTest, ParsesValueWithUnicode)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "ač東😳 = \"Ač東😳\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["ač東😳"], "Ač東😳");
}

/// Tests a localization script value with escaped characters.
TEST_F(LocalizationScriptKeyValueTest, ParsesEscapedValue)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), R"(foo = "\\, \", \n")");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "\\, \", \n");
}

/// Tests a localization script multiline value.
TEST_F(LocalizationScriptKeyValueTest, ParsesMultilineValue)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Multiline\nValue\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Multiline\nValue");
}

/// Tests a localization script with multiple values.
TEST_F(LocalizationScriptKeyValueTest, ParsesScriptWithMultipleValues)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Foo\"bar=\"Bar\"\nbaz = \"Baz\"");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 3);
	EXPECT_EQ(map["foo"], "Foo");
	EXPECT_EQ(map["bar"], "Bar");
	EXPECT_EQ(map["baz"], "Baz");
}

//

/// Tests a localization script with an empty namespace.
TEST_F(LocalizationScriptNamespaceTest, ParsesEmptyNamespace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo {}");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 0);
}

/// Tests a localization script with a basic namespace.
TEST_F(LocalizationScriptNamespaceTest, ParsesBasicNamespace)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo{bar = \"Bar\" baz = \"Baz\"}");
	EXPECT_TRUE(errors.empty());
	EXPECT_EQ(map.size(), 2);
	EXPECT_EQ(map["foo.bar"], "Bar");
	EXPECT_EQ(map["foo.baz"], "Baz");
}

/// Tests a localization script with a namespace and varied namespace.
TEST_F(LocalizationScriptNamespaceTest, ParsesNamespaceWithWhitespace)
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

/// Tests a localization script with a nested namespaces.
TEST_F(LocalizationScriptNamespaceTest, ParsesNestedNamespaces)
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

/// Tests a localization script with a namespace that is reopened.
TEST_F(LocalizationScriptNamespaceTest, ParsesReopenedNamespace)
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

/// Tests a localization script that triggers an unexpected backslash error.
TEST_F(LocalizationScriptErrorTest, ReportsUnexpectedBackslashError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "\\foo\\{\\bar\\=\\\"Bar\"\\}\\");
	EXPECT_EQ(errors.size(), 7);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_unexpected_backslash>());
	}
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar"], "Bar");
}

/// Tests a localization script that triggers an unexpected slash error.
TEST_F(LocalizationScriptErrorTest, ReportsUnexpectedSlashError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "/foo/{/bar/=/\"Bar\"/}/");
	EXPECT_EQ(errors.size(), 7);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_unexpected_slash>());
	}
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar"], "Bar");
}

/// Tests a localization script that triggers an unknown escape sequence error.
TEST_F(LocalizationScriptErrorTest, ReportsUnknownEscapeSequenceError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), R"(foo = "\\, \", \n, \e")");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_unknown_escape_sequence>());
	EXPECT_EQ(errors[0].get<tr::localization_script_unknown_escape_sequence>().character, 'e');
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "\\, \", \n, ");
}

/// Tests a localization script that triggers an unterminated string error.
TEST_F(LocalizationScriptErrorTest, ReportsUnterminatedStringError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Unterminated");
	EXPECT_EQ(errors.size(), 2);
	EXPECT_TRUE(errors[0].is<tr::localization_script_unterminated_string>());
	EXPECT_EQ(map.size(), 0);
}

/// Tests a localization script that triggers an unterminated multiline comment error.
TEST_F(LocalizationScriptErrorTest, ReportsUnterminatedMultilineCommentError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "/*This is a comment*");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_unterminated_multiline_comment>());
	EXPECT_EQ(map.size(), 0);
}

/// Tests a localization script that triggers an expected symbol error.
TEST_F(LocalizationScriptErrorTest, ReportsExpectedSymbolError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "= \"Foo\"");
	EXPECT_EQ(errors.size(), 2);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_expected_symbol>());
	}
	EXPECT_EQ(map.size(), 0);
}

/// Tests a localization script that triggers an expected closing brace error.
TEST_F(LocalizationScriptErrorTest, ReportsExpectedClosingBraceError)
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

/// Tests a localization script that triggers an extraneous closing brace error.
TEST_F(LocalizationScriptErrorTest, ReportsExtraneousClosingBraceError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo { bar = \"Bar\" } }");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_extraneous_closing_brace>());
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo.bar"], "Bar");
}

/// Tests a localization script that triggers an expected symbol or closing brace error.
TEST_F(LocalizationScriptErrorTest, ReportsExpectedSymbolOrClosingBraceError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo { = \"Foo\" }");
	EXPECT_EQ(errors.size(), 2);
	for (const auto& error : errors) {
		EXPECT_TRUE(error.is<tr::localization_script_expected_symbol_or_closing_brace>());
	}
	EXPECT_EQ(map.size(), 0);
}

/// Tests a localization script that triggers an expected equals or opening brace error.
TEST_F(LocalizationScriptErrorTest, ReportsExpectedEqualsOrOpeningBraceError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_expected_equals_or_opening_brace>());
	EXPECT_EQ(map.size(), 0);
}

/// Tests a localization script that triggers an expected string error.
TEST_F(LocalizationScriptErrorTest, ReportsExpectedStringError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo=");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_expected_string>());
	EXPECT_EQ(map.size(), 0);
}

/// Tests a localization script that triggers a duplicate key error.
TEST_F(LocalizationScriptErrorTest, ReportsDuplicateKeyError)
{
	tr::parse_localization_script_to(map, std::back_inserter(errors), "foo = \"Foo\"\nfoo = \"Bar\"");
	EXPECT_EQ(errors.size(), 1);
	EXPECT_TRUE(errors[0].is<tr::localization_script_duplicate_key>());
	EXPECT_EQ(errors[0].get<tr::localization_script_duplicate_key>().key, "foo");
	EXPECT_EQ(errors[0].get<tr::localization_script_duplicate_key>().original_line, 1);
	EXPECT_EQ(map.size(), 1);
	EXPECT_EQ(map["foo"], "Foo");
}