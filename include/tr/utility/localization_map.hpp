/// @file
/// @brief Provides `tr::localization_map`.

#pragma once
#include <tr/utility/hash_map.hpp>
#include <tr/utility/internal/binding.hpp>
#include <tr/utility/ranges.hpp>
#include <tr/utility/string_pool.hpp>

//

namespace tr
{
	/// @brief Concept checking whether variables of type `Target` are suitable for use as key-value input pairs for `tr::localization_map`.
	/// @details To fulfill this concept, variables of type `Target` must be able to destructure to two values convertible to
	/// `std::string_view`. On platforms other than Clang, `Target` must have defined `std::tuple_size` and `std::tuple_element`
	/// specializations to qualify.
	/// @hideinitializer
	template <typename Target>
	concept localization_key_value_pair =
#if TR_HAS_BUILTIN(__builtin_structured_binding_size)
		requires {
			requires __builtin_structured_binding_size(std::remove_cvref_t<Target>) == 2;
			requires std::convertible_to<internal::binding_t<0, Target>, std::string_view>;
			requires std::convertible_to<internal::binding_t<1, Target>, std::string_view>;
		}
#else
		requires {
			typename std::tuple_size<std::remove_cvref_t<Target>>::type;
			requires std::tuple_size_v<std::remove_cvref_t<Target>> == 2;
			requires std::convertible_to<std::tuple_element_t<0, std::remove_cvref_t<Target>>, std::string_view>;
			requires std::convertible_to<std::tuple_element_t<1, std::remove_cvref_t<Target>>, std::string_view>;
		}
#endif
	;

	/// @brief Read-optimized container for storing key-value string pairs.
	/// @details
	/// `tr::localization_map` is designed for efficient storage and lookup of string key-value pairs, primarily for use in storing
	/// localization strings. Rather than allocating individual buffers for each string, all strings are packed sequentially inside a
	/// single, contiguous buffer, eliminating allocation overhead and heap fragmentation.
	///
	/// While most optimized when created complete and treated as read-only, localization maps can also be dynamically expanded and updated
	/// at any point. Doing this invalidates all previously obtained views to strings stored in the map.
	class localization_map
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// @brief Constructs an empty localization map.
		[[nodiscard]] localization_map() noexcept = default;

