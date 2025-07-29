#pragma once
#include "hashmap.hpp"

namespace tr {
	// Error thrown when localization loading fails.
	class localization_load_error : public exception {
	  public:
		// Constructs an exception.
		localization_load_error(std::string&& description);

		// Gets the name of the error.
		std::string_view name() const override;
		// Gets the description of the error.
		std::string_view description() const override;
		// Gets further details about the error.
		std::string_view details() const override;

	  private:
		// The description of the error.
		std::string description_str;
	};

	// Wrapper over a string -> string hashmap with support for a custom file format for use in localization.
	class localization_map {
	  public:
		// Constructs an empty localization map.
		localization_map() = default;
		// Copies a localization map.
		localization_map(const string_hash_map<std::string>& map);
		// Moves a localization map.
		localization_map(string_hash_map<std::string>&& map);

		// Clears the localization map.
		void clear();
		// Loads a localization file, returning any non-fatal errors.
		// May throw: tr::localization_load_error.
		std::vector<std::string> load(const std::filesystem::path& path);

		// Gets whether a key has a corresponding localization string in the map.
		bool contains(std::string_view key) const;
		// Gets a localization string associated with a key.
		std::string_view operator[](std::string_view key) const;

	  private:
		string_hash_map<std::string> map;
	};
} // namespace tr
