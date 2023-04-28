#version 330 core

in vec2 vUVs;

uniform sampler2D tex;

// out vec4 glColor;
layout(location = 0) out vec4 glColor;

void main()
{
	glColor = texture(tex, vUVs);
	// glColor = vec4(vUVs, 0.0, 1.0);
	// id = 1;
}