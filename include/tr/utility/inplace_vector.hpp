/// @file
/// @brief Provides `tr::inplace_vector`.

#pragma once
#include <tr/utility/binary_io_specializations.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/pointer_iterator.hpp>
#include <tr/utility/range_concepts.hpp>

//

namespace tr
{
	/// @ingroup containers
	/// @brief Inplace-allocated, fixed-capacity vector.
	/// @details
	/// `tr::inplace_vector` mirrors the interace of `std::vector`, but its elements are stored inplace within the object. This eliminates
	/// any overhead or allocation uncertainty associated with dynamic allocation, but caps the maximum capacity of the container.
	///
	/// The invalidation rules of `tr::inplace_vector` largely mirror those of `std::vector`, but without the total invalidation when
	/// `std::vector` has to reallocate its buffer. The only exception to this is that moving `tr::inplace_vector` is done in linear time,
	/// as it must move every element of the vector.
	/// @tparam Element Type of the elements of the vector.
	/// @tparam Capacity Maximum capacity of the vector.
	template <typename Element, usize Capacity>
		requires std::is_nothrow_destructible_v<Element>
	class inplace_vector
	{
	  public:
		/// @name Container traits
		/// @{

		/// @brief Size type used by the container.
		using size_type = size_type_t<Capacity>;

		/// @brief Difference type used by the container.
		using difference_type = std::ptrdiff_t;

		/// @brief Value type used by the container.
		using value_type = Element;

		/// @brief Reference type used by the container.
		using reference = Element&;

		/// @brief Constant reference type used by the container.
		using const_reference = const Element&;

		/// @brief Pointer type used by the container.
		using pointer = Element*;

		/// @brief Constant pointer type used by the container.
		using const_pointer = const Element*;

		/// @brief Constant iterator type used by the container.
		struct const_iterator : public pointer_iterator<const_iterator, const_pointer>
		{
			using pointer_iterator<const_iterator, const_pointer>::pointer_iterator;
		};

		/// @brief Iterator type used by the container.
		struct iterator : public pointer_iterator<iterator, pointer>
		{
			/// @name Constructors, destructors, and assignment operators
			/// @{

			using pointer_iterator<iterator, pointer>::pointer_iterator;

			/// @}
			/// @name Observers
			/// @{

			/// @brief Converts the iterator into a constant iterator.
			/// @return Equivalent constant iterator.
			[[nodiscard]] operator const_iterator() const noexcept
			{
				return const_iterator{this->base()};
			}

			/// @}
		};

		/// @brief Constant reverse iterator type used by the container.
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		/// @brief Reverse iterator type used by the container.
		using reverse_iterator = std::reverse_iterator<iterator>;

		/// @}
		/// @name Constructors, destructors, and assignment operators
		/// @{

		/// @brief Constructs an empty vector.
		[[nodiscard]] constexpr inplace_vector() noexcept = default;

		/// @brief Constructs a vector with a default-initialized elements.
		/// @param size Initial size of the vector.
		/// @pre `%size` must not exceed the capacity of the vector.
		/// @par Complexity
		/// Linear time with respect to `%size`.
		[[nodiscard]] explicit inplace_vector(size_type size) noexcept(std::is_nothrow_default_constructible_v<Element>)
			requires std::default_initializable<Element>
			: m_size{size}
		{
			TR_ASSERT(size <= Capacity, "Tried to create an inplace vector of size {} but with a max capacity of only {}.", size, Capacity);

			std::ranges::uninitialized_default_construct(*this);
		}

		/// @brief Constructs a vector with copied elements.
		/// @param size Initial size of the vector.
		/// @param value Value to initialize the elements of the vector with.
		/// @pre `%size` must not exceed the capacity of the vector.
		/// @par Complexity
		/// Linear time with respect to `%size`.
		[[nodiscard]] inplace_vector(size_type size, const Element& value) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires std::copy_constructible<Element>
			: m_size{size}
		{
			TR_ASSERT(size <= Capacity, "Tried to create an inplace vector of size {} but with a max capacity of only {}.", size, Capacity);

			std::ranges::uninitialized_fill(*this, value);
		}

