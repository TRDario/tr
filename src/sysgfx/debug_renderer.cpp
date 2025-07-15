#include "../../include/tr/sysgfx/backbuffer.hpp"
#include "../../include/tr/sysgfx/blending.hpp"
#include "../../include/tr/sysgfx/debug_renderer.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include "../../include/tr/sysgfx/graphics_context.hpp"
#include "../../include/tr/sysgfx/render_target.hpp"
#include "../../include/tr/sysgfx/shader_buffer.hpp"
#include "../../include/tr/sysgfx/shader_pipeline.hpp"
#include "../../include/tr/sysgfx/texture.hpp"
#include "../../include/tr/sysgfx/texture_unit.hpp"
#include "../../include/tr/sysgfx/vertex_buffer.hpp"
#include "../../include/tr/sysgfx/vertex_format.hpp"
#include "../../include/tr/utility/benchmark.hpp"

namespace tr {
#include "resources/debug_font.bmp.hpp"

	// Debug vertex shader.
	inline constexpr const char* DEBUG_RENDERER_VERT_SRC =
		"#version 450\n#define L(l) layout(location=l)\nlayout(std430,binding=0)buffer G{int g[];};L(0)uniform vec2 "
		"r;L(1)uniform float s;L(0)in vec2 p;out gl_PerVertex{vec4 gl_Position;};L(0)out vec2 o;L(1)out vec4 T;L(2)out vec4 B;void "
		"main(){const int m=255,i=gl_InstanceID*3,a=g[i],t=g[i+1],b=g[i+2],h=a>>24&m;o=(vec2(h%16,h/16)+p)/"
		"16;T=vec4(t&m,t>>8&m,t>>16&m,t>>24&m)/m;B=vec4(b&m,b>>8&m,b>>16&m,b>>24&m)/m;vec2 "
		"C=vec2(a&m,a>>8&m),P;if((a>>16&m)!=0){P=C*8*s;P.x=r.x-P.x;P+=p*8*s;}else{P=(p+C)*8*s;}gl_Position=vec4(P.x/"
		"r.x*2-1,-P.y/r.y*2+1,0,1);}";
	// Debug fragment shader.
	inline constexpr const char* DEBUG_RENDERER_FRAG_SRC =
		"#version 450\n#define L(l) layout(location=l)\nL(2)uniform sampler2D t;L(0)in vec2 u;L(1)in vec4 c;L(2)in "
		"vec4 b;L(0)out vec4 o;void main(){o=texture(t,u).r==1?c:b;}";
	// Debug renderer ID.
	inline constexpr std::uint32_t DEBUG_RENDERER{2};

	// Debug renderer glyph.
	struct _debug_renderer_glyph {
		// Position on the glyph grid.
		glm::u8vec2 pos;
		// Whether the glyph should be right-aligned.
		bool align_right;
		// The ASCII value of the glyph.
		char chr;
		// The color of the text.
		rgba8 text_color;
		// The color of the background.
		rgba8 bg_color;
	};

	// Debug renderer implementation.
	struct _debug_renderer_state {
		// The pipeline and shaders used by the renderer.
		owning_shader_pipeline pipeline;
		// The debug vertex format.
		vertex_format format;
		// The shader buffer holding the glyphs.
		shader_buffer sbuffer;
		// The font texture.
		texture font;
		// The texture unit used by the renderer.
		texture_unit tex_unit;
		// The vertex buffer holding a glyph shape.
		static_vertex_buffer<glm::u8vec2> vbuffer;
		// List of glyphs to draw.
		std::vector<_debug_renderer_glyph> glyphs;
		// Maximum allowed number of glyphs per line.
		std::uint8_t column_limit;
		// The current left line position.
		std::uint8_t left_line;
		// The current right line position.
		std::uint8_t right_line;

		// Creates the debug renderer.
		_debug_renderer_state() noexcept;
	};
	// Debug renderer state.
	std::optional<_debug_renderer_state> _debug;

