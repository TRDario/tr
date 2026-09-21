/// @file
/// @brief Provides `tr::localization_map`.

#pragma once
#include <tr/utility/hash_map.hpp>
#include <tr/utility/internal/binding.hpp>
#include <tr/utility/string_pool.hpp>

//

namespace tr
{
	/// Concept denoting a type suitable for a localization key-value pair.
	/// @details To fulfill this concept, a type has to destructure to two values convertible to `std::string_view`.
	/// @note This concept is currently only enforced on Clang.
	/// @hideinitializer
	template <typename T>
	concept localization_key_value_pair =
#ifdef __clang__
		requires {
			requires __builtin_structured_binding_size(std::remove_cvref_t<T>) == 2;
			requires std::convertible_to<internal::binding_t<0, T>, std::string_view>;
			requires std::convertible_to<internal::binding_t<1, T>, std::string_view>;
		}
#else
		true
#endif
	;

	/// Optimized localization key-value map.
	class localization_map
	{
	  public:
		/// @name Constructors and destructors
		/// @{

		/// Constructs an empty localization map.
		[[nodiscard]] localization_map() noexcept = default;

		/// Constructs a localization map from a key-value iterator pair range.
		/// @pre The key-value range must not contain duplicate keys.
		/// @tparam Iterator Input iterator to a range of key-value pairs.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param begin Iterator to the first key-value pair to add.
		/// @param end Ending sentinel for the key-value pair range.
		template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
			requires(localization_key_value_pair<std::iter_reference_t<Iterator>>)
		[[nodiscard]] localization_map(Iterator begin, Sentinel end)
		{
			if constexpr (std::forward_iterator<Iterator>) {
#ifdef TR_ENABLE_ASSERTS
				for (Iterator it = begin; it != end; ++it) {
					for (Iterator jt = std::next(it); jt != end; ++jt) {
						auto&& [ikey, ivalue]{*it};
						auto&& [jkey, jvalue]{*jt};
						if (static_cast<std::string_view>(ikey) == static_cast<std::string_view>(jkey)) {
							TR_ASSERT(false, "Localization key-value range contains duplicate keys.");
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
				m_pool.reserve(string_pool_capacity);
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

		/// Constructs a localization map from a key-value range.
		/// @tparam Range Key-value pair input range type.
		/// @pre The key-value range must not contain duplicate keys.
		/// @param range Key-value pair input range.
		template <std::ranges::input_range Range = std::initializer_list<std::pair<std::string_view, std::string_view>>>
			requires(localization_key_value_pair<std::ranges::range_reference_t<Range>>)
		[[nodiscard]] explicit localization_map(Range&& range)
			: localization_map(std::ranges::begin(range), std::ranges::end(range))
		{
		}

		/// Copies a localization map.
		/// @param rhs Localization map to copy.
		[[nodiscard]] localization_map(const localization_map& rhs);

		/// Moves a localization map.
		/// @details `rhs` will be left empty after the move.
		/// @param rhs Localization map to move.
		[[nodiscard]] localization_map(localization_map&& rhs) noexcept = default;

		/// @}
		/// @name Assignment operators
		/// @{

		/// Copies a localization map.
		/// @param rhs Localization map to copy.
		/// @return Reference to `*this`.
		localization_map& operator=(const localization_map& rhs);

		/// Moves a localization map.
		/// @details `rhs` will be left empty after the move.
		/// @param rhs Localization map to move.
		/// @return Reference to `*this`.
		localization_map& operator=(localization_map&& rhs) noexcept = default;

		/// @}
		/// @name Access
		/// @{

		/// Gets the number of entries in the map.
		/// @return Number of entries in the map.
		[[nodiscard]] usize size() const noexcept;

		/// Gets whether a key has a corresponding localization string in the map.
		/// @param key Localization key to check.
		/// @return `true` if a string is associated with `key`, `false` otherwise.
		[[nodiscard]] bool contains(std::string_view key) const noexcept;

		/// Gets a localization string associated with a key.
		/// @param key Localization key to get a localization string for.
		/// @post The string `key` is a view of must stay alive after the function returns in case it's returned.
		/// @return Localization string associated with a key, or `key` if one doesn't exist.
		[[nodiscard]] std::string_view operator[](std::string_view key) const noexcept;

		/// @}
		/// @name Modification
		/// @{

		/// Clears the localization map.
		void clear() noexcept;

		/// Adds or replaces a key-value pair in the map.
		/// @note This may invalidate any previously obtained localization string views.
		/// @param key Localization key.
		/// @param value Localization value associated with `key`.
		void update(std::string_view key, std::string_view value);

		/// Adds and replaces key-value pairs in the map.
		/// @note This may invalidate any previously obtained localization string views.
		/// @pre The key-value range must not contain duplicate keys.
		/// @tparam Iterator Input iterator to a range of key-value pairs.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param begin Iterator to the first key-value pair to add or replace.
		/// @param end Ending sentinel for the key-value pair range.
		template <std::input_iterator Iterator, std::sentinel_for<Iterator> Sentinel>
			requires(localization_key_value_pair<std::iter_reference_t<Iterator>>)
		void update(Iterator begin, Sentinel end)
		{
			usize keys{0};
			bool stale_keys{false};
			const usize old_pool_capacity{m_pool.capacity()};

			for (const auto& [key, value] : std::ranges::subrange{begin, end}) {
				++keys;
				if (!stale_keys) {
					if (const auto existing_copy_it{m_keys.find(key)}; existing_copy_it != m_keys.end()) {
						stale_keys = true;
						m_pool.erase(*existing_copy_it, std::next(*existing_copy_it, 2));
					}
				}
				else {
					for (auto it = m_pool.begin(); it != m_pool.end(); std::advance(it, 2)) {
						if (*it == key) {
							m_pool.erase(it, std::next(it, 2));
							break;
						}
					}
				}

				m_pool.append(key);
				m_pool.append(value);
				if (m_pool.capacity() > old_pool_capacity) {
					stale_keys = true;
				}
			}

			if (stale_keys) {
				m_keys.clear();
				for (string_pool::iterator it{m_pool.begin()}; it != m_pool.end(); std::advance(it, 2)) {
					m_keys.insert(it);
				}
			}
			else if (!m_pool.empty()) {
				for (auto [i, it] = std::pair{0uz, std::prev(m_pool.end(), 2)}; i < keys; ++i, std::advance(it, -2)) {
					m_keys.insert(it);
				}
			}
		}

		/// Adds and replaces key-value pairs in the map.
		/// @note This may invalidate any previously obtained localization string views.
		/// @tparam Iterator Input iterator to a range of key-value pairs.
		/// @tparam Sentinel Sentinel for `Iterator`.
		/// @param range Key-value pair input range.
		template <std::ranges::input_range Range = std::initializer_list<std::pair<std::string_view, std::string_view>>>
			requires(localization_key_value_pair<std::ranges::range_reference_t<Range>>)
		void update(Range&& range)
		{
			update(std::ranges::begin(range), std::ranges::end(range));
		}

		/// @}

	  private:
		/// String pool iterator hasher.
		struct string_pool_iterator_hash : public string_hash
		{
			using string_hash::operator();

			/// Hashes a string pool iterator.
			/// @param it Iterator to hash.
			[[nodiscard]] usize operator()(string_pool::iterator it) const noexcept;
		};

		/// String pool iterator equality comparison object.
		struct string_pool_iterator_eq : public string_eq, public std::equal_to<string_pool::iterator>
		{
			using string_eq::operator();

			using std::equal_to<string_pool::iterator>::operator();

			/// Compares a string pool iterator with a string view.
			/// @param lhs Iterator to compare.
			/// @param rhs String view to compare.
			/// @return Whether the string pointed to by the iterator and `rhs` are equal.
			[[nodiscard]] bool operator()(string_pool::iterator lhs, std::string_view rhs) const noexcept;

			/// Compares a string pool iterator with a string view.
			/// @param lhs String view to compare.
			/// @param rhs Iterator to compare.
			/// @return Whether the string pointed to by the iterator and `lhs` are equal.
			[[nodiscard]] bool operator()(std::string_view lhs, string_pool::iterator rhs) const noexcept;
		};

		//

		/// Key-value string pair storage pool.
		/// @details Key-value strings are stored interleaved separated by NUL.
		string_pool m_pool;

		/// Set of keys present in the map.
		boost::unordered_flat_set<string_pool::iterator, string_pool_iterator_hash, string_pool_iterator_eq> m_keys;
	};
} // namespace tr