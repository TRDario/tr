///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Provides an inplace-allocated, fixed capacity vector class.                                                                           //
//                                                                                                                                       //
// tr::static_vector<T, S> is fundamentally a vector interface implemented over a fixed-size in-place array. The entire std::vector      //
// interface is implemented except functions relating to memory allocation, though errors do not throw exceptions and there are never    //
// checks in release mode. tr::static_vector is also binary readable and writable.                                                       //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "binary_io.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Inplace-allocated, fixed-capacity vector.
	template <typename Element, usize Capacity> class static_vector {
	  public:
		using size_type = size_type_t<Capacity>;
		using difference_type = std::make_signed_t<size_type>;
		using value_type = Element;
		using reference = Element&;
		using const_reference = const Element&;
		using pointer = Element*;
		using const_pointer = const Element*;
		using iterator = Element*;
		using const_iterator = const Element*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// Creates an empty vector.
		constexpr static_vector() = default;
		// Creates a vector with a certain number of default-initialized values.
		static_vector(size_type size)
			requires(std::default_initializable<Element>);
		// Creates a vector with a certain number of copies of a value.
		static_vector(size_type size, const Element& v)
			requires(std::copy_constructible<Element>);
		// Creates a vector from an iterator pair.
		template <std::input_iterator Iterator>
			requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, Element>)
		static_vector(Iterator first, Iterator last);
		// Creates a vector from a range.
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, Element>)
		static_vector(Range&& range);
		// Creates a vector from an initializer list.
		static_vector(std::initializer_list<Element> init)
			requires(std::copy_constructible<Element>);
		// Copies the vector.
		static_vector(const static_vector& r)
			requires(std::copy_constructible<Element>);
		// Moves the vector.
		static_vector(static_vector&& r) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires(std::move_constructible<Element>);
		// Destroys the vector.
		~static_vector();

		// Gets a reference to a value in the vector.
		reference at(size_type offset);
		// Gets a reference to a value in the vector.
		const_reference at(size_type offset) const;
		// Gets a reference to a value in the vector.
		reference operator[](size_type offset);
		// Gets a reference to a value in the vector.
		const_reference operator[](size_type offset) const;
		// Gets a reference to the first value in the vector.
		reference front();
		// Gets a reference to the first value in the vector.
		const_reference front() const;
		// Gets a reference to the last value in the vector.
		reference back();
		// Gets a reference to the last value in the vector.
		const_reference back() const;
		// Gets a pointer to the data of the vector.
		pointer data();
		// Gets a pointer to the data of the vector.
		const_pointer data() const;

		// Gets an iterator to the beginning of the vector.
		iterator begin();
		// Gets an iterator to the beginning of the vector.
		const_iterator begin() const;
		// Gets an iterator to the beginning of the vector.
		const_iterator cbegin() const;
		// Gets an iterator to the end of the vector.
		iterator end();
		// Gets an iterator to the end of the vector.
		const_iterator end() const;
		// Gets an iterator to the end of the vector.
		const_iterator cend() const;
		// Gets an iterator to the reverse beginning of the vector.
		reverse_iterator rbegin();
		// Gets an iterator to the reverse beginning of the vector.
		const_reverse_iterator rbegin() const;
		// Gets an iterator to the reverse beginning of the vector.
		const_reverse_iterator crbegin() const;
		// Gets an iterator to the reverse end of the vector.
		reverse_iterator rend();
		// Gets an iterator to the reverse end of the vector.
		const_reverse_iterator rend() const;
		// Gets an iterator to the reverse end of the vector.
		const_reverse_iterator crend() const;

		// Gets whether the vector is empty.
		constexpr bool empty() const;
		// Gets the current size of the vector.
		constexpr size_type size() const;
		// Gets the maximum allowed size of the vector.
		static constexpr size_type max_size();

		// Erases all values from the vector.
		void clear();
		// Inserts a value into the vector.
		iterator insert(const_iterator where, const Element& v)
			requires(std::copy_constructible<Element>);
		// Inserts a value into the vector.
		iterator insert(const_iterator where, Element&& v)
			requires(std::move_constructible<Element>);
		// Inserts an iterator pair range into the string.
		template <std::input_iterator Iterator>
			requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, Element>)
		iterator insert(const_iterator where, Iterator first, Iterator last);
		// Inserts a range into the string.
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, Element>)
		iterator insert(const_iterator where, Range&& range);
		// Inserts an initializer list into the string.
		iterator insert(const_iterator where, std::initializer_list<Element> init)
			requires(std::copy_constructible<Element>);
		// Emplaces a value in the vector.
		template <typename... Args>
			requires(std::constructible_from<Element, Args...>)
		iterator emplace(const_iterator where, Args&&... args);
		// Erases a value from the vector.
		iterator erase(const_iterator where);
		// Erases a range of values from the vector.
		iterator erase(const_iterator first, const_iterator last);

		// Appends a value to the vector.
		reference push_back(const Element& v)
			requires(std::copy_constructible<Element>);
		// Appends a value to the vector.
		reference push_back(Element&& v)
			requires(std::move_constructible<Element>);
		// Emplaces a value at the end of the vector.
		template <typename... Args>
			requires(std::constructible_from<Element, Args...>)
		reference emplace_back(Args&&... args);
		// Appends an iterator pair range to the vector.
		template <std::input_iterator Iterator>
			requires(std::same_as<typename std::iterator_traits<Iterator>::value_type, Element>)
		iterator append(Iterator first, Iterator last);
		// Appends a range to the vector.
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, Element>)
		iterator append(Range&& range);
		// Appends an initializer list to the vector.
		iterator append(std::initializer_list<Element> init)
			requires(std::copy_constructible<Element>);
		// Erases a value from the end of the vector.
		void pop_back();
		// Resizes the vector, default-constructing values if necessary.
		void resize(size_type size)
			requires(std::default_initializable<Element>);
		// Resizes the vector, copy-constructing values if necessary.
		void resize(size_type size, const Element& v)
			requires(std::copy_constructible<Element>);

	  private:
		// The buffer where elements are stored.
		alignas(Element) std::byte m_buffer[sizeof(Element) * Capacity]{};
		// The number of elements in the vector.
		size_type m_size{};
	};

	// Static vector binary reader.
	template <binary_constructible Element, usize Capacity> struct binary_reader<static_vector<Element, Capacity>> {
		void operator()(std::istream& is, static_vector<Element, Capacity>& out) const;
	};
	// Static vector binary writer.
	template <binary_writable Element, usize Capacity> struct binary_writer<static_vector<Element, Capacity>> {
		void operator()(std::ostream& os, const static_vector<Element, Capacity>& in) const;
	};
} // namespace tr

#include "impl/static_vector.hpp" // IWYU pragma: export