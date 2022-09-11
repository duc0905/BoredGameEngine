#version 330 core

in vec3 v_Color;
uniform int id;

layout (location = 0) out vec4 color;
layout (location = 1) out int color2;

void main()
{
	color = vec4(v_Color, 1.0);
	color2 = id;
}