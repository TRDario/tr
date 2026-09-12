/// @file
/// @brief Provides `tr::static_vector`.

#pragma once
#include <tr/utility/binary_io_specializations.hpp>
#include <tr/utility/macro.hpp>
#include <tr/utility/pointer_iterator.hpp>

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
	template <nothrow_destructible Element, usize Capacity>
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
			[[nodiscard]] operator const_iterator() const noexcept
			{
				return const_iterator{this->base()};
			}
		};

		/// Constant reverse iterator type used by the container.
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		/// Reverse iterator type used by the container.
		using reverse_iterator = std::reverse_iterator<iterator>;

		/// @name Constructors
		/// @{

		/// Creates an empty vector.
		[[nodiscard]] constexpr static_vector() noexcept = default;

		/// Creates a vector with a certain number of default-initialized values.
		/// @param size Initial size of the vector.
		[[nodiscard]] explicit static_vector(size_type size) noexcept(std::is_nothrow_default_constructible_v<Element>)
			requires(std::default_initializable<Element>)
			: m_size{size}
		{
			TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

			std::uninitialized_default_construct(begin(), end());
		}

		/// Creates a vector with a certain number of copies of a value.
		/// @param size Initial size of the vector.
		/// @param value Value to initialize the values of the vector to.
		[[nodiscard]] static_vector(size_type size, const Element& value) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires(std::copy_constructible<Element>)
			: m_size{size}
		{
			TR_ASSERT(size <= Capacity, "Tried to create a static vector of size {} but with a max capacity of only {}.", size, Capacity);

			std::uninitialized_fill(begin(), end(), value);
		}

		/// Creates a vector from an iterator pair.
		/// @tparam Iterator Forward iterator type to an element convertible to `Element`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param first Beginning iterator to the copied range.
		/// @param last Ending iterator to the copied range.
		/// @pre `std::ranges::distance(first, last)` must be less than or equal to `Capacity`.
		template <forward_iterator_to_convertible_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
		[[nodiscard]] static_vector(Iterator first, Sentinel last) noexcept(nothrow_forward_iterator_to_convertible_to<Iterator, Element>)
			: m_size{static_cast<size_type>(std::ranges::distance(first, last))}
		{
			std::uninitialized_copy(first, last, begin());
		}

		/// Creates a vector from a range.
		/// @tparam Range Forward range type to elements converible to `Element`.
		/// @param range Range to copy into the vector.
		/// @pre `std::ranges::size(range)` must be less than or equal to `Capacity`.
		template <forward_range_to_convertible_to<Element> Range>
		[[nodiscard]] explicit static_vector(Range&& range) noexcept(nothrow_forward_range_to_convertible_to<Range, Element>)
			: static_vector{std::ranges::begin(range), std::ranges::end(range)}
		{
		}

		/// Creates a vector from an initializer list.
		/// @param init Initializer list of elements to copy into the vector.
		[[nodiscard]] static_vector(std::initializer_list<Element> init) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires(std::copy_constructible<Element>)
			: static_vector{init.begin(), init.end()}
		{
		}

		/// Copies a vector.
		/// @param rhs Vector to copy.
		[[nodiscard]] static_vector(const static_vector& rhs) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires(std::copy_constructible<Element>)
			: static_vector{rhs.begin(), rhs.end()}
		{
		}

		/// Moves a vector.
		/// @param rhs Vector to move. The vector will be in a cleared state after this operation.
		[[nodiscard]] static_vector(static_vector&& rhs) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires(std::move_constructible<Element>)
			: static_vector{std::move_iterator{rhs.begin()}, std::move_iterator{rhs.end()}}
		{
			rhs.clear();
		}

		/// Destroys the vector.
		~static_vector() noexcept
		{
			std::destroy(begin(), end());
		}

		/// @}
		/// @name Assignment operators
		/// @{

		/// Copy-assigns a static vector.
		/// @param rhs Vector to copy.
		/// @return Reference to `*this`.
		static_vector& operator=(const static_vector& rhs) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires(std::copy_constructible<Element>)
		{
			std::destroy(begin(), end());
			std::uninitialized_copy(rhs.begin(), rhs.end(), begin());
			m_size = rhs.m_size;
			return *this;
		}

		/// Move-assigns a static vector.
		/// @param rhs Vector to move. The vector will be in a cleared state after this operation.
		/// @return Reference to `*this`.
		static_vector& operator=(static_vector&& rhs) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires(std::move_constructible<Element>)
		{
			std::destroy(begin(), end());
			std::uninitialized_move(rhs.begin(), rhs.end(), begin());
			m_size = rhs.m_size;
			rhs.clear();
			return *this;
		}

		/// @}
		/// @name Access
		/// @{

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		[[nodiscard]] reference at(size_type offset) noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);

			return data()[offset];
		}

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		[[nodiscard]] const_reference at(size_type offset) const noexcept
		{
			TR_ASSERT(offset < m_size, "Tried to do an out-of-bounds read at position {} of static vector with size {}.", offset, m_size);

			return data()[offset];
		}

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		[[nodiscard]] reference operator[](size_type offset) noexcept
		{
			return at(offset);
		}

		/// Gets a reference to a value in the vector.
		/// @param offset Index of the character within the vector.
		/// @pre `offset` must be less than the size of the vector.
		/// @return Reference to a value within the vector.
		[[nodiscard]] const_reference operator[](size_type offset) const noexcept
		{
			return at(offset);
		}

		/// Gets a reference to the first value in the vector.
		/// @return Reference to the first value in the vector.
		[[nodiscard]] reference front() noexcept
		{
			return at(0);
		}

		/// Gets a reference to the first value in the vector.
		/// @return Reference to the first value in the vector.
		[[nodiscard]] const_reference front() const noexcept
		{
			return at(0);
		}

		/// Gets a reference to the last value in the vector.
		/// @return Reference to the last value in the vector.
		[[nodiscard]] reference back() noexcept
		{
			return at(m_size - 1);
		}

		/// Gets a reference to the last value in the vector.
		/// @return Reference to the last value in the vector.
		[[nodiscard]] const_reference back() const noexcept
		{
			return at(m_size - 1);
		}

		/// Gets a pointer to the data of the vector.
		/// @return Pointer to the data of the vector.
		[[nodiscard]] pointer data() noexcept
		{
			return reinterpret_cast<pointer>(m_buffer);
		}

		/// Gets a pointer to the data of the vector.
		/// @return Pointer to the data of the vector.
		[[nodiscard]] const_pointer data() const noexcept
		{
			return reinterpret_cast<const_pointer>(m_buffer);
		}

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		[[nodiscard]] iterator begin() noexcept
		{
			return iterator{data()};
		}

		/// Gets an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		[[nodiscard]] const_iterator begin() const noexcept
		{
			return const_iterator{data()};
		}

		/// Gets an iterator to the beginning of the vector.
		/// @return Iterator to the beginning of the vector.
		[[nodiscard]] const_iterator cbegin() const noexcept
		{
			return begin();
		}

		/// Gets an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		[[nodiscard]] iterator end() noexcept
		{
			return begin() + m_size;
		}

		/// Gets an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		[[nodiscard]] const_iterator end() const noexcept
		{
			return begin() + m_size;
		}

		/// Gets an iterator to the end of the vector.
		/// @return Iterator to the end of the vector.
		[[nodiscard]] const_iterator cend() const noexcept
		{
			return begin() + m_size;
		}

		/// Gets an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		[[nodiscard]] reverse_iterator rbegin() noexcept
		{
			return std::reverse_iterator{end()};
		}

		/// Gets an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		[[nodiscard]] const_reverse_iterator rbegin() const noexcept
		{
			return std::reverse_iterator{end()};
		}

		/// Gets an iterator to the reverse beginning of the vector.
		/// @return Iterator to the reverse beginning of the vector.
		[[nodiscard]] const_reverse_iterator crbegin() const noexcept
		{
			return std::reverse_iterator{cend()};
		}

		/// Gets an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		[[nodiscard]] reverse_iterator rend() noexcept
		{
			return std::reverse_iterator{begin()};
		}

		/// Gets an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		[[nodiscard]] const_reverse_iterator rend() const noexcept
		{
			return std::reverse_iterator{begin()};
		}

		/// Gets an iterator to the reverse end of the vector.
		/// @return Iterator to the reverse end of the vector.
		[[nodiscard]] const_reverse_iterator crend() const noexcept
		{
			return std::reverse_iterator{cbegin()};
		}

		/// @}
		/// @name Size
		/// @{

		/// Gets whether the vector is empty.
		/// @return `true` if the vector is empty, `false` otherwise.
		[[nodiscard]] constexpr bool empty() const noexcept
		{
			return m_size == 0;
		}

		/// Gets the current size of the vector.
		/// @return Size of the vector in elements.
		[[nodiscard]] constexpr size_type size() const noexcept
		{
			return m_size;
		}

		/// Gets the maximum capacity of the vector.
		/// @return Maximum capacity of the vector in elements.
		[[nodiscard]] static constexpr size_type max_size() noexcept
		{
			return Capacity;
		}

		/// @}
		/// @name Manipulation
		/// @{

		/// Erases all values from the vector.
		void clear() noexcept
		{
			std::destroy(begin(), end());
			m_size = 0;
		}

		/// Inserts a value into the vector.
		/// @param where Position within the vector to insert the value.
		/// @param value Value to copy into the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the value was inserted.
		iterator insert(const_iterator where, const Element& value)
			noexcept(nothrow_movable<Element> && std::is_nothrow_copy_constructible_v<Element>)
			requires(std::move_constructible<Element> && move_assignable<Element> && std::copy_constructible<Element>)
		{
			return emplace(where, value);
		}

		/// Inserts a value into the vector.
		/// @param where Position within the vector to insert the value.
		/// @param value Value to move into the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the value was inserted.
		iterator insert(const_iterator where, Element&& value) noexcept(nothrow_movable<Element>)
			requires(std::move_constructible<Element> && move_assignable<Element>)
		{
			return emplace(where, std::move(value));
		}

		/// Inserts an iterator pair range into the string.
		/// @tparam Iterator Forward iterator type to an element convertible to `Element`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param where Position within the vector to insert the range.
		/// @param first Beginning iterator of the range to insert.
		/// @param last Ending iterator of the range to insert.
		/// @pre `this->size() + std::distance(first, last)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the range was inserted.
		template <forward_iterator_to_convertible_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
			requires(std::move_constructible<Element> && move_assignable<Element>)
		iterator insert(const_iterator where, Iterator first, Sentinel last)
			noexcept(nothrow_movable<Element> && nothrow_forward_iterator_to_convertible_to<Iterator, Element>)
		{
			const auto inserted_elements{std::ranges::distance(first, last)};

			TR_ASSERT(static_cast<usize>(m_size + inserted_elements) <= Capacity,
					  "Tried to do an insert into a static vector that would put it past its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

			const iterator mwhere{begin() + std::ranges::distance(cbegin(), where)};
			const difference_type affected_elements{std::ranges::distance(mwhere, end())};
			if (affected_elements > 0) {
				if (inserted_elements < affected_elements) {
					std::uninitialized_move(end() - inserted_elements, end(), end());
					std::move_backward(mwhere, end() - inserted_elements, end());
					std::destroy_n(mwhere, inserted_elements);
				}
				else {
					std::uninitialized_move(mwhere, end(), mwhere + inserted_elements);
					std::destroy(mwhere, end());
				}
			}
			std::uninitialized_copy(first, last, mwhere);
			m_size += inserted_elements;
			return mwhere;
		}

		/// Inserts a range into the string.
		/// @tparam Range Forward range type to elements converible to `Element`.
		/// @param where Position within the vector to insert the range.
		/// @param range Range to insert.
		/// @pre `this->size() + std::ranges::size(range)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the range was inserted.
		template <forward_range_to_convertible_to<Element> Range>
			requires(std::move_constructible<Element> && move_assignable<Element>)
		iterator insert(const_iterator where, Range&& range)
			noexcept(nothrow_movable<Element> && nothrow_forward_range_to_convertible_to<Range, Element>)
		{
			return insert(where, std::ranges::begin(range), std::ranges::end(range));
		}

		/// Inserts an initializer list into the string.
		/// @param where Position within the vector to insert the list.
		/// @param init Initializer list to insert.
		/// @pre `this->size() + init.size()` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the list was inserted.
		iterator insert(const_iterator where, std::initializer_list<Element> init)
			noexcept(nothrow_movable<Element> && std::is_nothrow_copy_constructible_v<Element>)
			requires(std::move_constructible<Element> && move_assignable<Element> && std::copy_constructible<Element>)
		{
			return insert(where, init.begin(), init.end());
		}

		/// Emplaces a value in the vector.
		/// @tparam Args Types of the arguments to the constructor of a value.
		/// @param where Position within the vector to emplace the value.
		/// @param args Arguments to the constructor of the value.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Iterator to the position where the value was inserted.
		template <typename... Args>
			requires(std::move_constructible<Element> && move_assignable<Element> && std::constructible_from<Element, Args...>)
		iterator emplace(const_iterator where, Args&&... args)
			noexcept(nothrow_movable<Element> && std::is_nothrow_constructible_v<Element, Args...>)
		{
			TR_ASSERT(m_size < Capacity, "Tried to insert into a static vector that is already at its capacity of {}.", Capacity);
			TR_ASSERT(where >= begin() && where <= end(), "Tried to pass an invalid iterator to static_vector::insert.");

			const iterator mwhere{begin() + std::distance(cbegin(), where)};
			if (mwhere != end()) {
				new (end().base()) Element{std::move(back())};
				std::move_backward(mwhere, end() - 1, end());
				mwhere->~Element();
			}
			new (mwhere.base()) Element{std::forward<Args>(args)...};
			++m_size;
			return mwhere;
		}

		/// Erases a value from the vector.
		/// @param where Iterator to the value to erase.
		/// @return Iterator to the position where the character was erased.
		iterator erase(const_iterator where) noexcept(std::is_nothrow_move_assignable_v<Element>)
			requires(move_assignable<Element>)
		{
			TR_ASSERT(where >= begin() && where < end(), "Tried to pass an invalid iterator to static_vector::erase.");

			const iterator mwhere{begin() + (where - begin())};
			std::move(mwhere + 1, end(), mwhere);
			back().~Element();
			--m_size;
			return mwhere;
		}

		/// Erases a range of values from the vector.
		/// @param first Iterator to the first value to erase.
		/// @param last Iterator to one past the last value to erase.
		/// @return Iterator to the position where the first character was erased.
		iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_move_assignable_v<Element>)
			requires(move_assignable<Element>)
		{
			TR_ASSERT(first >= begin() && first < end(), "Tried to pass an invalid start iterator to static_vector::erase.");
			TR_ASSERT(last >= begin() && last <= end(), "Tried to pass an invalid end iterator to static_vector::erase.");

			const iterator mfirst{begin() + (first - begin())};
			const iterator mlast{begin() + (last - begin())};
			const difference_type erased_elements{std::distance(mfirst, mlast)};
			std::move(mlast, end(), mfirst);
			std::destroy(end() - erased_elements, end());
			m_size -= erased_elements;
			return mfirst;
		}

		/// Appends a value to the vector.
		/// @param value Value to copy-append to the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to the appended value.
		reference push_back(const Element& value) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires(std::copy_constructible<Element>)
		{
			return emplace_back(value);
		}

		/// Appends a value to the vector.
		/// @param value Value to move-append to the vector.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to the appended value.
		reference push_back(Element&& value) noexcept(std::is_nothrow_move_constructible_v<Element>)
			requires(std::move_constructible<Element>)
		{
			return emplace_back(std::move(value));
		}

		/// Emplaces a value at the end of the vector.
		/// @tparam Args Types of the arguments to the constructor of a value.
		/// @param args Arguments to the constructor of the value.
		/// @pre The size of the string must be less than `Capacity`.
		/// @return Reference to the emplaced value.
		template <typename... Args>
			requires(std::constructible_from<Element, Args...>)
		reference emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<Element, Args...>)
		{
			return *emplace(end(), std::forward<Args>(args)...);
		}

		/// Appends an iterator pair range to the vector.
		/// @tparam Iterator Forward iterator type to an element convertible to `Element`.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param first Beginning iterator of the range to append.
		/// @param last Ending iterator of the range to append.
		/// @pre `this->size() + std::distance(first, last)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the first element was appended.
		template <forward_iterator_to_convertible_to<Element> Iterator, std::sentinel_for<Iterator> Sentinel>
		iterator append(Iterator first, Sentinel last) noexcept(nothrow_forward_iterator_to_convertible_to<Iterator, Element>)
		{
			return insert(end(), first, last);
		}

		/// Appends a range to the vector.
		/// @tparam Range Forward range type to elements converible to `Element`.
		/// @param range Range to append.
		/// @pre `this->size() + std::ranges::size(range)` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the first element was appended.
		template <forward_range_to_convertible_to<Element> Range>
		iterator append(Range&& range) noexcept(nothrow_forward_range_to_convertible_to<Range, Element>)
		{
			return append(std::ranges::begin(range), std::ranges::end(range));
		}

		/// Appends an initializer list to the vector.
		/// @param init Initializer list to append.
		/// @pre `this->size() + init.size()` must be less than or equal to `Capacity`.
		/// @return Iterator to the position where the first element was appended.
		iterator append(std::initializer_list<Element> init) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires(std::copy_constructible<Element>)
		{
			return append(init.begin(), init.end());
		}

		/// Erases a value from the end of the vector.
		void pop_back() noexcept
		{
			back().~Element();
			--m_size;
		}

		/// Resizes the vector, default-constructing values if necessary.
		/// @param size New size of the vector.
		/// @pre `size` must be less than or equal to `Capacity`.
		void resize(size_type size) noexcept(std::is_nothrow_default_constructible_v<Element>)
			requires(std::default_initializable<Element>)
		{
			TR_ASSERT(size <= max_size(), "Tried to resize a static vector with capacity {} to size {}.", max_size(), size);

			const iterator old_end{end()};
			m_size = size;
			if (end() < old_end) {
				std::destroy(end(), old_end);
			}
			else if (end() > old_end) {
				std::uninitialized_default_construct(old_end, end());
			}
		}

		/// Resizes the vector, copy-constructing values if necessary.
		/// @param size New size of the vector.
		/// @param value Value to fill the vector with.
		/// @pre `size` must be less than or equal to `Capacity`.
		void resize(size_type size, const Element& value) noexcept(std::is_nothrow_copy_constructible_v<Element>)
			requires(std::copy_constructible<Element>)
		{
			TR_ASSERT(size <= max_size(), "Tried to resize a static vector with capacity {} to size {}.", max_size(), size);

			const iterator old_end{end()};
			m_size = size;
			if (end() < old_end) {
				std::destroy(end(), old_end);
			}
			else if (end() > old_end) {
				std::uninitialized_fill(old_end, end(), value);
			}
		}

		/// @}

	  private:
		/// Buffer where elements are stored.
		alignas(Element) std::byte m_buffer[sizeof(Element) * Capacity]{};

		/// Number of elements in the vector.
		size_type m_size{};
	};
} // namespace tr

//

/// Static vector binary reader.
/// @tparam Element Type of the elements of the vector.
/// @tparam Capacity Maximum capacity of the vector.
template <tr::binary_constructible Element, tr::usize Capacity>
struct tr::binary_reader<tr::static_vector<Element, Capacity>>
{
	/// Reads a variable from a stream.
	/// @param is Input stream.
	/// @param out Output parameter.
	void operator()(std::istream& is, static_vector<Element, Capacity>& out) const
	{
		out.resize(read_binary<typename static_vector<Element, Capacity>::size_type>(is));
		read_binary(is, std::span{out});
	}
};

/// Static vector binary writer.
/// @tparam Element Type of the elements of the vector.
/// @tparam Capacity Maximum capacity of the vector.
template <tr::binary_writable Element, tr::usize Capacity>
struct tr::binary_writer<tr::static_vector<Element, Capacity>>
{
	/// Writes a variable to a stream.
	/// @param os Output stream.
	/// @param in Variable to write to the stream.
	void operator()(std::ostream& os, const static_vector<Element, Capacity>& in) const
	{
		write_binary(os, in.size());
		write_binary(os, std::span{in});
	}
};