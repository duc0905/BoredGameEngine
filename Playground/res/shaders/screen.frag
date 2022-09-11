#version 330 core

uniform sampler2D screenTexture;

in vec2 v_TexCoords;

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;

void main()
{
	color = texture(screenTexture, v_TexCoords);
	// color = vec4(v_TexCoords, 0.0, 1.0);
	color2 = 50;
}