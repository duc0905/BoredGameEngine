#version 330 core

in vec2 v_TexCoords;
uniform int u_ActorID;

layout (location = 0) out vec4 color;
layout (location = 1) out int actorID;

uniform sampler2D sampler;

void main()
{
	color = texture2D(sampler, v_TexCoords);
	actorID = u_ActorID;
}