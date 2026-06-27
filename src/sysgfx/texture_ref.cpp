///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements texture_ref.hpp.                                                                                                           //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/texture_ref.hpp"
#include "../../include/tr/sysgfx/texture.hpp"

/////////////////////////////////////////////////////////////// TEXTURE REF ///////////////////////////////////////////////////////////////

tr::texture_ref::texture_ref(const texture& tex)
	: m_ref{tex}
{
	tex.m_references.emplace_back(*this);
}

tr::texture_ref::texture_ref(const texture_ref& r)
	: m_ref{r.m_ref}
{
	if (!empty()) {
		m_ref->m_references.emplace_back(*this);
	}
}

tr::texture_ref::texture_ref(texture_ref&& r) noexcept
	: m_ref{std::exchange(r.m_ref, std::nullopt)}
{
	if (!empty()) {
		std::ranges::replace(m_ref->m_references, tr::ref{r}, tr::ref{*this});
	}
}

tr::texture_ref::~texture_ref()
{
	if (!empty()) {
		unstable_erase(m_ref->m_references, std::ranges::find(m_ref->m_references, *this));
	}
}

//

tr::texture_ref& tr::texture_ref::operator=(std::nullopt_t)
{
	if (!empty()) {
		unstable_erase(m_ref->m_references, std::ranges::find(m_ref->m_references, *this));
	}
	m_ref = std::nullopt;
	return *this;
}

tr::texture_ref& tr::texture_ref::operator=(const texture& tex)
{
	if (!empty()) {
		unstable_erase(m_ref->m_references, std::ranges::find(m_ref->m_references, *this));
	}
	m_ref = tex;
	tex.m_references.emplace_back(*this);
	return *this;
}

tr::texture_ref& tr::texture_ref::operator=(const texture_ref& r)
{
	if (!empty()) {
		unstable_erase(m_ref->m_references, std::ranges::find(m_ref->m_references, *this));
	}
	m_ref = r.m_ref;
	if (!empty()) {
		m_ref->m_references.emplace_back(*this);
	}
	return *this;
}

tr::texture_ref& tr::texture_ref::operator=(texture_ref&& r) noexcept
{
	if (!empty()) {
		unstable_erase(m_ref->m_references, std::ranges::find(m_ref->m_references, *this));
	}
	m_ref = std::exchange(r.m_ref, std::nullopt);
	if (!empty()) {
		std::ranges::replace(m_ref->m_references, tr::ref{r}, tr::ref{*this});
	}
	return *this;
}

//

const tr::texture& tr::texture_ref::operator*() const
{
	return *m_ref;
}

const tr::texture* tr::texture_ref::operator->() const
{
	return m_ref.operator->();
}

//

bool tr::texture_ref::empty() const
{
	return !m_ref.has_ref();
}

//

void tr::texture_ref::unbind()
{
	m_ref = std::nullopt;
}

void tr::texture_ref::rebind(const texture& texture)
{
	m_ref = texture;
}