#version 450

layout(location = 2) uniform sampler2D font;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 foreground_color;
layout(location = 2) in vec4 background_color;

layout(location = 0) out vec4 output_color;

void main()
{
	output_color = texture(font, uv).r == 1 ? foreground_color : background_color;
}