		/// @brief Constructs a vector by copying from a range of elements.
		/// @tparam Iterator Forward iterator type to an element convertible to `Element`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param first, last Pair of iterators defining the range of elements to copy from.
		/// @pre The size of `[first, last)` must not exceed the capacity of the vector.
		/// @par Complexity
		/// Linear time with respect to the size of `[first, last)`.
		template <forward_iterator_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
		[[nodiscard]] inplace_vector(Iterator first, Sentinel last) noexcept(nothrow_forward_iterator_to<Iterator, Element>)
			: m_size{static_cast<size_type>(std::ranges::distance(first, last))}
		{
			std::ranges::uninitialized_copy(first, last, begin(), end());
		}

		/// @brief Constructs a vector by copying from a range of elements.
		/// @tparam Range Forward range type to elements convertible to `Element`.
		/// @param range Range of elements to copy from.
		/// @pre The size of `range` must not exceed the capacity of the vector.
		/// @par Complexity
		/// Linear time with respect to the size of `range`.
		template <forward_range_to<Element> Range>
		[[nodiscard]] explicit inplace_vector(Range&& range) noexcept(nothrow_forward_range_to<Range, Element>)
			: inplace_vector{std::ranges::begin(range), std::ranges::end(range)}
		{
		}

		/// @brief Constructs a vector by copying elements from an initializer list.
		/// @param list Initializer list of elements to copy from.
		/// @pre The size of `list` must not exceed the capacity of the vector.
		/// @par Complexity
		/// Linear time with respect to the size of `list`.
		[[nodiscard]] inplace_vector(std::initializer_list<Element> list) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires std::copy_constructible<Element>
			: inplace_vector{list.begin(), list.end()}
		{
		}

		/// @brief Constructs a vector by copying elements from another vector.
		/// @param rhs Vector to copy from.
		/// @par Complexity
		/// Linear time with respect to the size of `rhs`.
		[[nodiscard]] inplace_vector(const inplace_vector& rhs) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires std::copy_constructible<Element>
			: inplace_vector{rhs.begin(), rhs.end()}
		{
		}

		/// @brief Replaces the contents of the vector with a copy of those from another vector.
		/// @param rhs Vector to copy the contents of.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements previously stored in `*this` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `rhs` and at most linear time with respect to the size of `*this`.
		/// @return Reference to `*this`.
		inplace_vector& operator=(const inplace_vector& rhs) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires std::copy_constructible<Element>
		{
			std::ranges::destroy(*this);
			std::ranges::uninitialized_copy(rhs, begin());
			m_size = rhs.m_size;
			return *this;
		}

		/// @brief Constructs a vector by moving elements from another vector.
		/// @param rhs Vector to move from.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements previously stored in `rhs` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `rhs`.
		/// @post `rhs` will be left empty.
		[[nodiscard]] inplace_vector(inplace_vector&& rhs) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires std::move_constructible<Element>
			: inplace_vector{std::move_iterator{rhs.begin()}, std::move_iterator{rhs.end()}}
		{
			rhs.clear();
		}

		/// @brief Replaces the contents of the vector with those moved from another vector.
		/// @param rhs Vector to move the contents of.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements previously stored in `*this` and `rhs` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `rhs` and at most linear time with respect to the size of `*this`.
		/// @return Reference to `*this`.
		inplace_vector& operator=(inplace_vector&& rhs) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires std::move_constructible<Element>
		{
			std::ranges::destroy(*this);
			std::ranges::uninitialized_move(rhs, begin());
			m_size = rhs.m_size;
			rhs.clear();
			return *this;
		}

		/// @brief Destroys the vector and all remaining elements in it.
		~inplace_vector() noexcept
			requires std::is_trivially_destructible_v<Element>
		= default;

		/// @brief Destroys the vector and all remaining elements in it.
		/// @par Complexity
		/// Linear time with respect to the size of the vector.
		~inplace_vector() noexcept
			requires(!std::is_trivially_destructible_v<Element>)
		{
			std::ranges::destroy(*this);
		}

		/// @}
		/// @name Element access
		/// @{

		/// @brief Returns a reference to an element in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to an element within the vector.
		[[nodiscard]] reference at(size_type offset) noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of inplace vector of size {}.", offset, m_size);

			return data()[offset];
		}

		/// @brief Returns a reference to an element in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to an element within the vector.
		[[nodiscard]] const_reference at(size_type offset) const noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of inplace vector of size {}.", offset, m_size);

