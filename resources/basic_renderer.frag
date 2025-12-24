#version 450

layout(location = 1) uniform sampler2D tex;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 output_color;

void main()
{
	output_color = uv.x == -100 ? color : color * texture(tex, uv);
}