	// Context passed to text formatting functions.
	struct _debug_renderer_context {
		// The current line number.
		std::uint8_t& line;
		// Whether the text is right-aligned.
		bool right_aligned;
		// The text color.
		rgba8 text_color;
		// The background color.
		rgba8 bg_color;
		// The length of the current line.
		std::uint8_t line_length;
		// The offset to the start of the text.
		std::size_t text_start;
		// The offset to the start of the line.
		std::size_t line_start;
		// The offset to the start of the word.
		std::size_t word_start;
	};

	// Formats a duration into a string.
	std::string _format_duration(std::string_view prefix, duration duration);
	// Right-aligns a line.
	void _right_align_line(std::size_t begin, std::size_t end) noexcept;
	// Trims trailing whitespace in a line.
	void _trim_trailing_whitespace(_debug_renderer_context& context) noexcept;
	// Moves a word to the next line.
	void _move_word_to_next_line(_debug_renderer_context& context) noexcept;
	// Breaks a line at the last whitespace.
	void _break_at_last_whitespace(_debug_renderer_context& context) noexcept;
	// Breaks an overlong word.
	void _break_overlong_word(_debug_renderer_context& context) noexcept;
	// Handles a line reaching the column limit.
	void _handle_column_limit(_debug_renderer_context& context) noexcept;
	// Writes a character.
	void _write_character(char chr, _debug_renderer_context& context);
	// Handles a newline.
	void _handle_newline(_debug_renderer_context& context);
	// Handles a control sequence.
	void _handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end, _debug_renderer_context& context,
								  rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors);
} // namespace tr

using namespace std::chrono_literals;

tr::_debug_renderer_state::_debug_renderer_state() noexcept
	: pipeline{vertex_shader{DEBUG_RENDERER_VERT_SRC}, fragment_shader{DEBUG_RENDERER_FRAG_SRC}}
	, format{vertex_attributef{vertex_attributef::type::UI8, 2, false, 0, 0}}
	, sbuffer{0, 256 * sizeof(_debug_renderer_glyph), shader_buffer::access::WRITE_ONLY}
	, font{load_embedded_bitmap(DEBUG_RENDERER_FONT)}
	, vbuffer{std::array<glm::u8vec2, 4>{{{0, 0}, {0, 1}, {1, 1}, {1, 0}}}}
	, column_limit{255}
	, left_line{0}
	, right_line{0}
{
	font.set_filtering(min_filter::NEAREST, mag_filter::NEAREST);
	tex_unit.set_texture(font);
	pipeline.fragment_shader().set_uniform(2, tex_unit);
	if (gfx_context::debug()) {
		pipeline.set_label("(tr) Debug Renderer Pipeline");
		pipeline.vertex_shader().set_label("(tr) Debug Renderer Vertex Shader");
		pipeline.fragment_shader().set_label("(tr) Debug Renderer Fragment Shader");
		format.set_label("(tr) Debug Renderer Vertex Format");
		sbuffer.set_label("(tr) Debug Renderer Shader Glyph Buffer");
		font.set_label("(tr) Debug Renderer Font Texture");
		vbuffer.set_label("(tr) Debug Renderer Vertex Buffer");
	}
}

std::string tr::_format_duration(std::string_view prefix, duration duration)
{
	if (duration <= 1us) {
		const double count{duration_cast<dnsecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}ns", prefix, precision), std::make_format_args(count));
	}
	else if (duration <= 1ms) {
		const double count{duration_cast<dusecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}us", prefix, precision), std::make_format_args(count));
	}
	else if (duration <= 1s) {
		const double count{duration_cast<dmsecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}ms", prefix, precision), std::make_format_args(count));
	}
	else {
		const double count{duration_cast<dsecs>(duration).count()};
		const int precision{6 - std::clamp(static_cast<int>(std::log10(count)), 0, 5)};
		return std::vformat(std::format("{}{{:#08.{}f}}s ", prefix, precision), std::make_format_args(count));
	}
}

void tr::_right_align_line(std::size_t begin, std::size_t end) noexcept
{
	for (std::size_t i = begin; i < end; ++i) {
		_debug->glyphs[i].pos.x = static_cast<std::uint8_t>(end - i);
	}
}

