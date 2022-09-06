#version 330 core

in vec2 v_Pos;
out vec4 color;

void main()
{
	color = vec4((v_Pos.x + 1.0) / 2, (v_Pos.y + 1.0) / 2, 0.0f, 1.0f);
}