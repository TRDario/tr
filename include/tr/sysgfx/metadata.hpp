/// @file
/// @brief Provides `tr::metadata` and `tr::set_metadata()`.

#pragma once
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// Application metadata.
	struct metadata
	{
		/// Supported application types.
		enum class application_type
		{
			/// The application is a game.
			game,

			/// The application type is unspecified.
			application
		};

		//

		/// Name of the application.
		zstring_view name{};

		/// Version of the application.
		zstring_view version{};

		/// Identifier of the application.
		zstring_view identifier{};

		/// Developer of the application.
		zstring_view developer{};

		/// Short copyright notice.
		zstring_view copyright{};

		/// URL relevant to the application.
		zstring_view url{};

		/// Application type.
		application_type type{application_type::application};
	};

	//

	/// @name Metadata
	/// @{

	/// Sets application metadata.
	/// @note This function must be called at least once before you can use `tr::user_directory()`.
	/// @param metadata Application metadata to set.
	void set_metadata(const metadata& metadata);

	/// @}
} // namespace tr