void tr::_trim_trailing_whitespace(_debug_renderer_context& context) noexcept
{
	std::size_t line_end = context.word_start - 1;
	while (line_end > context.line_start && _debug->glyphs[line_end].chr != ' ') {
		--line_end;
	}

	if (line_end != context.word_start) {
		_debug->glyphs.erase(_debug->glyphs.begin() + line_end, _debug->glyphs.begin() + context.word_start);
		context.word_start = line_end;
	}
}

void tr::_move_word_to_next_line(_debug_renderer_context& context) noexcept
{
	for (auto it = _debug->glyphs.begin() + context.word_start; it != _debug->glyphs.end(); ++it) {
		it->pos = {context.line_length++, context.line};
	}
	context.line_start = context.word_start;
}

void tr::_break_at_last_whitespace(_debug_renderer_context& context) noexcept
{
	_trim_trailing_whitespace(context);
	if (context.right_aligned) {
		_right_align_line(context.line_start, context.word_start);
	}
	_move_word_to_next_line(context);
}

void tr::_break_overlong_word(_debug_renderer_context& context) noexcept
{
	if (context.right_aligned) {
		_right_align_line(context.line_start, context.line_start + _debug->column_limit);
	}
	_debug->glyphs.back().pos = {context.line_length++, context.line};

	context.line_start += _debug->column_limit;
	context.word_start = context.line_start;
}

void tr::_handle_column_limit(_debug_renderer_context& context) noexcept
{
	context.line_length = 0;
	++context.line;

	if (context.word_start > context.line_start) {
		_break_at_last_whitespace(context);
	}
	else {
		_break_overlong_word(context);
	}
}

void tr::_write_character(char chr, _debug_renderer_context& context)
{
	if (context.line_length == _debug->column_limit && chr == ' ') {
		_handle_newline(context);
	}
	else {
		_debug->glyphs.push_back({{context.line_length, context.line}, context.right_aligned, chr, context.text_color, context.bg_color});

		if (_debug->glyphs.size() - context.text_start > 1) {
			const char prev{_debug->glyphs[_debug->glyphs.size() - 2].chr};
			if (prev == ' ' && chr != ' ') {
				context.word_start = _debug->glyphs.size() - 1;
			}
		}

		if (++context.line_length > _debug->column_limit) {
			_handle_column_limit(context);
		}
	}
}

void tr::_handle_newline(_debug_renderer_context& context)
{
	if (context.right_aligned) {
		_right_align_line(context.line_start, _debug->glyphs.size());
	}

	context.line_length = 0;
	context.word_start = _debug->glyphs.size();
	context.line_start = _debug->glyphs.size();
	++context.line;
}

