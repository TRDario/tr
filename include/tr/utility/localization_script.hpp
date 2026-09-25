/// @file
/// @brief Provides localization script parsing functions.

#pragma once
#include <tr/utility/internal/localization/parser.hpp>
#include <tr/utility/internal/localization/script.hpp>
#include <tr/utility/iostream.hpp>
#include <tr/utility/localization_map.hpp>
#include <tr/utility/localization_script_error.hpp>

//

namespace tr
{
	/// Result type returned by `tr::parse_localization_script`.
	/// @tparam ErrorOut Error output iterator type.
	template <std::output_iterator<localization_script_error> ErrorOut>
	struct parse_localization_script_result
	{
		/// Localization map holding the parsed localization data.
		tr::localization_map map;

		/// Error output iterator.
		ErrorOut error_out;
	};

	/// @name Localization script
	/// @{

	/// Parses localization script into a localization map.
	/// @tparam ErrorOut Error output iterator type.
	/// @tparam Iterator Character input iterator type.
	/// @tparam Sentinel Sentinel type for `Iterator`.
	/// @param error_out Error output iterator.
	/// @param begin Iterator to the beginning of the character range.
	/// @param end Iterator to the end of the character range.
	/// @param chunk_name Name of the parsed chunk.
	/// @return Localization script parsing result.
	template <std::output_iterator<localization_script_error> ErrorOut, input_iterator_to_convertible_to<char> Iterator,
			  std::sentinel_for<Iterator> Sentinel>
	[[nodiscard]] parse_localization_script_result<ErrorOut> parse_localization_script(ErrorOut error_out, Iterator begin, Sentinel end,
																					   std::string_view chunk_name = "<unnamed>")
	{
		auto result{internal::localization::parse_localization_script(error_out, begin, end, chunk_name)};
		return parse_localization_script_result{localization_map{result.pairs}, result.error_out};
	}

	/// Parses localization script into a localization map.
	/// @tparam ErrorOut Error output iterator type.
	/// @tparam Range Character input range.
	/// @param error_out Error output iterator.
	/// @param range Character range to parse.
	/// @param chunk_name Name of the parsed chunk.
	/// @return Localization script parsing result.
	template <std::output_iterator<localization_script_error> ErrorOut, input_range_to_convertible_to<char> Range>
	[[nodiscard]] parse_localization_script_result<ErrorOut> parse_localization_script(ErrorOut error_out, Range&& range,
																					   std::string_view chunk_name = "<unnamed>")
	{
		return parse_localization_script(error_out, std::ranges::begin(range), std::ranges::end(range), chunk_name);
	}

	/// Parses a localization script file into a localization map.
	/// @tparam ErrorOut Error output iterator type.
	/// @param error_out Error output iterator.
	/// @param path Localization file path.
	/// @exception file_not_found If the file was not found.
	/// @exception file_open_error If opening the file failed.
	/// @return Localization script parsing result.
	template <std::output_iterator<localization_script_error> ErrorOut>
	[[nodiscard]] parse_localization_script_result<ErrorOut> parse_localization_script_file(ErrorOut error_out,
																							const std::filesystem::path& path)
	{
		std::ifstream file{open_file_r(path)};
		return parse_localization_script(error_out, std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{},
										 TR_PATH_CSTR(std::filesystem::canonical(path)));
	}

	/// Parses localization script into a localization map.
	/// @tparam ErrorOut Error output iterator type.
	/// @tparam Iterator Character input iterator type.
	/// @tparam Sentinel Sentinel type for `Iterator`.
	/// @param out Localization map to output to.
	/// @param error_out Error output iterator.
	/// @param begin Iterator to the beginning of the character range.
	/// @param end Iterator to the end of the character range.
	/// @param chunk_name Name of the parsed chunk.
	/// @return End error output iterator.
	template <std::output_iterator<localization_script_error> ErrorOut, input_iterator_to_convertible_to<char> Iterator,
			  std::sentinel_for<Iterator> Sentinel>
	ErrorOut parse_localization_script_to(localization_map& out, ErrorOut error_out, Iterator begin, Sentinel end,
										  std::string_view chunk_name = "<unnamed>")
	{
		auto result{internal::localization::parse_localization_script(error_out, begin, end, chunk_name)};
		out.update(result.pairs);
		return result.error_out;
	}

	/// Parses localization script into a localization map.
	/// @tparam ErrorOut Error output iterator type.
	/// @tparam Range Character input range.
	/// @param out Localization map to output to.
	/// @param error_out Error output iterator.
	/// @param range Character range to parse.
	/// @param chunk_name Name of the parsed chunk.
	/// @return End error output iterator.
	template <std::output_iterator<localization_script_error> ErrorOut, input_range_to_convertible_to<char> Range>
	ErrorOut parse_localization_script_to(localization_map& out, ErrorOut error_out, Range&& range,
										  std::string_view chunk_name = "<unnamed>")
	{
		return parse_localization_script_to(out, error_out, std::ranges::begin(range), std::ranges::end(range), chunk_name);
	}

	/// Parses a localization script file into a localization map.
	/// @tparam ErrorOut Error output iterator type.
	/// @param out Localization map to output to.
	/// @param error_out Error output iterator.
	/// @param path Localization file path.
	/// @exception file_not_found If the file was not found.
	/// @exception file_open_error If opening the file failed.
	/// @return End error output iterator.
	template <std::output_iterator<localization_script_error> ErrorOut>
	ErrorOut parse_localization_script_file_to(localization_map& out, ErrorOut error_out, const std::filesystem::path& path)
	{
		std::ifstream file{open_file_r(path)};
		return parse_localization_script_to(out, error_out, std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{},
											TR_PATH_CSTR(std::filesystem::canonical(path)));
	}

	/// @}
} // namespace tr