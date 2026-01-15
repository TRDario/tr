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
#include "common.hpp"

//////////////////////////////////////////////////////////////// INTERFACE ////////////////////////////////////////////////////////////////

namespace tr {
	// Inplace-allocated, fixed-capacity vector.
	template <class T, usize S> class static_vector {
	  public:
		using size_type = size_type_t<S>;
		using difference_type = std::make_signed_t<size_type>;
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using iterator = T*;
		using const_iterator = const T*;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		// Creates an empty vector.
		constexpr static_vector() = default;
		// Creates a vector with a certain number of default-initialized values.
		static_vector(size_type size)
			requires(std::default_initializable<T>);
		// Creates a vector with a certain number of copies of a value.
		static_vector(size_type size, const T& v)
			requires(std::copy_constructible<T>);
		// Creates a vector from an iterator pair.
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
		static_vector(It first, It last);
		// Creates a vector from a range.
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
		static_vector(Range&& range);
		// Creates a vector from an initializer list.
		static_vector(std::initializer_list<T> init)
			requires(std::copy_constructible<T>);
		// Copies the vector.
		static_vector(const static_vector& r)
			requires(std::copy_constructible<T>);
		// Moves the vector.
		static_vector(static_vector&& r) noexcept(std::is_nothrow_move_constructible_v<T>)
			requires(std::move_constructible<T>);
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
		iterator insert(const_iterator where, const T& v)
			requires(std::copy_constructible<T>);
		// Inserts a value into the vector.
		iterator insert(const_iterator where, T&& v)
			requires(std::move_constructible<T>);
		// Inserts an iterator pair range into the string.
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
		iterator insert(const_iterator where, It first, It last);
		// Inserts a range into the string.
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
		iterator insert(const_iterator where, Range&& range);
		// Inserts an initializer list into the string.
		iterator insert(const_iterator where, std::initializer_list<T> init)
			requires(std::copy_constructible<T>);
		// Emplaces a value in the vector.
		template <class... Args>
			requires(std::constructible_from<T, Args...>)
		iterator emplace(const_iterator where, Args&&... args);
		// Erases a value from the vector.
		iterator erase(const_iterator where);
		// Erases a range of values from the vector.
		iterator erase(const_iterator first, const_iterator last);

		// Appends a value to the vector.
		reference push_back(const T& v)
			requires(std::copy_constructible<T>);
		// Appends a value to the vector.
		reference push_back(T&& v)
			requires(std::move_constructible<T>);
		// Emplaces a value at the end of the vector.
		template <class... Args>
			requires(std::constructible_from<T, Args...>)
		reference emplace_back(Args&&... args);
		// Appends an iterator pair range to the vector.
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
		iterator append(It first, It last);
		// Appends a range to the vector.
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
		iterator append(Range&& range);
		// Appends an initializer list to the vector.
		iterator append(std::initializer_list<T> init)
			requires(std::copy_constructible<T>);
		// Erases a value from the end of the vector.
		void pop_back();
		// Resizes the vector, default-constructing values if necessary.
		void resize(size_type size)
			requires(std::default_initializable<T>);
		// Resizes the vector, copy-constructing values if necessary.
		void resize(size_type size, const T& v)
			requires(std::copy_constructible<T>);

	  private:
		// The buffer where elements are stored.
		alignas(T) std::byte m_buffer[sizeof(T) * S]{};
		// The number of elements in the vector.
		size_type m_size{};
	};

	// Static vector binary reader.
	template <binary_constructible T, usize S> struct binary_reader<static_vector<T, S>> {
		void operator()(std::istream& is, static_vector<T, S>& out) const;
	};
	// Static vector binary writer.
	template <binary_writable T, usize S> struct binary_writer<static_vector<T, S>> {
		void operator()(std::ostream& os, const static_vector<T, S>& in) const;
	};
} // namespace tr

////////////////////////////////////////////////////////////// IMPLEMENTATION /////////////////////////////////////////////////////////////

#include "static_vector_impl.hpp" // IWYU pragma: keep