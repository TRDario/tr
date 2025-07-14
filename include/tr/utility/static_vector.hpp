#pragma once
#include "common.hpp"

namespace tr {
	/******************************************************************************************************************
	 * Inplace-allocated fixed-capacity vector.
	 *****************************************************************************************************************/
	template <class T, std::size_t S> class static_vector {
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

		//

		/**************************************************************************************************************
		 * Creates an empty vector.
		 *************************************************************************************************************/
		constexpr static_vector() noexcept = default;

		/**************************************************************************************************************
		 * Creates a vector with a certain number of default-initialized values.
		 *
		 * @param[in] size The number of values to emplace in the vector.
		 *************************************************************************************************************/
		static_vector(size_type size) noexcept(std::is_nothrow_default_constructible_v<T>)
			requires(std::default_initializable<T>);

		/**************************************************************************************************************
		 * Creates a vector with a certain number of copies of a value.
		 *
		 * @param[in] size The number of copies to emplace in the vector.
		 * @param[in] v The value to copy.
		 *************************************************************************************************************/
		static_vector(size_type size, const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

		/**************************************************************************************************************
		 * Creates a vector from an iterator pair.
		 *
		 * @param[in] first Iterator to the first element.
		 * @param[in] last Iterator to one past the last element.
		 *************************************************************************************************************/
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
		static_vector(It first, It last);

		/**************************************************************************************************************
		 * Creates a vector from a range.
		 *
		 * @param[in] range The range to create the vector from.
		 *************************************************************************************************************/
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
		static_vector(Range&& range);

		/**************************************************************************************************************
		 * Creates a vector from an initializer list.
		 *
		 * @param[in] init The list to create the vector from.
		 *************************************************************************************************************/
		static_vector(std::initializer_list<T> init) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

		/**************************************************************************************************************
		 * Copies the vector.
		 *
		 * @param[in] r The vector to copy from.
		 *************************************************************************************************************/
		static_vector(const static_vector& r) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

		/**************************************************************************************************************
		 * Moves the vector.
		 *
		 * @param[in] r The vector to move from.
		 *************************************************************************************************************/
		static_vector(static_vector&& r) noexcept(std::is_nothrow_move_constructible_v<T>)
			requires(std::move_constructible<T>);

		/**************************************************************************************************************
		 * Destroys the vector.
		 *************************************************************************************************************/
		~static_vector() noexcept(std::is_nothrow_destructible_v<T>);

		//

		/**************************************************************************************************************
		 * Gets a reference to a value in the vector.
		 *
		 * @pre @em offset must be less than the size of the vector.
		 *
		 * @return A reference to a value in the vector at the given offset.
		 *************************************************************************************************************/
		reference at(size_type offset) noexcept;

		/**************************************************************************************************************
		 * Gets a reference to a value in the vector.
		 *
		 * @pre @em offset must be less than the size of the vector.
		 *
		 * @return A reference to a value in the vector at the given offset.
		 *************************************************************************************************************/
		const_reference at(size_type offset) const noexcept;

		/**************************************************************************************************************
		 * Gets a reference to a value in the vector.
		 *
		 * @pre @em offset must be less than the size of the vector.
		 *
		 * @return A reference to a value in the vector at the given offset.
		 *************************************************************************************************************/
		reference operator[](size_type offset) noexcept;

		/**************************************************************************************************************
		 * Gets a reference to a value in the vector.
		 *
		 * @pre @em offset must be less than the size of the vector.
		 *
		 * @return A reference to a value in the vector at the given offset.
		 *************************************************************************************************************/
		const_reference operator[](size_type offset) const noexcept;

		/**************************************************************************************************************
		 * Gets a reference to the first value in the vector.
		 *
		 * @pre The vector must not be empty.
		 *
		 * @return A reference to the first value in the vector.
		 *************************************************************************************************************/
		reference front() noexcept;

		/**************************************************************************************************************
		 * Gets a reference to the first value in the vector.
		 *
		 * @pre The vector must not be empty.
		 *
		 * @return A reference to the first value in the vector.
		 *************************************************************************************************************/
		const_reference front() const noexcept;

		/**************************************************************************************************************
		 * Gets a reference to the last value in the vector.
		 *
		 * @pre The vector must not be empty.
		 *
		 * @return A reference to the last value in the vector.
		 *************************************************************************************************************/
		reference back() noexcept;

		/**************************************************************************************************************
		 * Gets a reference to the last value in the vector.
		 *
		 * @pre The vector must not be empty.
		 *
		 * @return A reference to the last value in the vector.
		 *************************************************************************************************************/
		const_reference back() const noexcept;

		/**************************************************************************************************************
		 * Gets a pointer to the data of the vector.
		 *
		 * @return A pointer to the data of the vector.
		 *************************************************************************************************************/
		pointer data() noexcept;

		/**************************************************************************************************************
		 * Gets a pointer to the data of the vector.
		 *
		 * @return A pointer to the data of the vector.
		 *************************************************************************************************************/
		const_pointer data() const noexcept;

		//

		/**************************************************************************************************************
		 * Gets an iterator to the beginning of the vector.
		 *
		 * @return An iterator to the beginning of the vector.
		 *************************************************************************************************************/
		iterator begin() noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the beginning of the vector.
		 *
		 * @return An iterator to the beginning of the vector.
		 *************************************************************************************************************/
		const_iterator begin() const noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the beginning of the vector.
		 *
		 * @return An iterator to the beginning of the vector.
		 *************************************************************************************************************/
		const_iterator cbegin() const noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the end of the vector.
		 *
		 * @return An iterator to the end of the vector.
		 *************************************************************************************************************/
		iterator end() noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the end of the vector.
		 *
		 * @return An iterator to the end of the vector.
		 *************************************************************************************************************/
		const_iterator end() const noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the end of the vector.
		 *
		 * @return An iterator to the end of the vector.
		 *************************************************************************************************************/
		const_iterator cend() const noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the reverse beginning of the vector.
		 *
		 * @return An iterator to the reverse beginning of the vector.
		 *************************************************************************************************************/
		reverse_iterator rbegin() noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the reverse beginning of the vector.
		 *
		 * @return An iterator to the reverse beginning of the vector.
		 *************************************************************************************************************/
		const_reverse_iterator rbegin() const noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the reverse beginning of the vector.
		 *
		 * @return An iterator to the reverse beginning of the vector.
		 *************************************************************************************************************/
		const_reverse_iterator crbegin() const noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the reverse end of the vector.
		 *
		 * @return An iterator to the reverse end of the vector.
		 *************************************************************************************************************/
		reverse_iterator rend() noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the reverse end of the vector.
		 *
		 * @return An iterator to the reverse end of the vector.
		 *************************************************************************************************************/
		const_reverse_iterator rend() const noexcept;

		/**************************************************************************************************************
		 * Gets an iterator to the reverse end of the vector.
		 *
		 * @return An iterator to the reverse end of the vector.
		 *************************************************************************************************************/
		const_reverse_iterator crend() const noexcept;

		//

		/**************************************************************************************************************
		 * Gets whether the vector is empty.
		 *
		 * @return True if the vector is empty, false otherwise.
		 *************************************************************************************************************/
		constexpr bool empty() const noexcept;

		/**************************************************************************************************************
		 * Gets the current size of the vector.
		 *
		 * @return The current size of the vector.
		 *************************************************************************************************************/
		constexpr size_type size() const noexcept;

		/**************************************************************************************************************
		 * Gets the maximum allowed size of the vector.
		 *
		 * @return The maximum allowed size of the vector.
		 *************************************************************************************************************/
		static constexpr size_type max_size() noexcept;

		//

		/**************************************************************************************************************
		 * Erases all values from the vector.
		 *************************************************************************************************************/
		void clear() noexcept(std::is_nothrow_destructible_v<T>);

		/**************************************************************************************************************
		 * Inserts a value into the vector.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] where Iterator to where the value should be inserted.
		 * @param[in] v The value to insert.
		 *************************************************************************************************************/
		iterator insert(const_iterator where, const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

		/**************************************************************************************************************
		 * Inserts a value into the vector.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] where Iterator to where the value should be inserted.
		 * @param[in] v The value to insert.
		 *************************************************************************************************************/
		iterator insert(const_iterator where, T&& v) noexcept(std::is_nothrow_move_constructible_v<T>)
			requires(std::move_constructible<T>);

		/**************************************************************************************************************
		 * Inserts an iterator pair range into the string.
		 *
		 * @pre size() + std::distance(first, last) <= max_size().
		 *
		 * @param[in] where Iterator to where the range should be inserted.
		 * @param[in] first Iterator to the first value to insert.
		 * @param[in] last Iterator to one past the last value to insert.
		 *************************************************************************************************************/
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
		iterator insert(const_iterator where, It first, It last);

		/**************************************************************************************************************
		 * Inserts a range into the string.
		 *
		 * @pre size() + std::ranges::size(range) <= max_size().
		 *
		 * @param[in] where Iterator to where the range should be inserted.
		 * @param[in] range The range to insert.
		 *************************************************************************************************************/
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
		iterator insert(const_iterator where, Range&& range);

		/**************************************************************************************************************
		 * Inserts an initializer list into the string.
		 *
		 * @pre size() + std::ranges::size(range) <= max_size().
		 *
		 * @param[in] where Iterator to where the range should be inserted.
		 * @param[in] init The list to insert.
		 *************************************************************************************************************/
		iterator insert(const_iterator where, std::initializer_list<T> init) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

		/**************************************************************************************************************
		 * Emplaces a value in the vector.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] args The arguments that will be passed to the constructor of the value.
		 *************************************************************************************************************/
		template <class... Args>
			requires(std::constructible_from<T, Args...>)
		iterator emplace(const_iterator where, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>);

		/**************************************************************************************************************
		 * Erases a value from the vector.
		 *
		 * @param[in] where Iterator to the deleted value.
		 *
		 * @return Iterator to one past the deleted value.
		 *************************************************************************************************************/
		iterator erase(const_iterator where) noexcept(std::is_nothrow_move_constructible_v<T>);

		/**************************************************************************************************************
		 * Erases a range of values from the vector.
		 *
		 * @param[in] first Iterator to the first deleted value.
		 * @param[in] last Iterator to one past the last deleted value.
		 *
		 * @return Iterator to one past the deleted values.
		 *************************************************************************************************************/
		iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_move_constructible_v<T>);

		/**************************************************************************************************************
		 * Appends a value to the vector.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] v The value to append.
		 *************************************************************************************************************/
		reference push_back(const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

		/**************************************************************************************************************
		 * Appends a value to the vector.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] v The value to append.
		 *************************************************************************************************************/
		reference push_back(T&& v) noexcept(std::is_nothrow_move_constructible_v<T>)
			requires(std::move_constructible<T>);

		/**************************************************************************************************************
		 * Emplaces a value at the end of the vector.
		 *
		 * @pre size() != max_size().
		 *
		 * @param[in] args The arguments that will be passed to the constructor of the value.
		 *************************************************************************************************************/
		template <class... Args>
			requires(std::constructible_from<T, Args...>)
		reference emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>);

		/**************************************************************************************************************
		 * Appends an iterator pair range to the vector.
		 *
		 * @pre size() + std::distance(first, last) <= max_size().
		 *
		 * @param[in] first Iterator to the first value to append.
		 * @param[in] last Iterator to one past the last value to append.
		 *************************************************************************************************************/
		template <std::input_iterator It>
			requires(std::same_as<typename std::iterator_traits<It>::value_type, T>)
		iterator append(It first, It last);

		/**************************************************************************************************************
		 * Appends a range to the vector.
		 *
		 * @pre size() + std::ranges::size(range) <= max_size().
		 *
		 * @param[in] range The range to append to the vector.
		 *************************************************************************************************************/
		template <std::ranges::input_range Range>
			requires(std::same_as<typename std::ranges::range_value_t<Range>, T>)
		iterator append(Range&& range);

		/**************************************************************************************************************
		 * Appends an initializer list to the vector.
		 *
		 * @pre size() + list.size() <= max_size().
		 *
		 * @param[in] init The list to append to the vector.
		 *************************************************************************************************************/
		iterator append(std::initializer_list<T> init) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

		/**************************************************************************************************************
		 * Erases a value from the end of the vector.
		 *************************************************************************************************************/
		void pop_back() noexcept;

		/**************************************************************************************************************
		 * Resizes the vector, default-constructing values if necessary.
		 *
		 * @param[in] size
		 * @parblock
		 * The new size of the vector.
		 *
		 * @pre @em size must be smaller than or equal to max_size().
		 * @endparblock
		 *************************************************************************************************************/
		void resize(size_type size) noexcept(std::is_nothrow_default_constructible_v<T>)
			requires(std::default_initializable<T>);

		/**************************************************************************************************************
		 * Resizes the vector, copy-constructing values if necessary.
		 *
		 * @param[in] size
		 * @parblock
		 * The new size of the vector.
		 *
		 * @pre @em size must be smaller than or equal to max_size().
		 * @endparblock
		 * @param[in] v The value to fill the vector with.
		 *************************************************************************************************************/
		void resize(size_type size, const T& v) noexcept(std::is_nothrow_copy_constructible_v<T>)
			requires(std::copy_constructible<T>);

	  private:
		alignas(T) std::byte _buffer[sizeof(T) * S]{};
		size_type _size{};
	};

	/******************************************************************************************************************
	 * Static vector binary reader.
	 *****************************************************************************************************************/
	template <binary_constructible T, std::size_t S> struct binary_reader<static_vector<T, S>> {
		static void read_from_stream(std::istream& is, static_vector<T, S>& out);
		static std::span<const std::byte> read_from_span(std::span<const std::byte> span, static_vector<T, S>& out);
	};

	/******************************************************************************************************************
	 * Static vector binary writer.
	 *****************************************************************************************************************/
	template <binary_writable T, std::size_t S> struct binary_writer<static_vector<T, S>> {
		static void write_to_stream(std::ostream& os, const static_vector<T, S>& in);
		static std::span<std::byte> write_to_span(std::span<std::byte> span, const static_vector<T, S>& in);
	};
} // namespace tr

#include "static_vector_impl.hpp"