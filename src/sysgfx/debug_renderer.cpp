///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                       //
// Implements debug_renderer.hpp.                                                                                                        //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../include/tr/sysgfx/backbuffer.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/debug_renderer.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"

using namespace std::chrono_literals;

////////////////////////////////////////////////////////////// DEBUG RENDERER /////////////////////////////////////////////////////////////

namespace {
#include "../../resources/generated/debug_renderer_font.hpp"
#include "../../resources/generated/debug_renderer_frag.hpp"
#include "../../resources/generated/debug_renderer_vert.hpp"
} // namespace

// Formats a duration into a string.
static std::string format_duration(std::string_view prefix, tr::duration duration)
{
	if (duration <= 1us) {
		const double count{duration_cast<tr::dnsecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}ns", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
	else if (duration <= 1ms) {
		const double count{duration_cast<tr::dusecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}us", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
	else if (duration <= 1s) {
		const double count{duration_cast<tr::dmsecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}ms", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
	else {
		const double count{duration_cast<tr::dsecs>(duration).count()};
		const int precision{6 - std::clamp(int(std::log10(count)), 0, 5)};
		const std::string format{TR_FMT::format("{}{{:#08.{}f}}s", prefix, precision)};
		return TR_FMT::vformat(format, TR_FMT::make_format_args(count));
	}
}

tr::gfx::debug_renderer::debug_renderer(float scale, u8 column_limit)
	: m_pipeline{vertex_shader{debug_renderer_vert}, fragment_shader{debug_renderer_frag}}
	, m_format{vertex_format_bindings}
	, m_font{load_embedded_bitmap(debug_renderer_font)}
	, m_mesh{std::array<glm::u8vec2, 4>{{{0, 0}, {0, 1}, {1, 1}, {1, 0}}}}
	, m_id{allocate_renderer_id()}
	, m_column_limit{column_limit}
	, m_left_line{0}
	, m_right_line{0}
{
	TR_SET_LABEL(m_pipeline, "(tr) Debug Renderer Pipeline");
	TR_SET_LABEL(m_pipeline.vertex_shader(), "(tr) Debug Renderer Vertex Shader");
	TR_SET_LABEL(m_pipeline.fragment_shader(), "(tr) Debug Renderer Fragment Shader");
	TR_SET_LABEL(m_format, "(tr) Debug Renderer Vertex Format");
	TR_SET_LABEL(m_font, "(tr) Debug Renderer Font Texture");
	TR_SET_LABEL(m_mesh, "(tr) Debug Renderer Vertex Buffer");
	TR_SET_LABEL(m_glyph_buffer, "(tr) Debug Renderer Glyph buffer");

	m_font.set_filtering(min_filter::nearest, mag_filter::nearest);
	m_pipeline.fragment_shader().set_uniform(2, m_font);
	set_scale(scale);
}

void tr::gfx::debug_renderer::set_scale(float scale)
{
	m_pipeline.vertex_shader().set_uniform(1, scale);
}

void tr::gfx::debug_renderer::set_column_limit(u8 columns)
{
	m_column_limit = columns;
}

void tr::gfx::debug_renderer::write_left(std::string_view text, const style& style)
{
	writer{style, m_glyphs, m_left_line, false, m_column_limit, m_glyphs.size()}.write(text);
}

void tr::gfx::debug_renderer::write_right(std::string_view text, const style& style)
{
	writer{style, m_glyphs, m_right_line, true, m_column_limit, m_glyphs.size()}.write(text);
}

void tr::gfx::debug_renderer::write_benchmark(duration min, duration avg, duration max, std::string_view name, duration limit)
{
	constexpr style alt_style{.text_color{255, 0, 0, 255}};

	if (!name.empty()) {
		write_right(TR_FMT::format("{:<15}", name));
	}
	write_right(format_duration("MIN: ", min), min < limit ? default_style : alt_style);
	write_right(format_duration("AVG: ", avg), avg < limit ? default_style : alt_style);
	write_right(format_duration("MAX: ", max), max < limit ? default_style : alt_style);
}

void tr::gfx::debug_renderer::newline_left()
{
	++m_left_line;
}

void tr::gfx::debug_renderer::newline_right()
{
	++m_right_line;
}

void tr::gfx::debug_renderer::draw()
{
	if (!m_glyphs.empty()) {
		m_glyph_buffer.set(m_glyphs);
		m_pipeline.vertex_shader().set_uniform(0, glm::vec2{backbuffer_size()});

		set_render_target(backbuffer_render_target());
		if (should_setup_context(m_id)) {
			set_face_culling(false);
			set_depth_test(false);
			set_blend_mode(alpha_blending);
			set_shader_pipeline(m_pipeline);
			set_vertex_format(m_format);
			set_vertex_buffer(m_mesh, 0, 0);
			set_vertex_buffer(m_glyph_buffer, 1, 0);
		}
		draw_instances(primitive::tri_fan, 0, 4, int(m_glyphs.size()));

		m_glyphs.clear();
	}

	m_left_line = 0;
	m_right_line = 0;
}

////////////////////////////////////////////////////////////////// WRITER /////////////////////////////////////////////////////////////////

tr::gfx::debug_renderer::writer::writer(const style& style, std::vector<glyph>& glyphs, u8& line, bool right_aligned, u8 column_limit,
										usize offset)
	: m_style{style}
	, m_glyphs{glyphs}
	, m_line{line}
	, m_right_aligned{right_aligned}
	, m_column_limit{column_limit}
	, m_text_color{style.text_color}
	, m_background_color{style.background_color}
	, m_line_length{0}
	, m_current_text_start{offset}
	, m_current_line_start{offset}
	, m_current_word_start{offset}
{
}

void tr::gfx::debug_renderer::writer::right_align_current_line_up_to(usize line_end)
{
	for (usize i = m_current_line_start; i < line_end; ++i) {
		m_glyphs[i].pos.x = u8(line_end - i);
	}
}

void tr::gfx::debug_renderer::writer::trim_whitespace_before_current_word()
{
	usize line_end = m_current_word_start - 1;
	while (line_end > m_current_line_start && m_glyphs[line_end].chr != ' ') {
		--line_end;
	}

	if (line_end != m_current_word_start) {
		m_glyphs.erase(m_glyphs.begin() + line_end, m_glyphs.begin() + m_current_word_start);
		m_current_word_start = line_end;
	}
}

void tr::gfx::debug_renderer::writer::move_current_word_to_next_line()
{
	for (glyph& glyph : std::ranges::subrange{m_glyphs.begin() + m_current_word_start, m_glyphs.end()}) {
		glyph.pos = {m_line_length++, m_line};
	}
	m_current_line_start = m_current_word_start;
}

void tr::gfx::debug_renderer::writer::break_before_current_word()
{
	trim_whitespace_before_current_word();
	if (m_right_aligned) {
		right_align_current_line_up_to(m_current_word_start);
	}
	move_current_word_to_next_line();
}

void tr::gfx::debug_renderer::writer::break_current_word()
{
	if (m_right_aligned) {
		right_align_current_line_up_to(m_current_line_start + m_column_limit);
	}
	m_glyphs.back().pos = {m_line_length++, m_line};

	m_current_line_start += m_column_limit;
	m_current_word_start = m_current_line_start;
}

void tr::gfx::debug_renderer::writer::break_current_line()
{
	m_line_length = 0;
	++m_line;

	if (m_current_word_start > m_current_line_start) {
		break_before_current_word();
	}
	else {
		break_current_word();
	}
}

void tr::gfx::debug_renderer::writer::handle_newline()
{
	if (m_right_aligned) {
		right_align_current_line_up_to(m_glyphs.size());
	}

	m_line_length = 0;
	m_current_word_start = m_glyphs.size();
	m_current_line_start = m_glyphs.size();
	++m_line;
}

void tr::gfx::debug_renderer::writer::write_character(char chr)
{
	if (m_line_length == m_column_limit && chr == ' ') {
		handle_newline();
	}
	else {
		m_glyphs.push_back({{m_line_length, m_line}, m_right_aligned, chr, m_text_color, m_background_color});

		if (m_current_text_start < m_glyphs.size() - 1) {
			const char prev{m_glyphs[m_glyphs.size() - 2].chr};
			if (prev == ' ' && chr != ' ') {
				m_current_word_start = m_glyphs.size() - 1;
			}
		}

		if (++m_line_length > m_column_limit) {
			break_current_line();
		}
	}
}

void tr::gfx::debug_renderer::writer::handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end)
{
	switch (*it) {
	case 'b':
		if (std::next(it) != end && std::isdigit(*++it) && u8(*it - '0') < m_style.extra_colors.size()) {
			m_background_color = m_style.extra_colors[*it - '0'];
		}
		break;
	case 'B':
		m_background_color = m_style.background_color;
		break;
	case 'c':
		if (std::next(it) != end && std::isdigit(*++it) && u8(*it - '0') < m_style.extra_colors.size()) {
			m_text_color = m_style.extra_colors[*it - '0'];
		}
		break;
	case 'C':
		m_text_color = m_style.text_color;
		break;
	case 'n':
		handle_newline();
		break;
	case '$':
		write_character('$');
		break;
	default:
		break;
	}
}

void tr::gfx::debug_renderer::writer::write(std::string_view text)
{
	for (std::string_view::iterator it = text.begin(); it != text.end(); ++it) {
		if (*it == '$') {
			if (std::next(it) != text.end()) {
				handle_control_sequence(++it, text.end());
			}
		}
		else {
			write_character(*it);
		}
	}
	handle_newline();
}