#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vNormal;
out vec3 vPos;
out vec2 vTexCoord;

void main() {
    vPos = vec3(uModel * vec4(aPosition, 1.0));
    // TODO: Normal mapping
    vNormal = mat3(uModel) * aNormal;

    vTexCoord = aTexCoord;
    gl_Position = uProjection * uView * vec4(vPos, 1.0);
}
