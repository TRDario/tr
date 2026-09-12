/// @file
/// @brief Provides `tr::lock_free_queue`.

#pragma once
#include <tr/utility/concepts.hpp>
#include <tr/utility/opt_ref.hpp>

//

namespace tr
{
	/// Lock-free inplace queue intended for use in multi-threaded, high-performance environments.
	/// @details
	/// `tr::lock_free_queue` provides a circular buffer queue stored inplace and designed to be safely used in a multithreaded environment
	/// without the use of locks. Specifically, it implements Dmitry Vyukov's MPMC bounded queue design.
	///
	/// Instances of `tr::lock_free_queue` are not copyable nor movable.
	///
	/// @note Smaller objects may have a significant amount of memory overhead comparative to their size.
	/// @tparam Element Type of the elements of the queue.
	/// @tparam Capacity Maximum capacity of the queue. Due to underlying implementation details may only be in powers of two.
	template <nothrow_destructible Element, usize Capacity>
		requires(std::has_single_bit(Capacity))
	class lock_free_queue
	{
	  public:
		/// @name Constructors
		/// @{

		/// Creates an empty queue.
		[[nodiscard]] lock_free_queue() noexcept
			: m_write_position{0}
			, m_read_position{0}
		{
			for (usize i = 0; slot& slot : m_slots) {
				slot.sequence.store(i++, std::memory_order::relaxed);
			}
		}

		/// Lock-free queues are not copyable.
		lock_free_queue(const lock_free_queue&) = delete;

		/// Lock-free queues are not movable.
		lock_free_queue(lock_free_queue&&) = delete;

		/// Destroys the queue.
		~lock_free_queue() noexcept
		{
			if constexpr (!std::is_trivially_destructible_v<Element>) {
				const std::size_t first{m_read_position.load(std::memory_order_relaxed)};
				const std::size_t last{m_write_position.load(std::memory_order_relaxed)};
				for (std::size_t i = first; i < last; ++i) {
					slot& slot{m_slots[i & (Capacity - 1)]};
					if (slot.sequence.load(std::memory_order_relaxed) == i + 1) {
						slot.element().~Element();
					}
				}
			}
		}

		/// @}
		/// @name Assignment operators
		/// @{

		/// Lock-free queues are not copyable.
		lock_free_queue& operator=(const lock_free_queue&) = delete;

		/// Lock-free queues are not movable.
		lock_free_queue& operator=(lock_free_queue&&) = delete;

		/// @}
		/// @name Access
		/// @{

		/// Attempts to emplace an element into the back of the queue.
		/// @tparam Args Types of the arguments to the constructor of a value.
		/// @param args Arguments to the constructor of a value.
		/// @return `true` if the element was successfully emplaced, `false` otherwise.
		template <typename... Args>
			requires(std::constructible_from<Element, Args...>)
		bool try_emplace_back(Args&&... args) noexcept(std::is_nothrow_constructible_v<Element, Args...>)
		{
			slot* slot{nullptr};
			std::size_t position{m_write_position.load(std::memory_order_relaxed)};
			// Spin until a valid slot is found or the buffer is full.
			while (true) {
				slot = &m_slots[position & (Capacity - 1)];
				std::size_t sequence{slot->sequence.load(std::memory_order_acquire)};
				std::ptrdiff_t diff{static_cast<std::ptrdiff_t>(sequence - position)};
				// Slot seems available.
				if (diff == 0) {
					// Check for contention.
					if (m_write_position.compare_exchange_weak(position, position + 1, std::memory_order_relaxed)) {
						break;
					}
				}
				// Queue is full, return.
				else if (diff < 0) {
					return false;
				}
				// Stale position, check again.
				else {
					position = m_write_position.load(std::memory_order_relaxed);
				}
			}

			new (slot->storage) Element{std::forward<Args>(args)...};

			// Set sequence to signal slot value construction is over.
			slot->sequence.store(position + 1, std::memory_order_release);
			return true;
		}

		/// Attempts to pop a value from the front of the queue.
		/// @param[out] out Value to write the output to.
		/// @return `true` if a value was popped from the queue, `false` otherwise.
		bool try_pop_front(Element& out) noexcept(std::is_nothrow_move_assignable_v<Element>)
			requires(std::is_move_assignable_v<Element>)
		{
			slot* slot{nullptr};
			std::size_t position{m_read_position.load(std::memory_order_relaxed)};
			while (true) {
				slot = &m_slots[position & (Capacity - 1)];
				std::size_t sequence{slot->sequence.load(std::memory_order_acquire)};
				std::ptrdiff_t diff{static_cast<std::ptrdiff_t>(sequence - (position + 1))};
				// Slot seems available.
				if (diff == 0) {
					// Check for contention.
					if (m_read_position.compare_exchange_weak(position, position + 1, std::memory_order_relaxed)) {
						break;
					}
				}
				// Buffer is empty, return.
				else if (diff < 0) {
					return false;
				}
				// Slot occupied, check again.
				else {
					position = m_read_position.load(std::memory_order_relaxed);
				}
			}

			/// Move value out and destroy it.
			Element& slot_value{slot->element()};
			out = std::move(slot_value);
			slot_value.~Element();

			// Set sequence to signal slot value destruction is over.
			slot->sequence.store(position + Capacity, std::memory_order_release);
			return true;
		}

		/// Attempts to pop a value from the front of the queue.
		/// @return The popped value, or an empty optional is the buffer is empty.
		[[nodiscard]] std::optional<Element> try_pop_front() noexcept(std::is_nothrow_move_constructible_v<Element>)
		{
			slot* slot{nullptr};
			std::size_t position{m_read_position.load(std::memory_order_relaxed)};
			while (true) {
				slot = &m_slots[position & (Capacity - 1)];
				std::size_t sequence{slot->sequence.load(std::memory_order_acquire)};
				std::ptrdiff_t diff{static_cast<std::ptrdiff_t>(sequence - (position + 1))};
				// Slot seems available.
				if (diff == 0) {
					// Check for contention.
					if (m_read_position.compare_exchange_weak(position, position + 1, std::memory_order_relaxed)) {
						break;
					}
				}
				// Buffer is empty, return.
				else if (diff < 0) {
					return std::nullopt;
				}
				// Slot occupied, check again.
				else {
					position = m_read_position.load(std::memory_order_relaxed);
				}
			}

			/// Move value out and destroy it.
			Element& slot_value{slot->element()};
			std::optional<Element> value{std::in_place, std::move(slot_value)};
			slot_value.~Element();

			// Set sequence to signal slot value destruction is over.
			slot->sequence.store(position + Capacity, std::memory_order_release);
			return value;
		}

		/// @}

	  private:
		/// Slot where an element is stored.
		struct slot
		{
			/// Sequence number of the slot.
			std::atomic_size_t sequence;

			/// Element storage.
			alignas(Element) std::byte storage[sizeof(Element)];

			//

			/// Gets a reference to the element contained within the slot.
			/// @return Reference to the element contained within the slot.
			[[nodiscard]] Element& element() noexcept
			{
				return *std::launder(reinterpret_cast<Element*>(storage));
			}
		};

		//

		/// Slots of the queue.
		alignas(64) std::array<slot, Capacity> m_slots;

		/// Position of the write head.
		alignas(64) std::atomic_size_t m_write_position;

		/// Position of the read head.
		alignas(64) std::atomic_size_t m_read_position;
	};
} // namespace tr