void tr::_handle_control_sequence(std::string_view::iterator& it, std::string_view::iterator end, _debug_renderer_context& context,
								  rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors)
{
	switch (*it) {
	case 'b':
		if (std::next(it) != end && std::isdigit(*++it) && std::uint8_t(*it - '0') < extra_colors.size()) {
			context.bg_color = extra_colors[*it - '0'];
		}
		break;
	case 'B':
		context.bg_color = bg_color;
		break;
	case 'c':
		if (std::next(it) != end && std::isdigit(*++it) && std::uint8_t(*it - '0') < extra_colors.size()) {
			context.text_color = extra_colors[*it - '0'];
		}
		break;
	case 'C':
		context.text_color = text_color;
		break;
	case 'n':
		_handle_newline(context);
		break;
	case '$':
		_write_character('$', context);
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////// DEBUG RENDERER /////////////////////////////////////////////////////////////

void tr::debug_renderer::initialize(float scale, std::uint8_t column_limit) noexcept
{
	_debug.emplace();
	set_scale(scale);
	set_column_limit(column_limit);
}

bool tr::debug_renderer::active() noexcept
{
	return _debug.has_value();
}

void tr::debug_renderer::shut_down() noexcept
{
	_debug.reset();
}

//

void tr::debug_renderer::set_scale(float scale) noexcept
{
	_debug->pipeline.vertex_shader().set_uniform(1, scale);
}

void tr::debug_renderer::set_column_limit(std::uint8_t columns) noexcept
{
	_debug->column_limit = columns;
}

void tr::debug_renderer::write_left(std::string_view text, rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors) noexcept
{
	const std::size_t old_size{_debug->glyphs.size()};
	_debug_renderer_context context{_debug->left_line, false, text_color, bg_color, 0, old_size, old_size, old_size};

	try {
		for (std::string_view::iterator it = text.begin(); it != text.end(); ++it) {
			if (*it == '$') {
				if (std::next(it) != text.end()) {
					_handle_control_sequence(++it, text.end(), context, text_color, bg_color, extra_colors);
				}
			}
			else {
				_write_character(*it, context);
			}
		}
		_handle_newline(context);
	}
	catch (std::bad_alloc&) {
		TR_TERMINATE("Out of memory", "Exception occurred while writing to debug renderer.");
	}
}

void tr::debug_renderer::write_right(std::string_view text, rgba8 text_color, rgba8 bg_color, std::span<rgba8> extra_colors) noexcept
{
	const std::size_t old_size{_debug->glyphs.size()};
	_debug_renderer_context context{_debug->left_line, true, text_color, bg_color, 0, old_size, old_size, old_size};

	try {
		for (std::string_view::iterator it = text.begin(); it != text.end(); ++it) {
			if (*it == '$') {
				if (std::next(it) != text.end()) {
					_handle_control_sequence(++it, text.end(), context, text_color, bg_color, extra_colors);
				}
			}
			else {
				_write_character(*it, context);
			}
		}
		_handle_newline(context);
	}
	catch (std::bad_alloc&) {
		TR_TERMINATE("Out of memory", "Exception occurred while writing to debug renderer.");
	}
}

void tr::debug_renderer::write_right(const benchmark& benchmark, std::string_view name, duration limit) noexcept
{
	constexpr tr::rgba8 TEXT_COLOR{255, 255, 255, 255};
	constexpr tr::rgba8 ALT_COLOR{255, 0, 0, 255};

	if (!name.empty()) {
		write_right(std::format("{:<15}", name));
	}
	write_right(_format_duration("MIN: ", benchmark.min()), benchmark.min() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(_format_duration("AVG: ", benchmark.avg()), benchmark.avg() < limit ? TEXT_COLOR : ALT_COLOR);
	write_right(_format_duration("MAX: ", benchmark.max()), benchmark.max() < limit ? TEXT_COLOR : ALT_COLOR);
}

void tr::debug_renderer::newline_left() noexcept
{
	++_debug->left_line;
}

void tr::debug_renderer::newline_right() noexcept
{
	++_debug->right_line;
}

void tr::debug_renderer::draw() noexcept
{
	if (!_debug->glyphs.empty()) {
		gfx_context::set_render_target(backbuffer::render_target());
		if (gfx_context::current_renderer() != DEBUG_RENDERER) {
			gfx_context::set_renderer(DEBUG_RENDERER);
			gfx_context::set_blend_mode(ALPHA_BLENDING);
			gfx_context::set_shader_pipeline(_debug->pipeline);
			gfx_context::set_vertex_format(_debug->format);
			gfx_context::set_vertex_buffer(_debug->vbuffer, 0, 0);
		}

		if (static_cast<std::size_t>(_debug->sbuffer.array_capacity()) < _debug->glyphs.size() * sizeof(_debug_renderer_glyph)) {
			std::intptr_t cap{static_cast<std::intptr_t>(std::bit_ceil(_debug->glyphs.size() * sizeof(_debug_renderer_glyph)))};
			_debug->sbuffer = shader_buffer{0, cap, shader_buffer::access::WRITE_ONLY};
			if (gfx_context::debug()) {
				_debug->sbuffer.set_label("(tr) Debug Renderer Glyph Buffer");
			}
		}
		_debug->sbuffer.set_array(_debug->glyphs);
		_debug->pipeline.vertex_shader().set_uniform(0, static_cast<glm::vec2>(backbuffer::size()));
		_debug->pipeline.vertex_shader().set_storage_buffer(0, _debug->sbuffer);
		gfx_context::draw_instances(primitive::TRI_FAN, 0, 4, static_cast<int>(_debug->glyphs.size()));
		_debug->glyphs.clear();
	}

	_debug->left_line = 0;
	_debug->right_line = 0;
}