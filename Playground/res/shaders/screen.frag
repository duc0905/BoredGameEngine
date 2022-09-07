#version 330 core

uniform sampler2D screenTexture;

in vec2 v_TexCoords;
out vec4 color;

void main()
{
	color = texture(screenTexture, v_TexCoords);
	// color = vec4(v_TexCoords, 0.0, 1.0);
}