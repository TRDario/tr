/// @file
/// @brief Provides utilities relating to `tr::vertex_binding`.

#pragma once
#include <tr/sysgfx/vertex_attribute.hpp>

//

namespace tr
{
	/// Sentinel for a vertex binding that is not instanced (that is, the attributes are per vertex).
	constexpr u32 not_instanced{0};

	/// Vertex binding point.
	struct vertex_binding
	{
		/// Divisor of the binding.
		u32 divisor;

		/// Attributes of the binding. This span is expected to last for the entire duration of the vertex format's lifetime.
		std::span<const vertex_attribute> attrs;
	};

	/// Maps a type (attributes obtained using `tr::as_vertex_attribute_list<T>`) and divisor to a vertex binding at compile-time.
	/// @tparam T Type to get the attribute list for.
	/// @tparam Divisor Divisor of the binding.
	/// @hideinitializer
	template <typename T, u32 Divisor = not_instanced>
	inline constexpr vertex_binding as_vertex_binding{Divisor, as_vertex_attribute_list<T>};

	//

	/// Vertex binding point tag struct used in `tr::as_vertex_bindings`.
	/// @tparam T Type to get the attribute list for.
	/// @tparam Divisor Divisor of the binding.
	template <typename T, u32 Divisor = not_instanced>
	struct vertex_binding_tag
	{
		/// Type to get the attribute list for.
		using type = T;

		/// Divisor of the binding.
		static constexpr u32 divisor{Divisor};
	};

	/// @cond implementation_details

	namespace internal
	{
		/// Holds a list of vertex bindings as if gotten by `tr::as_vertex_binding`.
		/// @tparam BindingTags List of vertex binding tags.
		/// @hideinitializer
		template <specialization_of_tv<vertex_binding_tag>... BindingTags>
		inline constexpr std::array<vertex_binding, sizeof...(BindingTags)> as_vertex_bindings_array{
			as_vertex_binding<typename BindingTags::type, BindingTags::divisor>...,
		};
	} // namespace internal

	/// @endcond

	/// Holds a span of vertex bindings as if gotten by `tr::as_vertex_binding` with a guaranteed static lifetime.
	/// @tparam BindingTags List of vertex binding tags, see `tr::vertex_binding_tag`.
	/// @hideinitializer
	template <specialization_of_tv<vertex_binding_tag>... BindingTags>
	inline constexpr std::span<const vertex_binding, sizeof...(BindingTags)> as_vertex_bindings{
		internal::as_vertex_bindings_array<BindingTags...>,
	};
} // namespace tr