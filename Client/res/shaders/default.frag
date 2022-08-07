#version 330 core

// in vec2 v_TexCoord;
in vec4 v_Color;

// uniform sampler2D u_Texture;

out vec4 color;

void main()
{
	// vec4 texColor = texture(u_Texture, v_TexCoord);
	color = v_Color;
	// color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
};