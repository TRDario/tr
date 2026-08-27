/// @file
/// @brief Provides `tr::static_vector`.

#pragma once
#include "pointer_iterator.hpp"

//

namespace tr
{
	/// Inplace-allocated, fixed-capacity vector.
	/// @details
	/// `tr::static_vector` mirrors the interace of a standard vector, but its elements are stored inplace within the object.
	///
	/// Instances of `tr::static_vector` are copyable if `Element` is copyable and movable if `Element` is movable.
	/// @tparam Element Type of the elements of the vector.
	/// @tparam Capacity Maximum capacity of the vector.
	template <typename Element, usize Capacity>
	class static_vector
	{
	  public:
		/// Size type used by the container.
		using size_type = size_type_t<Capacity>;

		/// Difference type used by the container.
		using difference_type = std::ptrdiff_t;

		/// Value type used by the container.
		using value_type = Element;

		/// Reference type used by the container.
		using reference = Element&;

		/// Constant reference type used by the container.
		using const_reference = const Element&;

		/// Pointer type used by the container.
		using pointer = Element*;

		/// Constant pointer type used by the container.
		using const_pointer = const Element*;

		/// Constant iterator type used by the container.
		struct const_iterator : public pointer_iterator<const_iterator, const_pointer>
		{
			using pointer_iterator<const_iterator, const_pointer>::pointer_iterator;
		};

		/// Iterator type used by the container.
		struct iterator : public pointer_iterator<iterator, pointer>
		{
			using pointer_iterator<iterator, pointer>::pointer_iterator;

			/// Converts the iterator into a constant iterator.
			/// @return Constant iterator equivalent to `*this`.
			operator const_iterator() const;
		};

		/// Constant reverse iterator type used by the container.
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		/// Reverse iterator type used by the container.
		using reverse_iterator = std::reverse_iterator<iterator>;

		/// @name Constructors
		/// @{

		/// Creates an empty vector.
		constexpr static_vector() = default;

		/// Creates a vector with a certain number of default-initialized values.
		/// @param size Initial size of the vector.
		static_vector(size_type size)
			requires(std::default_initializable<Element>);

		/// Creates a vector with a certain number of copies of a value.
		/// @param size Initial size of the vector.
		/// @param value Value to initialize the values of the vector to.
		static_vector(size_type size, const Element& value)
			requires(std::copy_constructible<Element>);

		/// Creates a vector from an iterator pair.
		/// @tparam Iterator Element input iterator type.
		/// @param first Beginning iterator to the copied range.
		/// @param last Ending iterator to the copied range.
		/// @pre `std::ranges::distance(first, last)` must be less than or equal to `Capacity`.
		template <typed_input_iterator<Element> Iterator>
		static_vector(Iterator first, Iterator last);

		/// Creates a vector from a range.
		/// @tparam Range Element input range type.
		/// @param range Range to copy into the vector.
		/// @pre `std::ranges::size(range)` must be less than or equal to `Capacity`.
		template <typed_input_range<Element> Range>
		static_vector(Range&& range);

		/// Creates a vector from an initializer list.
		/// @param init Initializer list of elements to copy into the vector.
		static_vector(std::initializer_list<Element> init)
			requires(std::copy_constructible<Element>);

		/// Copies a vector.
		/// @param rhs Vector to copy.
		static_vector(const static_vector& rhs)
			requires(std::copy_constructible<Element>);

		/// Moves a vector.
		/// @param rhs Vector to move.
		static_vector(static_vector&& rhs) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires(std::move_constructible<Element>);

		/// Destroys the vector.
		~static_vector();

		/// @}
		/// @name Access
		/// @{

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		reference at(size_type offset);

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		const_reference at(size_type offset) const;

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		reference operator[](size_type offset);

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		const_reference operator[](size_type offset) const;

		/// Gets a reference to the first value in the vector.
		/// @return Reference to the first value in the vector.
		reference front();

		/// Gets a reference to the first value in the vector.
		/// @return Reference to the first value in the vector.
		const_reference front() const;

		/// Gets a reference to the last value in the vector.
		/// @return Reference to the last value in the vector.
		reference back();

		/// Gets a reference to the last value in the vector.
		/// @return Reference to the last value in the vector.
		const_reference back() const;

		/// Gets a pointer to the data of the vector.
		/// @return Pointer to the data of the vector.
		pointer data();

		/// Gets a pointer to the data of the vector.
		/// @return Pointer to the data of the vector.
		const_pointer data() const;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		iterator begin();

		/// Gets an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		const_iterator begin() const;

		/// Gets an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		const_iterator cbegin() const;

		/// Gets an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		iterator end();

		/// Gets an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		const_iterator end() const;

		/// Gets an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		const_iterator cend() const;

		/// Gets an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		reverse_iterator rbegin();

		/// Gets an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		const_reverse_iterator rbegin() const;

		/// Gets an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		const_reverse_iterator crbegin() const;

		/// Gets an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		reverse_iterator rend();

		/// Gets an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		const_reverse_iterator rend() const;

		/// Gets an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		const_reverse_iterator crend() const;

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the vector is empty.
		/// @return `true` if the vector is empty, `false` otherwise.
		constexpr bool empty() const;

