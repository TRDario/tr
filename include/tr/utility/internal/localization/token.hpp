/// @file
/// @brief Provides localization tokens.

#pragma once
#include <tr/utility/localization_script_error.hpp>
#include <tr/utility/variant.hpp>

//

namespace tr::internal::localization
{
	/// @brief Symbol token.
	struct symbol_token
	{
		/// @brief Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::symbol};

		//

		/// @brief Number of the line the token is on.
		int line;

		/// @brief Name of the symbol.
		std::string name;
	};

	/// @brief String token.
	struct string_token
	{
		/// @brief Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::string};

		//

		/// @brief Number of the line the token is on.
		int line;

		/// @brief String value.
		std::string value;
	};

	/// @brief Equals sign token.
	struct equals_token
	{
		/// @brief Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::equals};

		//

		/// @brief Number of the line the token is on.
		int line;
	};

	/// @brief Opening brace token.
	struct opening_brace_token
	{
		/// @brief Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::opening_brace};

		//

		/// @brief Number of the line the token is on.
		int line;
	};

	/// @brief Closing brace token.
	struct closing_brace_token
	{
		/// @brief Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::closing_brace};

		//

		/// @brief Number of the line the token is on.
		int line;
	};

	/// @brief Concept checking for a localization token.
	/// @tparam Target Type to check for being a localization token.
	template <typename Target>
	concept token = one_of<Target, symbol_token, string_token, equals_token, opening_brace_token, closing_brace_token>;
} // namespace tr::internal::localization