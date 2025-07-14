#pragma once
#include "hashmap.hpp"

namespace tr {
	// Error thrown when localization loading fails.
	class localization_load_error : public exception {
	  public:
		// Constructs an exception.
		localization_load_error(std::string_view location, std::string&& description) noexcept;

		// Gets the name of the error.
		std::string_view name() const noexcept override;
		// Gets the description of the error.
		std::string_view description() const noexcept override;
		// Gets further details about the error.
		std::string_view details() const noexcept override;

	  private:
		// The description of the error.
		std::string _description;
	};

	// Wrapper over a string -> string hashmap with support for a custom file format for use in localization.
	class localization_map {
	  public:
		// Constructs an empty localization map.
		localization_map() noexcept = default;
		// Copies a localization map.
		localization_map(const string_hash_map<std::string>& map) noexcept;
		// Moves a localization map.
		localization_map(string_hash_map<std::string>&& map) noexcept;

		//

		// Clears the localization map.
		void clear() noexcept;
		// Loads a localization file, returning any non-fatal errors.
		// May throw: tr::localization_load_error.
		std::vector<std::string> load(const std::filesystem::path& path);

		// Gets whether a key has a corresponding localization string in the map.
		bool contains(std::string_view key) const noexcept;
		// Gets a localization string associated with a key.
		std::string_view operator[](std::string_view key) const noexcept;

	  private:
		string_hash_map<std::string> _map;
	};
} // namespace tr