		/// Gets the current size of the vector.
		/// @return Size of the vector in elements.
		constexpr size_type size() const;

		/// Gets the maximum capacity of the vector.
		/// @return Maximum capacity of the vector in elements.
		static constexpr size_type max_size();

		/// @}
		/// @name Manipulation
		/// @{

		/// Erases all values from the vector.
		void clear();

		/// Inserts a value into the vector.
		/// @param where Position within the vector to insert the value.
		/// @param value Value to copy into the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the value was inserted.
		iterator insert(const_iterator where, const Element& value)
			requires(std::copy_constructible<Element>);

		/// Inserts a value into the vector.
		/// @param where Position within the vector to insert the value.
		/// @param value Value to move into the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the value was inserted.
		iterator insert(const_iterator where, Element&& value)
			requires(std::move_constructible<Element>);

		/// Inserts an iterator pair range into the string.
		/// @tparam Iterator Element input iterator type.
		/// @param where Position within the vector to insert the range.
		/// @param first Beginning iterator of the range to insert.
		/// @param last Ending iterator of the range to insert.
		/// @pre `this->size() + std::distance(first, last)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the range was inserted.
		template <typed_input_iterator<Element> Iterator>
		iterator insert(const_iterator where, Iterator first, Iterator last);

		/// Inserts a range into the string.
		/// @tparam Range Element input range type.
		/// @param where Position within the vector to insert the range.
		/// @param range Range to insert.
		/// @pre `this->size() + std::ranges::size(range)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the range was inserted.
		template <typed_input_range<Element> Range>
		iterator insert(const_iterator where, Range&& range);

		/// Inserts an initializer list into the string.
		/// @param where Position within the vector to insert the list.
		/// @param init Initializer list to insert.
		/// @pre `this->size() + init.size()` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the list was inserted.
		iterator insert(const_iterator where, std::initializer_list<Element> init)
			requires(std::copy_constructible<Element>);

		/// Emplaces a value in the vector.
		/// @tparam Args Types of the arguments to the constructor of a value.
		/// @param where Position within the vector to emplace the value.
		/// @param args Arguments to the constructor of the value.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the value was inserted.
		template <typename... Args>
			requires(std::constructible_from<Element, Args...>)
		iterator emplace(const_iterator where, Args&&... args);

		/// Erases a value from the vector.
		/// @param where Iterator to the value to erase.
		/// @return Iterator to the position where the character was erased.
		iterator erase(const_iterator where);

		/// Erases a range of values from the vector.
		/// @param first Iterator to the first value to erase.
		/// @param last Iterator to one past the last value to erase.
		/// @return Iterator to the position where the first character was erased.
		iterator erase(const_iterator first, const_iterator last);

		/// Appends a value to the vector.
		/// @param value Value to copy-append to the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to the appended value.
		reference push_back(const Element& value)
			requires(std::copy_constructible<Element>);

		/// Appends a value to the vector.
		/// @param value Value to move-append to the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to the appended value.
		reference push_back(Element&& value)
			requires(std::move_constructible<Element>);

		/// Emplaces a value at the end of the vector.
		/// @tparam Args Types of the arguments to the constructor of a value.
		/// @param args Arguments to the constructor of the value.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to the emplaced value.
		template <typename... Args>
			requires(std::constructible_from<Element, Args...>)
		reference emplace_back(Args&&... args);

		/// Appends an iterator pair range to the vector.
		/// @tparam Iterator Element input iterator type.
		/// @param first Beginning iterator of the range to append.
		/// @param last Ending iterator of the range to append.
		/// @pre `this->size() + std::distance(first, last)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the first element was appended.
		template <typed_input_iterator<Element> Iterator>
		iterator append(Iterator first, Iterator last);

		/// Appends a range to the vector.
		/// @tparam Range Element input range type.
		/// @param range Range to append.
		/// @pre `this->size() + std::ranges::size(range)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the first element was appended.
		template <typed_input_range<Element> Range>
		iterator append(Range&& range);

		/// Appends an initializer list to the vector.
		/// @param init Initializer list to append.
		/// @pre `this->size() + init.size()` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the first element was appended.
		iterator append(std::initializer_list<Element> init)
			requires(std::copy_constructible<Element>);

		/// Erases a value from the end of the vector.
		void pop_back();

		/// Resizes the vector, default-constructing values if necessary.
		/// @param size New size of the vector.
		/// @pre `size` must be less than or equal to `Capacity`.
		void resize(size_type size)
			requires(std::default_initializable<Element>);

		/// Resizes the vector, copy-constructing values if necessary.
		/// @param size New size of the vector.
		/// @param value Value to fill the vector with.
		/// @pre `size` must be less than or equal to `Capacity`.
		void resize(size_type size, const Element& value)
			requires(std::copy_constructible<Element>);

		/// @}

	  private:
		/// Buffer where elements are stored.
		alignas(Element) std::byte m_buffer[sizeof(Element) * Capacity]{};

		/// Number of elements in the vector.
		size_type m_size{};
	};
} // namespace tr

#include "impl/static_vector.hpp" // IWYU pragma: export