#version 450

layout(location = 0) uniform mat4 transform;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 color;

layout(location = 0) out vec2 output_uv;
layout(location = 1) out vec4 output_color;
out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	output_uv = uv;
	output_color = color;
	gl_Position = transform * vec4(position, 0, 1);
}