			return data()[offset];
		}

		/// @brief Returns a reference to an element in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to an element within the vector.
		[[nodiscard]] reference operator[](size_type offset) noexcept
		{
			return at(offset);
		}

		/// @brief Returns a reference to an element in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to an element within the vector.
		[[nodiscard]] const_reference operator[](size_type offset) const noexcept
		{
			return at(offset);
		}

		/// @brief Returns a reference to the first element in the vector.
		/// @return Reference to the first element in the vector.
		[[nodiscard]] reference front() noexcept
		{
			return at(0);
		}

		/// @brief Returns a reference to the first element in the vector.
		/// @return Reference to the first element in the vector.
		[[nodiscard]] const_reference front() const noexcept
		{
			return at(0);
		}

		/// @brief Returns a reference to the last element in the vector.
		/// @return Reference to the last element in the vector.
		[[nodiscard]] reference back() noexcept
		{
			return at(m_size - 1);
		}

		/// @brief Returns a reference to the last element in the vector.
		/// @return Reference to the last element in the vector.
		[[nodiscard]] const_reference back() const noexcept
		{
			return at(m_size - 1);
		}

		/// @brief Returns a pointer to the data of the vector.
		/// @return Pointer to the data of the vector.
		[[nodiscard]] pointer data() noexcept
		{
			return std::launder(reinterpret_cast<pointer>(m_buffer));
		}

		/// @brief Returns a pointer to the data of the vector.
		/// @return Pointer to the data of the vector.
		[[nodiscard]] const_pointer data() const noexcept
		{
			return std::launder(reinterpret_cast<const_pointer>(m_buffer));
		}

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		[[nodiscard]] iterator begin() noexcept
		{
			return iterator{data()};
		}

		/// @brief Returns an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		[[nodiscard]] const_iterator begin() const noexcept
		{
			return const_iterator{data()};
		}

		/// @brief Returns an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		[[nodiscard]] const_iterator cbegin() const noexcept
		{
			return begin();
		}

		/// @brief Returns an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		[[nodiscard]] iterator end() noexcept
		{
			return begin() + m_size;
		}

		/// @brief Returns an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		[[nodiscard]] const_iterator end() const noexcept
		{
			return begin() + m_size;
		}

		/// @brief Returns an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		[[nodiscard]] const_iterator cend() const noexcept
		{
			return begin() + m_size;
		}

		/// @brief Returns an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		[[nodiscard]] reverse_iterator rbegin() noexcept
		{
			return std::reverse_iterator{end()};
		}

		/// @brief Returns an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept
		{
			return std::reverse_iterator{end()};
		}

		/// @brief Returns an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept
		{
			return std::reverse_iterator{cend()};
		}

		/// @brief Returns an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		[[nodiscard]] reverse_iterator rend() noexcept
		{
			return std::reverse_iterator{begin()};
		}

		/// @brief Returns an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		[[nodiscard]] const_reverse_iterator rend() const noexcept
		{
			return std::reverse_iterator{begin()};
		}

		/// @brief Returns an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		[[nodiscard]] const_reverse_iterator crend() const noexcept
		{
			return std::reverse_iterator{cbegin()};
		}

		/// @}
		/// @name Capacity
		/// @{

		/// @brief Checks whether the vector is empty.
		/// @return `true` if the vector is empty, `false` otherwise.
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return m_size == 0;
		}

		/// @brief Returns the number of elements in the vector.
		/// @return Number of elements in the vector.
		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return m_size;
		}

		/// @brief Returns the maximum capacity of the vector.
		/// @return Maximum capacity of the vector.
		[[nodiscard]] static constexpr size_type max_size() noexcept
		{
			return Capacity;
		}

		/// @}
		/// @name Modifiers
		/// @{

		/// @brief Erases all elements from the vector.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored in the vector are invalidated.
		/// @par Complexity
		/// At most linear time with respect to the size of the vector.
		void clear() noexcept
		{
			std::ranges::destroy(*this);
			m_size = 0;
		}

		/// @brief Inserts an element into the vector.
		/// @param where Position within the vector to insert the element.
		/// @param value Element value to copy into the vector.
		/// @pre The vector must not already be at capacity.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `where` are invalidated.
		/// @par Complexity
		/// At most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the element was inserted.
		iterator insert(const_iterator where, const Element& value)
			noexcept(std::is_nothrow_move_constructible_v<Element> && std::is_nothrow_move_assignable_v<Element> &&
					 std::is_nothrow_copy_constructible_v<Element>)
			requires std::move_constructible<Element> && std::is_move_assignable_v<Element> && std::copy_constructible<Element>
		{
			return emplace(where, value);
		}

		/// @brief Inserts an element into the vector.
		/// @param where Position within the vector to insert the element.
		/// @param value Element value to move into the vector.
		/// @pre The vector must not already be at capacity.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `where` are invalidated.
		/// @par Complexity
		/// At most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the element was inserted.
		iterator insert(const_iterator where, Element&& value)
			noexcept(std::is_nothrow_move_constructible_v<Element> && std::is_nothrow_move_assignable_v<Element>)
			requires std::move_constructible<Element> && std::is_move_assignable_v<Element>
		{
			return emplace(where, std::move(value));
		}

		/// @brief Inserts a range of elements into the vector.
		/// @tparam Iterator Forward iterator type to an element convertible to `Element`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param where Position within the vector to insert the range.
		/// @param first, last Pair of iterator defining the range of elements to copy from.
		/// @pre The combined sizes of the vector and `[first, last)` must not exceed the capacity of the vector.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `where` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `[first, last)` and at most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the range was inserted.
		template <forward_iterator_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
			requires std::move_constructible<Element> && std::is_move_assignable_v<Element>
		iterator insert(const_iterator where, Iterator first, Sentinel last)
			noexcept(std::is_nothrow_move_constructible_v<Element> && std::is_nothrow_move_assignable_v<Element> &&
					 nothrow_forward_iterator_to<Iterator, Element>)
		{
			const auto inserted_elements{std::ranges::distance(first, last)};

			TR_ASSERT(static_cast<usize>(m_size + inserted_elements) <= Capacity,
					  "Tried to do an insert into an inplace vector that would put it past its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to inplace_vector::insert.");

			const iterator mwhere{begin() + std::ranges::distance(cbegin(), where)};
			const difference_type affected_elements{std::ranges::distance(mwhere, end())};
			if (affected_elements > 0) {
				if (inserted_elements < affected_elements) {
					std::ranges::uninitialized_move(end() - inserted_elements, end(), end(), end() + inserted_elements);
					std::ranges::move_backward(mwhere, end() - inserted_elements, end());
					std::ranges::destroy_n(mwhere, inserted_elements);
				}
				else {
					std::ranges::uninitialized_move(mwhere, end(), mwhere + inserted_elements,
													mwhere + inserted_elements + affected_elements);
					std::ranges::destroy(mwhere, end());
				}
			}
			std::ranges::uninitialized_copy(first, last, mwhere, mwhere + inserted_elements);
			m_size += static_cast<size_type>(inserted_elements);
			return mwhere;
		}

		/// @brief Copies a range of elements into the vector.
		/// @tparam Range Forward range type to elements convertible to `Element`.
		/// @param where Position within the vector to insert the range.
		/// @param range Range of elements to copy from.
		/// @pre The combined sizes of the vector and `range` must not exceed the capacity of the vector.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `where` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `range` and at most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the range was inserted.
		template <forward_range_to<Element> Range>
			requires std::move_constructible<Element> && std::is_move_assignable_v<Element>
		iterator insert(const_iterator where, Range&& range)
			noexcept(std::is_nothrow_move_constructible_v<Element> && std::is_nothrow_move_assignable_v<Element> &&
					 nothrow_forward_range_to<Range, Element>)
		{
			return insert(where, std::ranges::begin(range), std::ranges::end(range));
		}

		/// @brief Copies elements from an initializer list into the vector.
		/// @param where Position within the vector to insert the list.
		/// @param list Initializer list to insert.
		/// @pre The combined sizes of the vector and `list` must not exceed the capacity of the vector.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `where` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `list` and at most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the list was inserted.
		iterator insert(const_iterator where, std::initializer_list<Element> list)
			noexcept(std::is_nothrow_move_constructible_v<Element> && std::is_nothrow_move_assignable_v<Element> &&
					 std::is_nothrow_copy_constructible_v<Element>)
			requires std::move_constructible<Element> && std::is_move_assignable_v<Element> && std::copy_constructible<Element>
		{
			return insert(where, list.begin(), list.end());
		}

		/// @brief Emplaces an element in the vector.
		/// @tparam Args Types of the arguments to the constructor of an element.
		/// @param where Position within the vector to emplace the element.
		/// @param args Arguments to the constructor of the element.
		/// @pre The vector must not already be at capacity.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `where` are invalidated.
		/// @par Complexity
		/// At most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the element was inserted.
		template <typename... Args>
			requires std::move_constructible<Element> && std::is_move_assignable_v<Element> && std::constructible_from<Element, Args...>
		iterator emplace(const_iterator where, Args&&... args)
			noexcept(std::is_nothrow_move_constructible_v<Element> && std::is_nothrow_move_assignable_v<Element> &&
					 std::is_nothrow_constructible_v<Element, Args...>)
		{
			TR_ASSERT(m_size < Capacity, "Tried to insert into an inplace vector that is already at its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to inplace_vector::insert.");

			const iterator mwhere{begin() + std::ranges::distance(cbegin(), where)};
			if (mwhere != end()) {
				new (end().base()) Element{std::move(back())};
				std::ranges::move_backward(mwhere, end() - 1, end());
				std::ranges::destroy_at(mwhere.base());
			}
			new (mwhere.base()) Element{std::forward<Args>(args)...};
			++m_size;
			return mwhere;
		}

		/// @brief Erases an element from the vector.
		/// @param where Iterator to the element to erase.
		/// @pre `where` must not be equal to `end()`.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `where` are invalidated.
		/// @par Complexity
		/// At most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the character was erased.
		iterator erase(const_iterator where) noexcept(std::is_nothrow_move_assignable_v<Element>)
			requires std::is_move_assignable_v<Element>
		{
			TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to inplace_vector::erase.");

			const iterator mwhere{begin() + (where - begin())};
			std::ranges::move(mwhere + 1, end(), mwhere);
			std::ranges::destroy_at(std::addressof(back()));
			--m_size;
			return mwhere;
		}

		/// @brief Erases a range of elements from the vector.
		/// @param first, last Pair of iterators defining the range of elements to erase.
		/// @par Invalidation
		/// All iterators (including `end()`) and references to elements stored at or after `first` are invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `[first, last)` and at most linear time with respect to the size of the vector.
		/// @return Iterator to the position where the first character was erased.
		iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_move_assignable_v<Element>)
			requires std::is_move_assignable_v<Element>
		{
			TR_ASSERT(first >= begin() && first < end(), "Tried to pass an invalid start iterator to inplace_vector::erase.");
			TR_ASSERT(last >= begin() && last <= end(), "Tried to pass an invalid end iterator to inplace_vector::erase.");

			const iterator mfirst{begin() + (first - begin())};
			const iterator mlast{begin() + (last - begin())};
			const auto erased_elements{std::ranges::distance(mfirst, mlast)};
			std::ranges::move(mlast, end(), mfirst);
			std::ranges::destroy(end() - erased_elements, end());
			m_size -= static_cast<size_type>(erased_elements);
			return mfirst;
		}

		/// @brief Appends an element to the vector.
		/// @param value Element to copy-append to the vector.
		/// @pre The vector must not already be at capacity.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @return Reference to the appended value.
		reference push_back(const Element& value) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires std::copy_constructible<Element>
		{
			return emplace_back(value);
		}

		/// @brief Appends an element to the vector.
		/// @param value Element to move-append to the vector.
		/// @pre The vector must not already be at capacity.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @return Reference to the appended element.
		reference push_back(Element&& value) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires std::move_constructible<Element>
		{
			return emplace_back(std::move(value));
		}

		/// @brief Emplaces an element at the end of the vector.
		/// @tparam Args Types of the arguments to the constructor of an element.
		/// @param args Arguments to the constructor of the element.
		/// @pre The vector must not already be at capacity.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @return Reference to the emplaced element.
		template <typename... Args>
			requires std::constructible_from<Element, Args...>
		reference emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<Element, Args...>)
		{
			return *emplace(end(), std::forward<Args>(args)...);
		}

		/// @brief Appends an iterator pair range to the vector.
		/// @tparam Iterator Forward iterator type to an element convertible to `Element`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param first, last Pair of iterators defining the range of elements to append.
		/// @pre The combined sizes of the vector and `[first, last)` must not exceed the capacity of the vector.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `[first, last)`.
		/// @return Iterator to the position where the first element was appended.
		template <forward_iterator_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
		iterator append(Iterator first, Sentinel last) noexcept(nothrow_forward_iterator_to<Iterator, Element>)
		{
			return insert(end(), first, last);
		}

		/// @brief Appends a range of elements to the vector.
		/// @tparam Range Forward range type to elements convertible to `Element`.
		/// @param range Range of elements to append.
		/// @pre The combined sizes of the vector and `range` must not exceed the capacity of the vector.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `range`.
		/// @return Iterator to the position where the first element was appended.
		template <forward_range_to<Element> Range>
		iterator append(Range&& range) noexcept(nothrow_forward_range_to<Range, Element>)
		{
			return append(std::ranges::begin(range), std::ranges::end(range));
		}

		/// @brief Appends an initializer list to the vector.
		/// @param init Initializer list of elements to append.
		/// @pre The combined sizes of the vector and `list` must not exceed the capacity of the vector.
		/// @par Invalidation
		/// The `end()` iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the size of `list`.
		/// @return Iterator to the position where the first element was appended.
		iterator append(std::initializer_list<Element> init) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires std::copy_constructible<Element>
		{
			return append(init.begin(), init.end());
		}

		/// @brief Erases an element from the end of the vector.
		/// @pre The vector must not be empty.
		/// @par Invalidation
		/// Iterators and references to the previous last element in the vector, as well as the `end()` iterator, are invalidated.
		void pop_back() noexcept
		{
			TR_ASSERT(!empty(), "Tried to pop back from an empty inplace vector.");

			std::ranges::destroy_at(std::addressof(back()));
			--m_size;
		}

		/// @brief Resizes the vector, default-constructing elements if necessary.
		/// @param size New size of the vector.
		/// @pre `%size` must be less than or equal to the capacity of the vector.
		/// @par Invalidation
		/// If `%size` is less than the current size of the vector, all iterators (including `end()`) and references to elements at or after
		/// `begin() + %size` are invalidated. Otherwise, if `%size` is greater than the current size of the vector, only the `end()`
		/// iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the number of newly constructed elements.
		void resize(size_type size) noexcept(std::is_nothrow_default_constructible_v<Element>)
			requires std::default_initializable<Element>
		{
			TR_ASSERT(size <= max_size(), "Tried to resize an inplace vector with capacity {} to size {}.", max_size(), size);

			const iterator old_end{end()};
			m_size = size;
			if (end() < old_end) {
				std::ranges::destroy(end(), old_end);
			}
			else if (end() > old_end) {
				std::ranges::uninitialized_default_construct(old_end, end());
			}
		}

		/// @brief Resizes the vector, copy-constructing elements if necessary.
		/// @param size New size of the vector.
		/// @param value Value to fill new elements of the vector with.
		/// @pre `%size` must be less than or equal to the capacity of the vector.
		/// @par Invalidation
		/// If `%size` is less than the current size of the vector, all iterators (including `end()`) and references to elements at or after
		/// `begin() + %size` are invalidated. Otherwise, if `%size` is greater than the current size of the vector, only the `end()`
		/// iterator is invalidated.
		/// @par Complexity
		/// Linear time with respect to the number of newly constructed elements.
		void resize(size_type size, const Element& value) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires std::copy_constructible<Element>
		{
			TR_ASSERT(size <= max_size(), "Tried to resize an inplace vector with capacity {} to size {}.", max_size(), size);

			const iterator old_end{end()};
			m_size = size;
			if (end() < old_end) {
				std::ranges::destroy(end(), old_end);
			}
			else if (end() > old_end) {
				std::ranges::uninitialized_fill(old_end, end(), value);
			}
		}

		/// @}

	  private:
		/// @brief Buffer where elements are stored.
		alignas(Element) std::byte m_buffer[sizeof(Element) * Capacity]{};

		/// @brief Number of elements in the vector.
		size_type m_size{};
	};
} // namespace tr

//

/// @brief Inplace vector binary reader.
template <tr::binary_constructible Element, tr::usize Capacity>
struct tr::binary_reader<tr::inplace_vector<Element, Capacity>>
{
	/// @brief Reads a variable from a stream.
	/// @param is Input stream.
	/// @param out Output parameter.
	void operator()(std::istream& is, inplace_vector<Element, Capacity>& out) const
	{
		out.resize(read_binary<typename inplace_vector<Element, Capacity>::size_type>(is));
		read_binary(is, std::span{out});
	}
};

/// @brief Inplace vector binary writer.
template <tr::binary_writable Element, tr::usize Capacity>
struct tr::binary_writer<tr::inplace_vector<Element, Capacity>>
{
	/// @brief Writes a variable to a stream.
	/// @param os Output stream.
	/// @param in Variable to write to the stream.
	void operator()(std::ostream& os, const inplace_vector<Element, Capacity>& in) const
	{
		write_binary(os, in.size());
		write_binary(os, std::span{in});
	}
};