		/// @brief Constructs a localization map by copying a range of values.
		/// @details If `Iterator` models `std::forward_iterator`, the range `[begin, end)` is iterated through at least twice: once to
		/// precalculate the required allocation size, and once to populate the map. Otherwise, every iterator in `[begin, end)` is
		/// dereferenced exactly once.
		/// @tparam Iterator Input iterator to a key-value pair.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param begin, end Pair of iterators defining the range of key-value pairs to copy from.
		/// @pre The key-value range must not contain duplicate keys.
		/// @par Complexity
		/// Linear time with respect to the size of `[begin, end)`.
		/// @par Exception safety
		/// If an exception is thrown other than from an iterator operation, this function has no effect (strong exception guarantee).
		template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
			requires localization_key_value_pair<std::iter_reference_t<Iterator>>
		[[nodiscard]] localization_map(Iterator begin, Sentinel end)
		{
			if constexpr (std::forward_iterator<Iterator>) {
#ifdef TR_ENABLE_ASSERTS
				for (Iterator it = begin; it != end; ++it) {
					for (Iterator jt = std::ranges::next(it); jt != end; ++jt) {
						auto&& [ikey, ivalue]{*it};
						auto&& [jkey, jvalue]{*jt};
						if (static_cast<std::string_view>(ikey) == static_cast<std::string_view>(jkey)) {
							TR_ASSERT(false, "Localization map input key-value range contains duplicate keys.");
						}
					}
				}
#endif

				usize keys{0};
				usize string_pool_capacity{0};
				for (const auto& [key, value] : std::ranges::subrange{begin, end}) {
					++keys;
					string_pool_capacity += (static_cast<std::string_view>(key).size() + static_cast<std::string_view>(value).size() + 2);
				}
				m_pool.reserve_bytes(string_pool_capacity);
				m_keys.reserve(keys);
			}

			for (const auto& [key, value] : std::ranges::subrange{begin, end}) {
				m_pool.append(key);
				m_pool.append(value);
			}
			for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::advance(it, 2)) {
				m_keys.insert(it);
			}
		}

		/// @brief Constructs a localization map by copying a range of values.
		/// @details If `Range` models `std::forward_range`, `range` is iterated through at least twice: once to precalculate the required
		/// allocation size, and once to populate the map. Otherwise, every iterator in `range` is dereferenced exactly once.
		/// @tparam Range Input range of key-value pairs.
		/// @param range Range of key-value pairs to copy from.
		/// @pre `range` must not contain duplicate keys.
		/// @par Complexity
		/// Linear time with respect to the size of `range`.
		/// @par Exception safety
		/// If an exception is thrown other than from an iterator operation, this function has no effect (strong exception guarantee).
		template <std::ranges::input_range Range = std::initializer_list<std::pair<std::string_view, std::string_view>>>
			requires localization_key_value_pair<std::ranges::range_reference_t<Range>>
		[[nodiscard]] explicit localization_map(Range&& range)
			: localization_map(std::ranges::begin(range), std::ranges::end(range))
		{
		}

		/// @brief Constructs a localization map by copying the contents of another map.
		/// @param rhs Map to copy the contents of.
		/// @par Complexity
		/// Linear time with respect to `rhs.size()`.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		[[nodiscard]] localization_map(const localization_map& rhs);

		/// @brief Constructs a localization map by moving the contents of another map.
		/// @param rhs Map to move the contents of.
		/// @post `rhs` is left in a valid, but unspecified state after the move.
		[[nodiscard]] localization_map(localization_map&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// @brief Replaces the contents of the localization map with a copy of those of another pool.
		/// @param rhs Map to copy the contents of.
		/// @par Invalidation
		/// All views to strings previously stored in `*this` are invalidated.
		/// @par Complexity
		/// Linear time with respect to `rhs.size()`.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		/// @return Reference to `*this`.
		localization_map& operator=(const localization_map& rhs);

		/// @brief Replaces the contents of the localization map with those moved from another pool.
		/// @param rhs Map to move the contents of.
		/// @par Invalidation
		/// All views to strings previously stored in `*this` are invalidated.
		/// @post `rhs` is left in a valid, but unspecified state after the move.
		/// @return Reference to `*this`.
		localization_map& operator=(localization_map&& rhs) noexcept = default;

		/// @}
		/// @name Access
		/// @{

		/// @brief Returns the number of entries in the map.
		/// @return Number of entries in the map.
		[[nodiscard]] usize size() const noexcept;

		/// @brief Checks whether a key has an associated value string in the map.
		/// @param key Key string to check.
		/// @return `true` if a value string associated with `key` exists in the map, `false` otherwise.
		[[nodiscard]] bool contains(std::string_view key) const noexcept;

		/// @brief Tries to return the value string associated with a key string.
		/// @param key Key string to get the associated value string for.
		/// @post The string backing `key` must stay valid after the function returns in case it's returned.
		/// @return Value string associated with `key`, or `key` if one doesn't exist.
		[[nodiscard]] std::string_view operator[](std::string_view key) const noexcept;

		/// @brief Returns the value string associated with a key string or a default value if one doesn't exist.
		/// @param key Key string to get the associated value string for.
		/// @param default_value Default value string to return if there is no value string associated with `key` in the map.
		/// @post The string backing `default_value` must stay valid after the function returns in case it's returned.
		/// @return Value string associated with `key`, or `default_value` if one doesn't exist.
		[[nodiscard]] std::string_view value_or(std::string_view key, std::string_view default_value) const noexcept;

		/// @}
		/// @name Modification
		/// @{

		/// @brief Erases all entries from the map.
		/// @details No allocations made by the map are released by this operation.
		/// @par Invalidation
		/// All views to strings previously stored in the map are invalidated.
		void clear() noexcept;

		/// @brief Adds or replaces a key-value pair in the map.
		/// @param key Key string.
		/// @param value Value string associated with `key`.
		/// @par Invalidation
		/// All views to strings previously stored in the map are invalidated.
		/// @par Complexity
		/// At most linear with respect to `size()`.
		/// @par Exception safety
		/// If an exception is thrown, this function has no effect (strong exception guarantee).
		void update(std::string_view key, std::string_view value);

		/// @brief Adds or replaces a range of key-value pairs in the map.
		/// @details The range `[begin, end)` is iterated through at least twice: once to precalculate the required allocation size, and
		/// once to populate the map.
		/// @tparam Iterator Forward input iterator to a key-value pair.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param begin, end Pair of iterators defining the range of key-value pairs to update from.
		/// @par Invalidation
		/// All views to strings previously stored in the map are invalidated.
		/// @par Complexity
		/// At most O(n * m) with respect to `size()` and `[begin, end)`.
		/// @par Exception safety
		/// If an exception is thrown other than from an iterator operation, this function has no effect (strong exception guarantee).
		template <std::forward_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
			requires localization_key_value_pair<std::iter_reference_t<Iterator>>
		void update(Iterator begin, Sentinel end)
		{
#ifdef TR_ENABLE_ASSERTS
			for (Iterator it = begin; it != end; ++it) {
				for (Iterator jt = std::ranges::next(it); jt != end; ++jt) {
					auto&& [ikey, ivalue]{*it};
					auto&& [jkey, jvalue]{*jt};
					if (static_cast<std::string_view>(ikey) == static_cast<std::string_view>(jkey)) {
						TR_ASSERT(false, "Localization map input key-value range contains duplicate keys.");
					}
				}
			}
#endif

			const usize old_size{size()};
			usize new_size{old_size};
			usize erased_bytes{0};
			usize appended_bytes{0};
			for (const auto& [key, value] : std::ranges::subrange{begin, end}) {
				const std::string_view key_string{static_cast<std::string_view>(key)};
				const std::string_view value_string{static_cast<std::string_view>(value)};

				if (const decltype(m_keys)::const_iterator key_it{m_keys.find(key_string)}; key_it != m_keys.end()) {
					erased_bytes += key_string.size() + std::ranges::distance(*std::ranges::next(*key_it)) + 2;
				}
				else {
					++new_size;
				}
				appended_bytes += key_string.size() + value_string.size() + 2;
			}
			const usize new_pool_size{m_pool.size_bytes() - erased_bytes + appended_bytes};
			const bool reallocated_pool{m_pool.capacity_bytes() < new_pool_size};

			if (new_size != old_size) {
				m_keys.reserve(new_size);
			}
			if (reallocated_pool) {
				m_pool.reserve_bytes(std::bit_ceil(new_pool_size));
				m_keys.clear();
				for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::ranges::advance(it, 2)) {
					m_keys.insert(it);
				}
			}

			try {
				for (const auto& [key, value] : std::ranges::subrange{begin, end}) {
					const std::string_view key_string{static_cast<std::string_view>(key)};
					const std::string_view value_string{static_cast<std::string_view>(value)};

					if (const decltype(m_keys)::const_iterator key_it_it{m_keys.find(key_string)}; key_it_it != m_keys.end()) {
						const string_pool::iterator key_it{*key_it_it};
						boost::unordered::erase_if(m_keys, [=](string_pool::iterator it) { return it >= key_it; });
						const string_pool::iterator begin_it{m_pool.erase(key_it, std::ranges::next(key_it, 2))};
						for (string_pool::iterator it{begin_it}; it != m_pool.end(); std::ranges::advance(it, 2)) {
							m_keys.insert(it);
						}
					}
					m_keys.insert(m_pool.append(key_string));
					m_pool.append(value_string);
				}
			}
			catch (std::exception& error) {
				clear();
				throw error;
			}
		}

		/// @brief Adds or replaces a range of key-value pairs in the map.
		/// @details The range `[begin, end)` is iterated through exactly once.
		/// @tparam Iterator Input iterator to a key-value pair.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param begin, end Pair of iterators defining the range of key-value pairs to update from.
		/// @par Invalidation
		/// All views to strings previously stored in the map are invalidated unless the range is empty.
		/// @par Complexity
		/// At most O(n * m) with respect to `size()` and `[begin, end)`.
		/// @par Exception safety
		/// If an exception is thrown, `*this` is left in a valid, but unspecified state (basic exception guarantee).
		template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
			requires localization_key_value_pair<std::iter_reference_t<Iterator>>
		void update(Iterator begin, Sentinel end)
		{
			try {
				usize range_distance{0};
				bool replaced_keys_or_reallocated_pool{false};
				const usize old_pool_capacity{m_pool.capacity_bytes()};

				for (const auto& [key, value] : std::ranges::subrange{begin, end}) {
					++range_distance;
					if (!replaced_keys_or_reallocated_pool) {
						if (const decltype(m_keys)::const_iterator key_it{m_keys.find(key)}; key_it != m_keys.end()) {
							replaced_keys_or_reallocated_pool = true;
							m_pool.erase(*key_it, std::ranges::next(*key_it, 2));
						}
					}
					else if (const string_pool::iterator key_it{std::ranges::find(m_pool | std::views::stride(2), key).base()};
							 key_it != m_pool.end()) {
						m_pool.erase(key_it, std::ranges::next(key_it, 2));
					}

					m_pool.append(key);
					m_pool.append(value);
					if (m_pool.capacity_bytes() > old_pool_capacity) {
						replaced_keys_or_reallocated_pool = true;
					}
				}

				if (replaced_keys_or_reallocated_pool) {
					m_keys.clear();
					for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::ranges::advance(it, 2)) {
						m_keys.insert(it);
					}
				}
				else if (!m_pool.empty()) {
					const string_pool::iterator rbegin{std::ranges::prev(m_pool.end(), 2)};
					const string_pool::iterator rend{std::ranges::prev(rbegin, range_distance * 2)};
					for (string_pool::iterator it = rbegin; it != rend; std::ranges::advance(it, -2)) {
						m_keys.insert(it);
					}
				}
			}
			catch (std::exception& error) {
				clear();
				throw error;
			}
		}

		/// @brief Adds or replaces a range of key-value pairs in the map.
		/// @details If `Range` models `std::forward_range`, `range` is iterated through at least twice: once to precalculate the required
		/// allocation size, and once to populate the map. Otherwise, every iterator in `range` is dereferenced exactly once.
		/// @tparam Range Input range of key-value pairs.
		/// @param range Range of key-value pairs to update from.
		/// @par Invalidation
		/// All views to strings previously stored in the map are invalidated unless the range is empty.
		/// @par Complexity
		/// At most linear with respect to `size()` and the size of `range`.
		/// @par Exception safety
		/// If `Range` models `std::ranges::forward_range` and an exception is thrown other than from an iterator operation, this function
		/// has no effect (strong exception guarantee). Otherwise, `*this` is left in a valid, but unspecified state (basic exception
		/// guarantee).
		template <std::ranges::input_range Range = std::initializer_list<std::pair<std::string_view, std::string_view>>>
			requires localization_key_value_pair<std::ranges::range_reference_t<Range>>
		void update(Range&& range)
		{
			update(std::ranges::begin(range), std::ranges::end(range));
		}

		/// @}

	  private:
		/// @brief String pool iterator hasher.
		struct string_pool_iterator_hash : public string_hash
		{
			using string_hash::operator();

			/// @brief Hashes a string pool iterator.
			/// @param it Iterator to hash.
			/// @return Hash of the string pool iterator.
			[[nodiscard]] usize operator()(string_pool::iterator it) const noexcept;
		};

		/// @brief String pool iterator equality comparator.
		struct string_pool_iterator_eq : public string_eq, public std::equal_to<string_pool::iterator>
		{
			using string_eq::operator();

			using std::equal_to<string_pool::iterator>::operator();

			/// @brief Compares a string pool iterator with a string view.
			/// @param lhs Iterator to compare.
			/// @param rhs String view to compare.
			/// @return Whether the string pointed to by the iterator and `rhs` are equal.
			[[nodiscard]] bool operator()(string_pool::iterator lhs, std::string_view rhs) const noexcept;

			/// @brief Compares a string pool iterator with a string view.
			/// @param lhs String view to compare.
			/// @param rhs Iterator to compare.
			/// @return Whether the string pointed to by the iterator and `lhs` are equal.
			[[nodiscard]] bool operator()(std::string_view lhs, string_pool::iterator rhs) const noexcept;
		};

		//

		/// @brief Key-value string pair storage pool.
		/// @details Keys and values are stored interleaved.
		string_pool m_pool;

		/// @brief Hashed set of keys present in the map.
		boost::unordered_flat_set<string_pool::iterator, string_pool_iterator_hash, string_pool_iterator_eq> m_keys;
	};
} // namespace tr