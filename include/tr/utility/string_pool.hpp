/// @file
/// @brief Provides `tr::string_pool`.

#pragma once
#include <tr/utility/iterator_interface.hpp>
#include <tr/utility/opt_ref.hpp>
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// @brief Container storing a series of immutable NUL-terminated strings in a single allocation.
	/// @details
	/// `tr::string_pool` is designed for efficient storage and lifetime management of a series of immutable strings. Rather than allocating
	/// individual buffers for each string, all strings are packed sequentially inside a single, contiguous buffer. This eliminates
	/// allocation overhead and heap fragmentation for scenarios where several strings share a common lifetime and do not require
	/// mutability. The underlying buffer is dynamically resizable and may be reallocated when its capacity is exceeded.
	///
	/// The invalidation rules of `tr::string_pool` largely mirror those of `std::vector`:
	/// Any operation that triggers in a reallocation invalidates all iterators and views to strings in the pool.
	/// Appending without reallocation invalidates the end iterator.
	/// Erasing invalidates all iterators and views to strings starting from the first erased string to the end of the pool.
	///
	/// `tr::string_pool` on its own does not do any sort of caching or indexing of the strings it stores. This means that iterating through
	/// the string pool is linear with respect not to the number of strings, but rather the total number of bytes taken up by said strings.
	/// This also means that `tr::string_pool` does not provide a constant-time `size()` function returning the number of stored strings.
	class string_pool
	{
	  public:
		/// @brief Iterator to a string in a string pool.
		/// @details `tr::string_pool::iterator` models `std::bidirectional_iterator`. Due to the unindexed nature of the underlying
		/// container, incrementing and decrementing the iterator is actually done in linear time with respect to the iterated strings.
		class iterator : public iterator_interface<iterator>
		{
		  public:
			/// @brief Value type used by the iterator.
			using value_type = zstring_view;

			/// @brief Difference type used by the iterator.
			using difference_type = std::ptrdiff_t;

			/// @brief Iterator category of the iterator.
			using iterator_category = std::bidirectional_iterator_tag;

			/// @brief Iterator concept of the iterator.
			using iterator_concept = std::bidirectional_iterator_tag;

			/// @cond Implementation details
			/// @name Constructors and destructors
			/// @{

			/// @brief Constructs a singular iterator.
			/// @details This is necessary to satisfy `std::semiregular`.
			[[nodiscard]] iterator() noexcept = default;

			/// @brief Constructs an iterator.
			/// @param begin Base iterator to the beginning of the string pool.
			/// @param base Base iterator to the beginning of the pointed-to string.
			[[nodiscard]] iterator(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator base) noexcept;

			/// @}
			/// @endcond
			/// @name Comparison operators
			/// @{

			/// @brief String pool iterators are three-way comparable.
			[[nodiscard]] friend std::strong_ordering operator<=>(iterator, iterator) noexcept = default;

			/// @brief String pool iterators are equality-comparable.
			[[nodiscard]] friend bool operator==(iterator, iterator) noexcept = default;

			/// @}
			/// @name Other operators
			/// @{

			/// @brief Increments the iterator.
			/// @par Complexity
			/// Linear time with respect to the length of the pointed-to string.
			/// @return Reference to `*this`.
			iterator& operator++() noexcept;

			/// @brief Decrements the iterator.
			/// @par Complexity
			/// Linear time with respect to the length of the previous string.
			/// @return Reference to `*this`.
			iterator& operator--() noexcept;

			/// @brief Dereferences the iterator.
			/// @return View to the pointed-to string.
			[[nodiscard]] value_type operator*() const noexcept;

			/// @}
			/// @cond implementation_details
			/// @name Implementation details
			/// @{

			/// @brief Returns the base iterator to the beginning of the pointed-to string.
			/// @return Base iterator to the beginning of the pointed-to string.
			[[nodiscard]] std::vector<char>::const_iterator base() const noexcept;

			/// @}
			/// @endcond

		  private:
			/// @brief Base iterator to the beginning of the string pool.
			std::vector<char>::const_iterator m_begin;

			/// @brief Base iterator to the beginning of the pointed-to string.
			std::vector<char>::const_iterator m_base;
		};

		/// @brief String builder used for inplace string building into a string pool.
		/// @details
		/// To avoid a situation in which a temporary string has to be built just to be appended to a string pool, `string_builder()` is
		/// provided as a method of directly emplacing a string into a pool.
		///
		/// The emplaced string is kept as a shadow element at the end of the pool until the builder goes out of scope, at which point the
		/// string is finalized and starts beign treated as a full element of the pool.
		///
		/// Several string pool operations are incompatible with the string builder. Weaving a string builder with regular string pool
		/// operations is thus generally not recommended.
		class string_builder_t
		{
		  public:
			/// @name Constructors and destructors
			/// @{

			/// @brief Constructs a string builder.
			/// @param pool Pool delegating string building to this string builder.
			[[nodiscard]] string_builder_t(string_pool& pool) noexcept;

			/// @brief String builders are not copyable.
			string_builder_t(const string_builder_t&) = delete;

			/// @brief Takes over the task from another string builder.
			/// @param rhs String builder to transfer the task from.
			/// @post `rhs` is left in a special state where it is unassociated with any pool, and thus `out()` cannot be called.
			[[nodiscard]] string_builder_t(string_builder_t&& rhs) noexcept;

			/// @brief Finalizes building the string.
			~string_builder_t() noexcept;

			/// @}
			/// @name Assignment operators
			/// @{

			/// @brief String builders are not copyable.
			string_builder_t& operator=(const string_builder_t&) = delete;

			/// @brief Finalizes building the current string and takes over the task from another string builder.
			/// @param rhs String builder to transfer the task from.
			/// @post `rhs` is left in a special state where it is unassociated with any pool, and thus `out()` cannot be called.
			/// @return Reference to `*this`.
			string_builder_t& operator=(string_builder_t&& rhs) noexcept;

			/// @}
			/// @name Iterator
			/// @{

			/// @brief Returns a character output iterator.
			/// @return Character output iterator.
			[[nodiscard]] std::back_insert_iterator<std::vector<char>> out() const noexcept;

			/// @}

		  private:
			/// @brief Reference to the pool delegating string building.
			/// @details May be empty after being moved from.
			opt_ref<string_pool> m_pool;
		};

		/// @name Constructors and destructors
		/// @{

		/// @brief Constructs an empty string pool.
		[[nodiscard]] string_pool() noexcept = default;

		/// @brief Constructs a string pool by copying the contents of another pool.
		/// @param rhs Pool to copy the contents of.
		/// @par Complexity
		/// Linear time with respect to `rhs.size_bytes()`.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] string_pool(const string_pool& rhs) = default;

		/// @brief Constructs a string pool by moving the contents of another pool.
		/// @param rhs Pool to move the contents of.
		/// @par Invalidation
		/// All iterators (including `end()`) and views to strings previously stored in `rhs` stay valid, but now point to the equivalent
		/// strings stored in `*this`.
		/// @post `rhs` is left in a valid, but unspecified state after the move.
		[[nodiscard]] string_pool(string_pool&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// @brief Replaces the contents of the pool with a copy of those of another pool.
		/// @param rhs Pool to copy the contents of.
		/// @pre There may not be an active string builder associated with this pool at the time this function is called.
		/// @par Invalidation
		/// All iterators (including `end()`) and views to strings previously stored in `*this` are invalidated.
		/// @par Complexity
		/// Linear time with respect to `rhs.size_bytes()`.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return Reference to `*this`.
		string_pool& operator=(const string_pool& rhs);

		/// @brief Replaces the contents of the pool with those moved from another pool.
		/// @param rhs Pool to move the contents of.
		/// @pre There may not be an active string builder associated with `*this` or `rhs` at the time this function is called.
		/// @par Invalidation
		/// All iterators (including `end()`) and views to strings previously stored in the pool are invalidated.
		///
		/// All iterators (including `end()`) and views to strings previously stored in `rhs` stay valid, but now point to the equivalent
		/// strings stored in `*this`.
		/// @post `rhs` is left in a valid, but unspecified state after the move.
		/// @return Reference to `*this`.
		string_pool& operator=(string_pool&& rhs) noexcept = default;

		/// @}
		/// @name Iterators
		/// @{

		/// @brief Returns an iterator to the first string in the pool.
		/// @details If the pool is empty, the returned iterator will be equal to `end()`.
		/// @return Iterator to the first string in the pool.
		[[nodiscard]] iterator begin() const noexcept;

		/// @brief Returns an iterator to the position after the last string in the pool.
		/// @details The `end()` iterator cannot be dereferenced.
		/// @return Iterator to the position after the last string in the pool.
		[[nodiscard]] iterator end() const noexcept;

		/// @}
		/// @name Size and capacity
		/// @{

		/// @brief Checks whether the string pool is empty.
		/// @return `true` if the string pool is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// @brief Returns the amount of space taken up by strings in the pool.
		/// @return Amount of space (in bytes) taken up by strings in the pool.
		[[nodiscard]] usize size_bytes() const noexcept;

		/// @brief Returns the amount of space that may be taken up by strings in the pool without requiring reallocation.
		/// @return Amount of space (in bytes) that may be taken up by strings in the pool without requiring reallocation.
		[[nodiscard]] usize capacity_bytes() const noexcept;

		/// @brief Increases the capacity of the pool.
		/// @details
		/// The capacity of a string pool is the amount of space that may be taken up by strings in the pool without requiring reallocation.
		///
		/// The capacity of a string pool may never shrink, so reserving a capacity smaller than `capacity_bytes()` does nothing.
		/// @param capacity Desired capacity (in bytes) of the pool.
		/// @par Invalidation
		/// If `capacity` exceeds `capacity_bytes()`, all iterators (including `end()`) and views to strings stored in the pool are
		/// invalidated. Otherwise, no iterators or references are invalidated.
		/// @par Complexity
		/// At most linear with respect to `capacity`.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void reserve_bytes(usize capacity);

		/// @}
		/// @name Manipulation
		/// @{

		/// @brief Erases all strings from the pool.
		/// @details The `capacity_bytes()` of the pool stays unchanged.
		/// @pre There may not be an active string builder associated with this pool at the time this function is called.
		/// @par Invalidation
		/// All iterators and views to strings stored in the pool are invalidated.
		void clear() noexcept;

		/// @brief Appends a string to the end of the pool.
		/// @param string String to append.
		/// @pre `string` may not contain any embedded NUL characters.
		/// @pre There may not be an active string builder associated with this pool at the time this function is called.
		/// @par Invalidation
		/// If `size_bytes() + string.size() + 1` exceeds `capacity()`, all iterators (including `end()`) and views to strings stored in the
		/// pool are invalidated. Otherwise, only the `end()` iterator is invalidated.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		/// @return Iterator to the appended string.
		iterator append(std::string_view string);

		/// @brief Constructs a string builder object that allows inplace string emplacement into the pool.
		/// @pre There may not already be an active string builder associated with this pool at the time this function is called.
		/// @par Invalidation
		/// If `capacity_bytes()` is exceeded at any point during the emplacement, all iterators (including `end()`) and views to strings
		/// stored in the pool are invalidated. Otherwise, only `end()` is invalidated once the string builder goes out of scope.
		/// @return String builder object that allows inplace string emplacement into the pool.
		[[nodiscard]] string_builder_t string_builder() noexcept;

		/// Erases a string from the pool.
		/// @param where Iterator to the erased string.
		/// @pre `where` cannot be the `end()` iterator.
		/// @par Invalidation
		/// All iterators (including `end()`) and views to strings at and after `where` are invalidated.
		/// @return Iterator to the string following the erased string, or `end()` if the erased string was the last in the pool.
		iterator erase(iterator where) noexcept;

		/// Erases a range of strings from the pool.
		/// @param begin, end Pair of iterators defining the range of strings to erase.
		/// @pre `%begin` cannot be the `end()` iterator.
		/// @par Invalidation
		/// All iterators (including `end()`) and views to strings at and after `%begin` are invalidated.
		/// @return Iterator to the string following the last erased string, or `end()` if `%end` pointed to the previous `end()`.
		iterator erase(iterator begin, iterator end) noexcept;

		/// @}

	  private:
		/// @brief Backing vector of the pool.
		/// @details Strings are stored sequentially and NUL-terminated.
		std::vector<char> m_data;
	};
} // namespace tr