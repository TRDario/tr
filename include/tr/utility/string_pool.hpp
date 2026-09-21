/// @file
/// @brief Provides `tr::string_pool`.

#pragma once
#include <tr/utility/iterator_interface.hpp>
#include <tr/utility/opt_ref.hpp>
#include <tr/utility/zstring_view.hpp>

//

namespace tr
{
	/// Pooled string storage.
	class string_pool
	{
	  public:
		/// Iterator type used by the string pool.
		/// @details `tr::string_pool::iterator` satisfies `std::bidirectional_iterator`.
		class iterator : public iterator_interface<iterator>
		{
		  public:
			/// Value type used by the iterator.
			using value_type = zstring_view;

			/// Difference type used by the iterator.
			using difference_type = std::ptrdiff_t;

			/// Category of the iterator.
			using iterator_category = std::bidirectional_iterator_tag;

			/// @cond Implementation details
			/// @name Constructors and destructors
			/// @{

			/// Constructs a singular iterator.
			/// @details This is necessary to satisfy `std::semiregular`.
			[[nodiscard]] iterator() noexcept = default;

			/// Constructs an iterator.
			/// @param ptr Base iterator to the beginning of the pointed-to string.
			[[nodiscard]] explicit iterator(std::vector<char>::const_iterator base) noexcept;

			/// @}
			/// @endcond
			/// @name Comparison operators
			/// @{

			/// String pool iterators are three-way comparable.
			[[nodiscard]] friend std::strong_ordering operator<=>(iterator, iterator) noexcept = default;

			/// String pool iterators are equality-comparable.
			[[nodiscard]] friend bool operator==(iterator, iterator) noexcept = default;

			/// @}
			/// @name Other operators
			/// @{

			/// Increments the iterator.
			/// @return Reference to `*this`.
			iterator& operator++() noexcept;

			/// Decrements the iterator.
			/// @return Reference to `*this`.
			iterator& operator--() noexcept;

			/// Dereferences the iterator.
			/// @return View to the pointed-to string.
			[[nodiscard]] value_type operator*() const noexcept;

			/// @}
			/// @cond implementation_details
			/// @name Implementation details
			/// @{

			/// Gets the base iterator to the beginning of the pointed-to string.
			/// @return Base iterator to the beginning of the pointed-to string.
			[[nodiscard]] std::vector<char>::const_iterator base() const noexcept;

			/// @}
			/// @endcond

		  private:
			/// Base iterator to the beginning of the pointed-to string.
			std::vector<char>::const_iterator m_base;
		};

		/// String builder object.
		class string_builder_t
		{
		  public:
			/// @name Constructors and destructors
			/// @{

			/// Constructs a string builder.
			[[nodiscard]] string_builder_t(string_pool& pool) noexcept;

			/// String builders are not copyable.
			string_builder_t(const string_builder_t&) = delete;

			/// Moves a string builder.
			/// @param rhs String builder to move.
			[[nodiscard]] string_builder_t(string_builder_t&& rhs) noexcept;

			/// Finalizes building the string.
			~string_builder_t() noexcept;

			/// @}
			/// @name Assignment operators
			/// @{

			/// String builders are not copyable.
			string_builder_t& operator=(const string_builder_t&) = delete;

			/// Moves a string builder.
			/// @param rhs String builder to move.
			/// @return Reference to `*this`.
			string_builder_t& operator=(string_builder_t&& rhs) noexcept;

			/// @}
			/// @name Iterator
			/// @{

			/// Gets a character back inserter iterator from the appender.
			/// @return Character back inserter iterator.
			[[nodiscard]] std::back_insert_iterator<std::vector<char>> out() const noexcept;

			/// @}

		  private:
			/// Optional refernce to the underlying pool.
			opt_ref<string_pool> m_pool;
		};

		/// @name Constructors and destructors
		/// @{

		/// Constructs an empty string pool.
		[[nodiscard]] string_pool() noexcept = default;

		/// Copies a string pool.
		/// @param rhs Pool to copy.
		[[nodiscard]] string_pool(const string_pool& rhs) = default;

		/// Moves a string pool.
		/// @param rhs Pool to move.
		[[nodiscard]] string_pool(string_pool&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Copies a string pool.
		/// @note This invalidates all string views and iterators to strings previously held in the pool.
		/// @param rhs Pool to copy.
		/// @return Reference to `*this`.
		string_pool& operator=(const string_pool& rhs) = default;

		/// Copies a string pool.
		/// @note This invalidates all string views and iterators to strings previously held in the pool.
		/// @param rhs Pool to move.
		/// @return Reference to `*this`.
		string_pool& operator=(string_pool&& rhs) noexcept = default;

		/// @}
		/// @name Iterators
		/// @{

		/// Gets an iterator to the beginning of the pool.
		/// @return Iterator to the beginning of the pool.
		[[nodiscard]] iterator begin() const noexcept;

		/// Gets an iterator to the end of the pool.
		/// @return Iterator to the end of the pool.
		[[nodiscard]] iterator end() const noexcept;

		/// @}
		/// @name Capacity
		/// @{

		/// Gets whether the string pool is empty.
		/// @return `true` if the string pool is empty, `false` otherwise.
		[[nodiscard]] bool empty() const noexcept;

		/// Gets the capacity of the string pool.
		/// @return Capacity of the string pool in bytes.
		[[nodiscard]] usize capacity() const noexcept;

		/// Reserves a certain capacity in the string pool.
		/// @param capacity Desired capacity of the string pool in bytes.
		void reserve(usize capacity);

		/// @}
		/// @name Manipulation
		/// @{

		/// Clears the string pool.
		void clear() noexcept;

		/// Appends a string to the string pool.
		/// @note This invalidates all string views and iterators to strings in the pool if a reallocation occurs.
		/// @return Iterator to the appended string.
		iterator append(std::string_view string);

		/// Creates a string builder object to enable inplace string emplacement.
		/// @note It is not allowed to have multiple string builders alive at once for a given string pool.
		/// @return String builder object.
		[[nodiscard]] string_builder_t string_builder() noexcept;

		/// Erases a string from the string pool.
		/// @note This invalidates all string views and iterators to strings after `where` in the pool.
		/// @param where Iterator to the erased string.
		/// @return Iterator to the string after the erased string.
		iterator erase(iterator where) noexcept;

		/// Erases a range of strings from the string pool.
		/// @note This invalidates all string views and iterators to strings after `end` in the pool.
		/// @param begin Iterator to the first erased string.
		/// @param end Iterator to one past the last erased string.
		/// @return Iterator to the string after the erased strings.
		iterator erase(iterator begin, iterator end) noexcept;

		/// @}

	  private:
		/// Backing vector of the pool.
		/// @details Strings are stored sequentially and NUL-terminated.
		std::vector<char> m_data;
	};
} // namespace tr