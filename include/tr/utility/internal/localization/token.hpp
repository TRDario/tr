/// @file
/// @brief Provides localization tokens.

#pragma once
#include <tr/utility/localization_script_error.hpp>
#include <tr/utility/variant.hpp>

//

namespace tr::internal::localization
{
	/// Symbol token.
	struct symbol_token
	{
		/// Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::symbol};

		//

		/// Number of the line the token is on.
		int line;

		/// Name of the symbol.
		std::string name;
	};

	/// String token.
	struct string_token
	{
		/// Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::string};

		//

		/// Number of the line the token is on.
		int line;

		/// String value.
		std::string value;
	};

	/// Equals sign token.
	struct equals_token
	{
		/// Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::equals};

		//

		/// Number of the line the token is on.
		int line;
	};

	/// Opening brace token.
	struct opening_brace_token
	{
		/// Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::opening_brace};

		//

		/// Number of the line the token is on.
		int line;
	};

	/// Closing brace token.
	struct closing_brace_token
	{
		/// Token type enumerator equivalent of this type.
		static constexpr localization_token_type type{localization_token_type::closing_brace};

		//

		/// Number of the line the token is on.
		int line;
	};

	/// Concept denoting a localization token.
	template <typename T>
	concept token = one_of<T, symbol_token, string_token, equals_token, opening_brace_token, closing_brace_token>;
} // namespace tr::internal::localization