#pragma once
#include "bitmap.hpp"

namespace tr {
	// Read-only reference to a bitmap pixel.
	class sub_bitmap::pixel_ref {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const;

	  private:
		// A pointer to the pixel data.
		const std::byte* ptr;
		// The format of the pixel.
		pixel_format format;

		pixel_ref() = default;
		pixel_ref(const std::byte* ptr, pixel_format format);

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
		iterator() = default;
		// Constructs an iterator to a bitmap pixel.
		iterator(const sub_bitmap& bitmap, glm::ivec2 pos);

		std::partial_ordering operator<=>(const iterator&) const;
		bool operator==(const iterator&) const;

		// Dereferences the iterator.
		value_type operator*() const;
		// Dereferences the iterator with a subscript.
		value_type operator[](difference_type diff) const;
		// Dereferences the iterator with a subscript.
		value_type operator[](glm::ivec2 diff) const;
		// Dereferences the iterator through a pointer.
		pointer operator->() const;

		// Increments the iterator.
		iterator& operator++();
		// Post-increments the iterator.
		iterator operator++(int);
		// Advances the iterator.
		iterator& operator+=(difference_type diff);
		// Advances an iterator.
		iterator& operator+=(glm::ivec2 diff);
		// Adds to an iterator.
		friend iterator operator+(const iterator& it, difference_type diff);
		// Adds to an iterator.
		friend iterator operator+(difference_type diff, const iterator& it);
		// Adds to an iterator.
		friend iterator operator+(const iterator& it, glm::ivec2 diff);
		// Adds to an iterator.
		friend iterator operator+(glm::ivec2 diff, const iterator& it);

		// Decrements the iterator.
		iterator& operator--();
		// Post-decrements the iterator.
		iterator operator--(int);
		// Moves the iterator back.
		iterator& operator-=(difference_type diff);
		// Moves the iterator back.
		iterator& operator-=(glm::ivec2 diff);
		// Subtracts from an iterator.
		friend iterator operator-(const iterator& it, difference_type diff);
		// Subtracts from an iterator.
		friend iterator operator-(difference_type diff, const iterator& it);
		// Subtracts from an iterator.
		friend iterator operator-(const iterator& it, glm::ivec2 diff);
		// Subtracts from an iterator.
		friend iterator operator-(glm::ivec2 diff, const iterator& it);
		// Gets the distance between two iterators.
		friend difference_type operator-(const iterator& l, const iterator& r);

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		pixel_ref pixel;
		// The size of the pointed-to bitmap.
		glm::ivec2 bitmap_size;
		// The pitch of the pointed-to-bitmap.
		int bitmap_pitch;
		// The position of the iterator within the bitmap.
		glm::ivec2 bitmap_pos;
	};

	// Mutable reference to a bitmap pixel.
	class bitmap::pixel_ref {
	  public:
		// Gets the pixel's color value.
		operator rgba8() const;
		// Sets the pixel's color value.
		pixel_ref& operator=(rgba8 color);

	  private:
		// A pointer to the pixel data.
		std::byte* ptr;
		// The format of the pixel.
		pixel_format format;

		pixel_ref() = default;
		pixel_ref(std::byte* ptr, pixel_format format);

		friend class mut_it;
	};

	// Mutable pixel iterator.
	class bitmap::mut_it {
	  public:
		using value_type = pixel_ref;
		using pointer = const value_type*;
		using difference_type = int;

		// Default-constructs an iterator.
		mut_it() = default;
		// Constructs an iterator to a bitmap pixel.
		mut_it(bitmap& bitmap, glm::ivec2 pos);

		std::partial_ordering operator<=>(const mut_it&) const;
		bool operator==(const mut_it&) const;

		// Dereferences the iterator.
		value_type operator*() const;
		// Dereferences the iterator with a subscript.
		value_type operator[](difference_type diff) const;
		// Dereferences the iterator with a subscript.
		value_type operator[](glm::ivec2 diff) const;
		// Dereferences the iterator through a pointer.
		pointer operator->() const;

		// Increments the iterator.
		mut_it& operator++();
		// Post-increments the iterator.
		mut_it operator++(int);
		// Advances the iterator.
		mut_it& operator+=(difference_type diff);
		// Advances the iterator.
		mut_it& operator+=(glm::ivec2 diff);
		// Adds to an iterator.
		friend mut_it operator+(const mut_it& it, difference_type diff);
		// Adds to an iterator.
		friend mut_it operator+(difference_type diff, const mut_it& it);
		// Adds to an iterator.
		friend mut_it operator+(const mut_it& it, glm::ivec2 diff);
		// Adds to an iterator.
		friend mut_it operator+(glm::ivec2 diff, const mut_it& it);

		// Decrements the iterator.
		mut_it& operator--();
		// Post-decrements the iterator.
		mut_it operator--(int);
		// Moves the iterator back.
		mut_it& operator-=(difference_type diff);
		// Moves the iterator back.
		mut_it& operator-=(glm::ivec2 diff);
		// Subtracts from an iterator.
		friend mut_it operator-(const mut_it& it, difference_type diff);
		// Subtracts from an iterator.
		friend mut_it operator-(difference_type diff, const mut_it& it);
		// Subtracts from an iterator.
		friend mut_it operator-(const mut_it& it, glm::ivec2 diff);
		// Subtracts from an iterator.
		friend mut_it operator-(glm::ivec2 diff, const mut_it& it);
		// Gets the distance between two iterators.
		friend difference_type operator-(const mut_it& l, const mut_it& r);

		// Gets the 2D position of the iterator within the bitmap.
		glm::ivec2 pos() const;

	  private:
		// Reference to a pixel, needed for the pointer dereference.
		pixel_ref pixel;
		// The pointed-to-bitmap.
		bitmap* bitmap;
		// The position of the iterator within the bitmap.
		glm::ivec2 bitmap_pos;
	};
} // namespace tr