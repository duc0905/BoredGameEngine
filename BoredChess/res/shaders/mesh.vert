#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_Normal;

struct MVPMats
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
};

uniform MVPMats mvp = MVPMats(mat4(1.0), mat4(1.0), mat4(1.0));

out vec2 v_UV;
out vec3 v_Normal;

void main()
{
    v_UV = a_UV;
    v_Normal = a_Normal;
    gl_Position = mvp.Proj * mvp.View * mvp.Model * vec4(a_Pos, 1.0);
}