# LLDB Formatters

tr provides LLDB formatters for several datatypes and containers used and provided by the library. This includes:
 - glm vector types
 - boost::unordered containers
 - most tr graphical objects
 - `tr::angle`
 - `tr::atlas_entries`
 - `tr::basic_zstring_view`
 - `tr::enum_wrapper`
 - `tr::handle`
 - `tr::inplace_string` and its iterators
 - `tr::inplace_vector` and its iterators
 - `tr::localization_map`
 - `tr::lock_free_queue`
 - `tr::ref`/`tr::opt_ref`
 - `tr::rgb8`/`tr::rgbf`/`tr::rgba8`/`tr::rgbaf`/`tr::hsv`
 - `tr::string_literal`
 - `tr::string_pool`
 - `tr::texture_view`/`tr::mutable_texture_view`
 - `tr::utf8::iterator`/`tr::utf8::indexed_iterator`

The easiest way to include these formatters is to insert `command script import <path to ptr>/lldb/tr.py` inside `.lldbinit` in your
project or home directory to automatically include the full list on startup.