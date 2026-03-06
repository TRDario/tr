///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an inplace-allocated, fixed capacity string class.                                                                           //
//                                                                                                                                       //
// tr::static_string<T, S> is fundamentally a string interface implemented over a fixed-size in-place array of characters. Almost the    //
// entire std::string interface is implemented, functions relating to memory allocation are not implemented, and neither is .c_str(), as //
// tr::static_string is not guaranteed to be NUL-terminated. Additionally, errors do not throw exceptions and there are never checks in  //
// release mode. tr::static_string is binary readable and writable, as well as formattable.                                              //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Inplace-allocated fixed-capacity string.
	template <usize Capacity> class static_string {
	  public:
		using value_type = char;
		using size_type = size_type_t<Capacity>;
		using difference_type = std::make_signed_t<size_type>;
		using reference = char&;
		using const_reference = const char&;
		using pointer = char*;
		using const_pointer = const char*;
		using iterator = pointer;
		using const_iterator = const_pointer;

		// Creates an empty string.
		constexpr static_string() = default;
		// Creates a string with a starting size.
		constexpr static_string(size_type size, char chr = '\0');
		// Creates a string by copying from a string literal.
		template <usize Size> constexpr static_string(const char (&literal)[Size]);
		// Creates a string by copying from another string.
		constexpr static_string(std::string_view str);
		// Creates a string by copying from another string.
		template <std::convertible_to<std::string_view> String> constexpr static_string(String&& str);

		// Gets a string view over the string.
		constexpr operator std::string_view() const;
		// Gets a standard string copy of the string.
		constexpr explicit operator std::string() const;

		// Gets a reference to a character in the string.
		constexpr reference operator[](size_type offset);
		// Gets a reference to a character in the string.
		constexpr const_reference operator[](size_type offset) const;
		// Gets a reference to the first character in the string.
		constexpr reference front();
		// Gets a reference to the first character in the string.
		constexpr const_reference front() const;
		// Gets a reference to the last character in the string.
		constexpr reference back();
		// Gets a reference to the last character in the string.
		constexpr const_reference back() const;
		// Gets a pointer to the data of the string.
		constexpr pointer data();
		// Gets a pointer to the data of the string.
		constexpr const_pointer data() const;

		// Gets an iterator to the beginning of the string.
		constexpr iterator begin();
		// Gets an iterator to the beginning of the string.
		constexpr const_iterator begin() const;
		// Gets an iterator to the beginning of the string.
		constexpr const_iterator cbegin() const;
		// Gets an iterator to the end of the string.
		constexpr iterator end();
		// Gets an iterator to the end of the string.
		constexpr const_iterator end() const;
		// Gets an iterator to the end of the string.
		constexpr const_iterator cend() const;

		// Gets whether the string is empty.
		constexpr bool empty() const;
		// Gets the current size of the string.
		constexpr size_type size() const;
		// Gets the maximum allowed size of the string.
		static constexpr size_type max_size();

		// Erases all characters from the string.
		constexpr void clear();
		// Appends a character to the string.
		constexpr void push_back(char chr);
		// Appends a character to the string.
		constexpr void append(char chr);
		// Appends a character to the string.
		constexpr static_string& operator+=(char chr);
		// Appends a range to the string.
		template <std::input_iterator Iterator>
			requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, char>)
		constexpr void append(Iterator begin, Iterator end);
		// Appends a substring to the string.
		constexpr void append(std::string_view str);
		// Appends a substring to the string.
		constexpr static_string& operator+=(std::string_view str);
		// Inserts a character into the string.
		constexpr void insert(const_iterator where, char chr);
		// Inserts a range into the string.
		template <std::input_iterator Iterator>
			requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, char>)
		constexpr void insert(const_iterator where, Iterator first, Iterator last);
		// Inserts a substring into the string.
		constexpr void insert(const_iterator where, std::string_view str);
		// Erases a character from the end of the string.
		constexpr void pop_back();
		// Erases a character from the string.
		constexpr void erase(const_iterator where);
		// Erases a portion of the string.
		constexpr void erase(const_iterator start, const_iterator end);
		// Resizes the string, filling it with a character if necessary.
		constexpr void resize(size_type size, char chr = '\0');

	  private:
		std::array<char, Capacity> m_buffer{};
		size_type m_size{0};
	};
	template <usize Capacity>
	constexpr std::strong_ordering operator<=>(const static_string<Capacity>& l, const static_string<Capacity>& r);
	template <usize Capacity> constexpr bool operator==(const static_string<Capacity>& l, const static_string<Capacity>& r);
	template <usize Capacity> constexpr std::strong_ordering operator<=>(const static_string<Capacity>& l, const std::string_view& r);
	template <usize Capacity> constexpr bool operator==(const static_string<Capacity>& l, const std::string_view& r);
	template <usize Capacity> constexpr std::strong_ordering operator<=>(const std::string_view& l, const static_string<Capacity>& r);
	template <usize Capacity> constexpr bool operator==(const std::string_view& l, const static_string<Capacity>& r);

	// Output stream formatter for static strings.
	template <usize Capacity> inline std::ostream& operator<<(std::ostream& os, const static_string<Capacity>& str);

	// Static string binary reader.
	template <usize Capacity> struct binary_reader<static_string<Capacity>> {
		void operator()(std::istream& is, static_string<Capacity>& out) const;
	};
	// Static string binary writer.
	template <usize Capacity> struct binary_writer<static_string<Capacity>> {
		void operator()(std::ostream& os, const static_string<Capacity>& in) const;
	};

} // namespace tr

// Formatter for static strings.
template <tr::usize Capacity> struct TR_FMT::formatter<tr::static_string<Capacity>> : TR_FMT::formatter<std::string_view> {};

#include "impl/static_string.hpp" // IWYU pragma: export