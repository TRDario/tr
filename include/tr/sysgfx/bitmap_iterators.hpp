#pragma once
#include "bitmap.hpp"

namespace tr {
	// Read-only reference to a bitmap pixel.
	class sub_bitmap::pixel_ref {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const noexcept;

	  private:
		// A pointer to the pixel data.
		const std::byte* _impl;
		// The format of the pixel.
		pixel_format _format;

		pixel_ref() noexcept = default;
		pixel_ref(const std::byte* ptr, pixel_format format) noexcept;

		friend class sub_bitmap;
		friend class Iterator;
	};

	// Immutable bitmap pixel iterator.
	class sub_bitmap::iterator {
	  public:
		using value_type = pixel_ref;
		using pointer = const value_type*;
		using difference_type = int;

		// Default-constructs an iterator.
		iterator() noexcept = default;
		// Constructs an iterator to a bitmap pixel.
		iterator(const sub_bitmap& bitmap, glm::ivec2 pos) noexcept;

		std::partial_ordering operator<=>(const iterator&) const noexcept;
		bool operator==(const iterator&) const noexcept;

		// Dereferences the iterator.
		value_type operator*() const noexcept;
		// Dereferences the iterator with a subscript.
		value_type operator[](difference_type diff) const noexcept;
		// Dereferences the iterator with a subscript.
		value_type operator[](glm::ivec2 diff) const noexcept;
		// Dereferences the iterator through a pointer.
		pointer operator->() const noexcept;

		// Increments the iterator.
		iterator& operator++() noexcept;
		// Post-increments the iterator.
		iterator operator++(int) noexcept;
		// Advances the iterator.
		iterator& operator+=(difference_type diff) noexcept;
		// Advances an iterator.
		iterator& operator+=(glm::ivec2 diff) noexcept;
		// Adds to an iterator.
		friend iterator operator+(const iterator& it, difference_type diff) noexcept;
		// Adds to an iterator.
		friend iterator operator+(difference_type diff, const iterator& it) noexcept;
		// Adds to an iterator.
		friend iterator operator+(const iterator& it, glm::ivec2 diff) noexcept;
		// Adds to an iterator.
		friend iterator operator+(glm::ivec2 diff, const iterator& it) noexcept;

		// Decrements the iterator.
		iterator& operator--() noexcept;
		// Post-decrements the iterator.
		iterator operator--(int) noexcept;
		// Moves the iterator back.
		iterator& operator-=(difference_type diff) noexcept;
		// Moves the iterator back.
		iterator& operator-=(glm::ivec2 diff) noexcept;
		// Subtracts from an iterator.
		friend iterator operator-(const iterator& it, difference_type diff) noexcept;
		// Subtracts from an iterator.
		friend iterator operator-(difference_type diff, const iterator& it) noexcept;
		// Subtracts from an iterator.
		friend iterator operator-(const iterator& it, glm::ivec2 diff) noexcept;
		// Subtracts from an iterator.
		friend iterator operator-(glm::ivec2 diff, const iterator& it) noexcept;
		// Gets the distance between two iterators.
		friend difference_type operator-(const iterator& l, const iterator& r) noexcept;

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const noexcept;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		pixel_ref _pixel;
		// The size of the pointed-to bitmap.
		glm::ivec2 _bitmap_size;
		// The pitch of the pointed-to-bitmap.
		int _pitch;
		// The position of the iterator within the bitmap.
		glm::ivec2 _pos;
	};

	// Mutable reference to a bitmap pixel.
	class bitmap::pixel_ref {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const noexcept;
		// Sets the pixel's color value.
		pixel_ref& operator=(rgba8 color) noexcept;

	  private:
		// A pointer to the pixel data.
		std::byte* _impl;
		// The format of the pixel.
		pixel_format _format;

		pixel_ref() noexcept = default;
		pixel_ref(std::byte* ptr, pixel_format format) noexcept;

		friend class mut_it;
	};

	// Mutable pixel iterator.
	class bitmap::mut_it {
	  public:
		using value_type = pixel_ref;
		using pointer = const value_type*;
		using difference_type = int;

		// Default-constructs an iterator.
		mut_it() noexcept = default;
		// Constructs an iterator to a bitmap pixel.
		mut_it(bitmap& bitmap, glm::ivec2 pos) noexcept;

		std::partial_ordering operator<=>(const mut_it&) const noexcept;
		bool operator==(const mut_it&) const noexcept;

		// Dereferences the iterator.
		value_type operator*() const noexcept;
		// Dereferences the iterator with a subscript.
		value_type operator[](difference_type diff) const noexcept;
		// Dereferences the iterator with a subscript.
		value_type operator[](glm::ivec2 diff) const noexcept;
		// Dereferences the iterator through a pointer.
		pointer operator->() const noexcept;

		// Increments the iterator.
		mut_it& operator++() noexcept;
		// Post-increments the iterator.
		mut_it operator++(int) noexcept;
		// Advances the iterator.
		mut_it& operator+=(difference_type diff) noexcept;
		// Advances the iterator.
		mut_it& operator+=(glm::ivec2 diff) noexcept;
		// Adds to an iterator.
		friend mut_it operator+(const mut_it& it, difference_type diff) noexcept;
		// Adds to an iterator.
		friend mut_it operator+(difference_type diff, const mut_it& it) noexcept;
		// Adds to an iterator.
		friend mut_it operator+(const mut_it& it, glm::ivec2 diff) noexcept;
		// Adds to an iterator.
		friend mut_it operator+(glm::ivec2 diff, const mut_it& it) noexcept;

		// Decrements the iterator.
		mut_it& operator--() noexcept;
		// Post-decrements the iterator.
		mut_it operator--(int) noexcept;
		// Moves the iterator back.
		mut_it& operator-=(difference_type diff) noexcept;
		// Moves the iterator back.
		mut_it& operator-=(glm::ivec2 diff) noexcept;
		// Subtracts from an iterator.
		friend mut_it operator-(const mut_it& it, difference_type diff) noexcept;
		// Subtracts from an iterator.
		friend mut_it operator-(difference_type diff, const mut_it& it) noexcept;
		// Subtracts from an iterator.
		friend mut_it operator-(const mut_it& it, glm::ivec2 diff) noexcept;
		// Subtracts from an iterator.
		friend mut_it operator-(glm::ivec2 diff, const mut_it& it) noexcept;
		// Gets the distance between two iterators.
		friend difference_type operator-(const mut_it& l, const mut_it& r) noexcept;

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const noexcept;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		pixel_ref _pixel;
		// The pointed-to-bitmap.
		bitmap* _bitmap;
		// The position of the iterator within the bitmap.
		glm::ivec2 _pos;
	};
} // namespace tr