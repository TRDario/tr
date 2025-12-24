#version 450

layout(location = 0) uniform vec2 backbuffer_size;
layout(location = 1) uniform float character_scale;

layout(location = 0) in vec2 relative_vertex_position;
layout(location = 1) in vec2 character_position;
layout(location = 2) in float right_aligned;
layout(location = 3) in float character;
layout(location = 4) in vec4 foreground_color;
layout(location = 5) in vec4 background_color;

layout(location = 0) out vec2 output_uv;
layout(location = 1) out vec4 output_foreground_color;
layout(location = 2) out vec4 output_background_color;
out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	const float character_size = 8 * character_scale;

	vec2 vertex_position;
	if (right_aligned == 1) {
		vertex_position = character_position * character_size;
		vertex_position.x = backbuffer_size.x - vertex_position.x;
		vertex_position += relative_vertex_position * character_size;
	}
	else {
		vertex_position = (relative_vertex_position + character_position) * character_size;
	}

	output_uv = (vec2(mod(character, 16), floor(character / 16)) + relative_vertex_position) / 16;
	output_foreground_color = foreground_color;
	output_background_color = background_color;
	gl_Position = vec4(vertex_position.x / backbuffer_size.x * 2 - 1, -vertex_position.y / backbuffer_size.y * 2 + 1, 0, 1);
}